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
    connect(dReader,SIGNAL(SendWord(QString)),this,SLOT(UpdateTextEdit(QString)),Qt::QueuedConnection);
    connect(dReader,SIGNAL(Start()),ui->plainTextEdit,SLOT(clear()),Qt::QueuedConnection);
    connect(this,SIGNAL(StopParsing()),dReader,SLOT(StopParsing()),Qt::QueuedConnection);
    connect(this,SIGNAL(StartParsing(QString)),dReader,SLOT(StartParsing(QString)),Qt::QueuedConnection);

    timer.setInterval(200);
    connect(&timer,SIGNAL(timeout()),dReader,SLOT(SendBuffer()),Qt::QueuedConnection);
    thread->start();


    connect(dReader,SIGNAL(Start()),this,SLOT(printLoading()),Qt::QueuedConnection);
    connect(dReader,SIGNAL(Complete()),this,SLOT(printComplete()),Qt::QueuedConnection);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::UpdateTextEdit(const QString &str){
    QTextDocument* doc = ui->plainTextEdit->document();
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(str);
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    emit StopParsing();
    emit StartParsing(arg1);
    timer.start();
}

void MainWindow::printLoading()
{
    ui->statusbar->showMessage("Loading...");
}

void MainWindow::printComplete()
{
    ui->statusbar->showMessage("Complete",2000);
}
