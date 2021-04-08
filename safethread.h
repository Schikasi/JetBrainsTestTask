#ifndef SAFETHREAD_H
#define SAFETHREAD_H

#include <QObject>
#include <QThread>

class SafeThread : public QThread {
  Q_OBJECT
  using QThread::run;
public:
  ~SafeThread();
};


#endif // SAFETHREAD_H
