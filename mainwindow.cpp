#include "dictionaryreader.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this,&MainWindow::Clear,this,[&]{
        if(m_timer.isActive())m_timer.stop();
        bufferResults.clear();
        ui->plainTextEdit->clear();
    });

    connect(this,&MainWindow::StartParsing,this,[&]{
        isReceivingComplete = false;
        if(!m_timer.isActive())m_timer.start(0,this);
        ui->statusbar->showMessage("Loading...");
    });

    connect(this,&MainWindow::CompleteReceiving,this,[&]{
        isReceivingComplete = true;
    });

    connect(ui->lineEdit,&QLineEdit::textEdited,this,&MainWindow::SendPattern);
    connect(ui->checkBox,&QCheckBox::stateChanged,this,&MainWindow::SendSubSeqOption);
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

void MainWindow::AppendBuffer(const QStringList &buffer){
    bufferResults.append(buffer.mid(0).join(""));
}

void MainWindow::UpdateTextEdit(){
    QTextDocument* doc = ui->plainTextEdit->document();
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::End);
    if(!bufferResults.empty()){
        cursor.insertText(bufferResults.front());
        bufferResults.pop_front();
    }else{
        if(isReceivingComplete){
            ui->statusbar->showMessage("Complete",2000);
            m_timer.stop();
        }
    }
}

