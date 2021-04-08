#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCursor>
#include <QTextDocument>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    //Преобразует список строк в строку и добавляет в конец буфера
    void AppendBuffer(const QStringList &);
private slots:
    //Выводит верхний блок совпавших слов из буфера
    void UpdateTextEdit();
    void timerEvent(QTimerEvent *event);
private:
    Ui::MainWindow *ui;
    QBasicTimer m_timer;
    //Буффер блоков совпавших слов
    QStringList bufferResults;
    //Флаг завершения приёма всех совпадений
    bool isReceivingComplete = true;
signals:
    void StartParsing();
    void SendPattern(const QString &);
    void SendSubSeqOption(const int &);
    void CompleteReceiving();
    void Clear();
};
#endif // MAINWINDOW_H
