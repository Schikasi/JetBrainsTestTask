#ifndef DICTIONARYREADER_H
#define DICTIONARYREADER_H


#include <QBasicTimer>
#include <QTimerEvent>
#include <QFile>
#include <QObject>
#include <QTextStream>



class DictionaryReader: public QObject
{
    Q_OBJECT
private:
    //Файл словаря в котором происходит поиск
    QFile dict;
    QTextStream *stream= nullptr;
    //Паттерн по которому флгоритм ищет словов в словаре
    QString pattern="";
    //Буфер строк, которые совпали с паттерном
    QStringList bufferResults;
    //Флаг активности процесса
    bool isStoped = true;
    //Флаг меняющий алгоритм поиска (подстроки/подпоследовательности)
    bool SrchSbSqns = false;
    //Колличесвто строк считываемых за раз
    static const int CountRows = 2048;
    QBasicTimer m_timer;
    //Функция проверки входимости паттерна в качестве подпослежовательности в исследуемую строку
    bool containsSubSequence(const QString &string, const QString &pattern);
    //Обрабатывает следующий блок слов и формирует буфер
    void ParseNext();
    void timerEvent(QTimerEvent *event);
public:
    DictionaryReader(const QString &pathFile);
    ~DictionaryReader();
public slots:
    void SetPattern(const QString &);
    void SetSrchCbSqns(const int &);
    void StartParsing();
    void StopParsing();
    void OpenDictionary();
    void CloseDictionary();
signals:
    void SendMatchingWords(const QStringList &);
    void Start();
    void Complete();
};

#endif // DICTIONARYREADER_H
