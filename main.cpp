#include "mainwindow.h"
#include "safethread.h"

#include "dictionaryreader.h"
#include <QApplication>
#include <QThread>
#include <QObject>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QString path = (argc == 2) ? argv[1] : "words.txt";

    if(!QFile::exists(path)){
        QMessageBox msg;
        msg.setText("Файл: \""+path+"\" не существует.");
        msg.exec();
        return 0;
    }

    SafeThread thread;
    DictionaryReader dReader(path);
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
