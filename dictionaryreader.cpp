#include "dictionaryreader.h"

#include <QFile>
#include <QTimerEvent>

DictionaryReader::DictionaryReader(const QString &pathFile):dict(pathFile)
{
    //connect(this,SIGNAL(Start()),this,SLOT(ParseNext()),Qt::QueuedConnection);
    //connect(this,SIGNAL(Next()),this,SLOT(ParseNext()),Qt::QueuedConnection);
    connect(this,SIGNAL(Complete()),this,SLOT(StopParsing()),Qt::QueuedConnection);

}

void DictionaryReader::timerEvent(QTimerEvent *event) {
  if (event->timerId() == m_timer.timerId()){
    ParseNext();
    SendBuffer();
  }
}

DictionaryReader::~DictionaryReader()
{
    CloseDictionary();
}

void DictionaryReader::OpenDictionary(){
    dict.open(QIODevice::ReadOnly);
    stream = new QTextStream(&dict);
}

void DictionaryReader::CloseDictionary(){
    if(stream != nullptr){
        delete stream;
    }
    if(dict.isOpen()){
        dict.close();
    }
}

void DictionaryReader::SendBuffer()
{
    emit SendWord(bufferResults);
    bufferResults.clear();
}

void DictionaryReader::StartParsing(const QString &newPattern){
    pattern = newPattern;
    isStoped = false;
    OpenDictionary();
    m_timer.start(0,this);
    emit Start();
}

void DictionaryReader::StopParsing(){
    SendBuffer();
    m_timer.stop();
    if(!isStoped)CloseDictionary();
    isStoped = true;
}

void DictionaryReader::ParseNext(){
    if(isStoped)return;
    for(int i=0;i<1000;++i){
        if(stream->atEnd()||pattern==""){
            emit Complete();
            return;
        }
        (*stream)>>tmpBuf;
        if(tmpBuf.contains(pattern))
            bufferResults.append(tmpBuf+"\n");
    }
}
