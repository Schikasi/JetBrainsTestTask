#include "dictionaryreader.h"


DictionaryReader::DictionaryReader(const QString &pathFile):dict(pathFile)
{
    connect(this,&DictionaryReader::Complete,this,&DictionaryReader::StopParsing);
}

void DictionaryReader::timerEvent(QTimerEvent *event) {
  if (event->timerId() == m_timer.timerId()){
    ParseNext();
    if(!bufferResults.empty()){
        emit SendMatchingWords(bufferResults);
        bufferResults.clear();
    }
  }
}

DictionaryReader::~DictionaryReader()
{
    CloseDictionary();
}

void DictionaryReader::SetPattern(const QString & newPattern)
{
    if(pattern == newPattern)return;
    pattern = newPattern;
    if(!isStoped)StopParsing();
    StartParsing();
}

void DictionaryReader::SetSrchCbSqns(const int &state)
{
    if(SrchSbSqns==static_cast<bool>(state))return;
    SrchSbSqns = static_cast<bool>(state);
    if(!isStoped)StopParsing();
    StartParsing();
}

void DictionaryReader::OpenDictionary(){
    dict.open(QIODevice::ReadOnly);
    stream = new QTextStream(&dict);
}

void DictionaryReader::CloseDictionary(){
    if(dict.isOpen()){
        dict.close();
    }
    if(stream != nullptr){
        delete stream;
        stream = nullptr;
    }
}

void DictionaryReader::StartParsing(){
    OpenDictionary();
    m_timer.start(0,this);
    isStoped = false;
    emit Start();
}

void DictionaryReader::StopParsing(){
    bufferResults.clear();
    m_timer.stop();
    CloseDictionary();
    isStoped = true;
}

void DictionaryReader::ParseNext(){
    if(isStoped)return;
    QString tmpBuf;
    for(int i=0;i<CountRows;++i){
        if(stream->atEnd()||pattern==""){
            emit Complete();
            return;
        }
        (*stream)>>tmpBuf;
        if(SrchSbSqns){
            if(containsSubSequence(tmpBuf,pattern))
                bufferResults.append(tmpBuf+"\n");
        }else{
            if(tmpBuf.contains(pattern))
                bufferResults.append(tmpBuf+"\n");
        }
    }
}

bool DictionaryReader::containsSubSequence(const QString &string, const QString &pattern){
    int j = 0;
    for(int i= 0;i<string.length();++i){
        if(string[i]==pattern[j]){
            ++j;
            if(j==pattern.length())
                return true;
        }
    }
    return false;
}
