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
    void UpdateTextEdit(const QString &);

private slots:
    void on_lineEdit_textChanged(const QString &);
    void printLoading();
    void printComplete();

private:
    Ui::MainWindow *ui;
    QTimer timer;
signals:
    void StopParsing();
    void StartParsing(const QString &);
};
#endif // MAINWINDOW_H
