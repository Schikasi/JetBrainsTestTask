#include "dictionaryreader.h"

#include <QFile>

DictionaryReader::DictionaryReader(const QString &pathFile):dict(pathFile)
{
    connect(this,SIGNAL(Start()),this,SLOT(ParseNext()),Qt::QueuedConnection);
    connect(this,SIGNAL(Next()),this,SLOT(ParseNext()),Qt::QueuedConnection);
    connect(this,SIGNAL(Complete()),this,SLOT(StopParsing()),Qt::QueuedConnection);
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
    emit SendWord(buffer);
    buffer.clear();
}

void DictionaryReader::StartParsing(const QString &newPattern){
    pattern = newPattern;
    isStoped = false;
    OpenDictionary();
    emit Start();
}

void DictionaryReader::StopParsing(){
    SendBuffer();
    if(!isStoped)CloseDictionary();
    isStoped = true;
}

void DictionaryReader::ParseNext(){
    if(isStoped)return;
    if(stream->atEnd()||pattern==""){
        emit Complete();
        return;
    }
    (*stream)>>tmpBuf;
    if(tmpBuf.contains(pattern))
        buffer.append(tmpBuf+"\n");
    emit Next();
}
