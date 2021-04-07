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
    void UpdateTextEdit();

    void UpdateBuffer(const QStringList &str);
private slots:
    void on_lineEdit_textChanged(const QString &);
    void printLoading();

    void timerEvent(QTimerEvent *event);
private:
    Ui::MainWindow *ui;
    QBasicTimer m_timer;
    QStringList bufferResults;
    bool flComputeComlete = true;
signals:
    void StopParsing();
    void StartParsing(const QString &);
    void SP();
};
#endif // MAINWINDOW_H
