#include "ubtsendfilethread.h"
#include  "../UBTUtil/MD5.h"
#include "../UBTUtil/filedirhandle.h"
#include "../UBXRobotController/Robot/SerialPort.h"
#include "../common/UBXPublic.h"
#include "../UBTUtil/cstringhandle.h"
#include <QDebug>
UBTSendFileThread::UBTSendFileThread()
{

}

void UBTSendFileThread::run()
{
    qDebug()<<"MainWindow::onSynchronizeClicked strFileName:" <<m_strCurFileName<<endl;
    QFile file(m_strCurFileName);
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly);
    }
    QByteArray contentArray = file.readAll();
    file.close();
    std::string strContent = contentArray.toStdString();

    bool bRet = false;
    {
        QString strBasename = CFileDirHandle::getFileBaseName(m_strCurFileName);
        qDebug() << "MainWindow::onSynchronizeClicked strBasename:" << strBasename << endl;
        char* pBaseName = NULL;
        UnicodeToUtf8(strBasename, pBaseName);

        //     m_objDataProcessor.Req_SendData(m_pPort, UBXPROTOCOL_CMDID_STOP, NULL, 1);

        size_t nFrameCount = (strContent.length() - 1) / UBXPROTOCOL_PACKAGE_SIZE + 1;;
        qDebug("FrameCount = %d\n", nFrameCount);
        byte* pBeginSendFileData = NULL;
        int nOutLen = m_objDataProcessor.Req_PackageBeginSendFileData((byte*)pBaseName, strlen(pBaseName), nFrameCount, pBeginSendFileData);
        SAFE_DELETE_ARRAY(pBaseName);
        bRet = m_objDataProcessor.Req_SendData(UBXPROTOCOL_CMDID_BEGIN_SEND_FILE, pBeginSendFileData, nOutLen + 1);
        SAFE_DELETE_ARRAY(pBeginSendFileData);
    }

    if (bRet)
    {
        bRet =  m_objDataProcessor.Req_SendFile(UBXPROTOCOL_CMDID_SENDING_FILE, (byte*)strContent.c_str(), strContent.length());
    }

    if (bRet)
    {
        CMD5 md5obj;
        md5obj.GenerateMD5((byte*)strContent.c_str(), strContent.length());
        std::string strRequestKey = md5obj.ToString();//return local static variable
        qDebug() << "MainWindow::onSynchronizeClicked pszRequestKey:" << strRequestKey.c_str() << "len:" << strRequestKey.length() << endl;
        byte* pMd5Hex = new byte[strRequestKey.length() / 2];
        CStringHandle::HexStrToByte(strRequestKey.c_str(), pMd5Hex, strRequestKey.length());
        m_objDataProcessor.Req_SendData(UBXPROTOCOL_CMDID_END_SEND_FILE, pMd5Hex, strRequestKey.length()/2);
        SAFE_DELETE_ARRAY(pMd5Hex);
    }
}

