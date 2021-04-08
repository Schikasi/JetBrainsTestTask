#include "mainwindow.h"
#include "safethread.h"

#include "dictionaryreader.h"
#include <QApplication>
#include <QThread>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    SafeThread thread;
    DictionaryReader dReader("file.txt");
    dReader.moveToThread(&thread);

    QObject::connect(&dReader,&DictionaryReader::SendMatchingWords,&w,&MainWindow::AppendBuffer);
    QObject::connect(&w,&MainWindow::SendPattern,&dReader,&DictionaryReader::SetPattern);
    QObject::connect(&w,&MainWindow::SendSubSeqOption,&dReader,&DictionaryReader::SetSrchCbSqns);
    QObject::connect(&dReader,&DictionaryReader::Start,&w,&MainWindow::Clear);
    QObject::connect(&dReader,&DictionaryReader::Start,&w,&MainWindow::StartParsing);
    QObject::connect(&dReader,&DictionaryReader::Complete,&w,&MainWindow::CompleteReceiving);

    thread.start();
    w.show();
    return a.exec();
}
