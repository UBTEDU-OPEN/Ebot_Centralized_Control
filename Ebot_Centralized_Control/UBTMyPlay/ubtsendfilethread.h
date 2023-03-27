#ifndef UBTSENDFILETHREAD_H
#define UBTSENDFILETHREAD_H
#include <QThread>
#include "ubxdataprocessor.h"

class UBTSendFileThread: public QThread
{
    Q_OBJECT
public:
    UBTSendFileThread();
    void SetCurFileName(QString strFileName){m_strCurFileName = strFileName;}
protected:
    virtual void run();
private:
    QString m_strCurFileName;
    UBXDataProcessor  m_objDataProcessor;  //数据处理器
};

#endif // UBTSENDFILETHREAD_H
