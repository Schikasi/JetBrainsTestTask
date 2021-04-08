#include "dictionaryreader.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QThread* thread = new QThread();
    DictionaryReader* dReader = new DictionaryReader("file.txt");
    dReader->moveToThread(thread);
    connect(dReader,SIGNAL(SendWord(QStringList)),this,SLOT(UpdateBuffer(QStringList)),Qt::QueuedConnection);
    connect(this,SIGNAL(StopParsing()),dReader,SLOT(StopParsing()),Qt::QueuedConnection);
    connect(this,SIGNAL(StartParsing(QString)),dReader,SLOT(StartParsing(QString)),Qt::QueuedConnection);

    thread->start();

    connect(dReader,&DictionaryReader::Start,this,[this]{bufferResults.clear();});
    connect(dReader,SIGNAL(Start()),ui->plainTextEdit,SLOT(clear()),Qt::QueuedConnection);
    connect(dReader,SIGNAL(Start()),this,SLOT(printLoading()),Qt::QueuedConnection);
    connect(dReader,&DictionaryReader::Complete,this,[&]{flComputeComlete=true;});
}

void MainWindow::timerEvent(QTimerEvent *event) {
  if (event->timerId() == m_timer.timerId()){
      UpdateTextEdit();
  }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateBuffer(const QStringList &buffer){
    bufferResults.append(buffer.mid(0,-1).join(""));
}

void MainWindow::UpdateTextEdit(){
    QTextDocument* doc = ui->plainTextEdit->document();
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::End);
    if(!bufferResults.empty()){
        cursor.insertText(bufferResults.front());
        bufferResults.pop_front();
    }
    if(bufferResults.empty()&&flComputeComlete){
        ui->statusbar->showMessage("Complete",2000);
        m_timer.stop();
    }

}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    m_timer.stop();
    flComputeComlete = false;
    emit StopParsing();
    emit StartParsing(arg1);
    m_timer.start(0,this);
}

void MainWindow::printLoading()
{
    ui->statusbar->showMessage("Loading...");
}
