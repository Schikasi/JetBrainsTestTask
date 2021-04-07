#ifndef DICTIONARYREADER_H
#define DICTIONARYREADER_H

#include <QBasicTimer>
#include <QFile>
#include <QObject>
#include <QTextStream>



class DictionaryReader: public QObject
{
    Q_OBJECT
private:
    QString tmpBuf = "";
    QString buffer="";
    bool isStoped = true;
    QString pattern="";
    QFile dict;
    QTextStream *stream= nullptr;

    QStringList bufferResults;

    QBasicTimer m_timer;public:
    DictionaryReader(const QString &pathFile);
    ~DictionaryReader();
public slots:
    void StartParsing(const QString &);
    void StopParsing();
    void OpenDictionary();
    void CloseDictionary();
    void SendBuffer();
signals:
    void SendWord(const QStringList &);
    void Start();
    void Complete();
    void Next();
private slots:
    void ParseNext();
    void timerEvent(QTimerEvent *event);
};

#endif // DICTIONARYREADER_H
