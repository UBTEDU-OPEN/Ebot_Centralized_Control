/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：Util.cpp
* 创建时间：2015/05/14 17:25
* 文件标识：
* 文件摘要：辅助函数类。
*
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/05/14 17:25
* 版本摘要：
*/
#include "ubtutil.h"
#include "filedirhandle.h"
#include "WinVer.h"

#include <QTextStream>
#include <QTextCodec>
#include <QDir>
#include <QLayout>
#include <QWidget>

CUtil::CUtil(void)
{
}

CUtil::~CUtil(void)
{
}

/**************************************************************************
* FunctionName:
*     StrSplit
* Description:
*     字符串分割
* Parameter:
*     @[in ] source: 源字符串
*     @[in ] ch: 分割字符
* ReturnValue:
*     返回分割后的字符串列表
*/
vector<QString> CUtil::StrSplit(const QString &source, char ch)
{
    vector<QString> strlist;
    int m=0,n=0;
    for(int i=0;i<source.length();)
    {
        if(source[i]==ch)
        {
            n=i-1;
            QString str = source.mid(m,n-m+1);
            strlist.push_back(str);

            m=n+2;
            n=m;
            i=m;
        }
        else
            i++;
    }

    //以分割符结尾的时候  m的值最终等于source的长度
    //反之  m的值小于source的长度
    if(m>0&&m<source.length())
    {
        strlist.push_back(source.mid(m));
    }
    return strlist;
}

/**************************************************************************
* FunctionName:
*     StrSplit
* Description:
*     字符串分割
* Parameter:
*     @[in ] source: 源字符串
*     @[in ] frontch: 前分割字符
*     @[in ] rearch: 后分割字符
* ReturnValue:
*     返回分割后的字符串列表
*/
vector<QString> CUtil::StrSplit(const QString &source, char frontch, char rearch)
{
    vector<QString> strlist;
    int m=0,n=0;

    for(int i=0;i<source.length();)
    {
        BOOL bfindrear = FALSE;
        if(source[i]==frontch)
        {
            m=i+1;

            for(int j=m;j<source.length();j++)
            {
                if(source[j]==rearch)
                {
                    bfindrear = TRUE;

                    n=j-1;

                    QString str = source.mid(m,n-m+1);
                    strlist.push_back(str);

                    i=j+1;//跳过rear
                    m=n=i;
                    break;
                }
            }
        }
        if(!bfindrear)
            i++;
    }

    //以rear结尾的时候  m的值最终等于source的长度
    //反之  m的值小于source的长度
    if(m>0&&m<source.length())
    {
        strlist.push_back(source.mid(m));
    }
    return strlist;
}

/**************************************************************************
* FunctionName:
*     StrSplit
* Description:
*     字符串分割
* Parameter:
*     @[in ] source: 源字符串
*     @[in ] splitstr: 分割字符串
* ReturnValue:
*     返回分割后的字符串列表
*/
vector<QString> CUtil::StrSplit(const QString &source, const QString &splitstr)
{
    vector<QString> strlist;
    int index1 = 0;
    while(index1 < source.length())
    {
        int index2 = source.indexOf(splitstr, index1);
        if(index2 != -1)
        {
            strlist.push_back(source.mid(index1, index2-index1));
            index1 = index2 + splitstr.length();
        }
        else
        {
            strlist.push_back(source.mid(index1));
            index1 = source.length();
        }
    }
    return strlist;
}

/**************************************************************************
* FunctionName:
*     StrSplit
* Description:
*     字符串分割
* Parameter:
*     @[in ] source: 源字符串
*     @[in ] frontstr: 前分割字符串
*     @[in ] rearstr: 后分割字符串
* ReturnValue:
*     返回分割后的字符串列表
*/
vector<QString> CUtil::StrSplit(const QString &source, const QString &frontstr, const QString &rearstr)
{
    vector<QString> strlist;
    int index1 = 0;
    while(index1 < source.length())
    {
        int index2 = source.indexOf(frontstr, index1);
        int index3 = source.indexOf(rearstr, index1);
        if(index2 != -1)
        {
            if(index3 != -1)
            {
                strlist.push_back(source.mid(index2+frontstr.length(), index3-index2-frontstr.length()));
                index1 = index3 + rearstr.length();
            }
            else
            {
                index1 = source.length();
            }
        }
        else
        {
            index1 = source.length();
        }
    }
    return strlist;
}

/**************************************************************************
* 函数名: GetFileSize
* 功能: 获取文件大小
* 参数:
*    @[in ] sFilePath: 文件路径
* 返回值: 文件大小
* 时间: 2016/01/04 09:47
* 作者: ZDJ
*/
qint64 CUtil::GetFileSize(const QString& sFilePath)
{
    qint64 nSize = 0;
    QFile file(sFilePath);
    if(file.open(QIODevice::ReadOnly))
    {
        nSize = file.size();
        file.close();
    }
    return nSize;
}

/**************************************************************************
* 函数名: GetFileExt
* 功能: 获取文件后缀名
* 参数:
*    @[in ] sFilePath: 文件路径
* 返回值: 文件后缀名
* 时间: 2016/01/13 16:17
* 作者: ZDJ
*/
QString CUtil::GetFileExt(const QString& sFilePath)
{
    return sFilePath.mid(sFilePath.lastIndexOf('.'));
}

/**************************************************************************
* 函数名: GetQString
* 功能: 获取字符串
* 参数:
*    @[in ] pStr: WCHAR字符串指针
*    @[in ] nLen: 字符串字节数
* 返回值: QString字符串
* 时间: 2016/01/13 19:40
* 作者: ZDJ
*/
QString CUtil::GetQString(WCHAR* pStr, int nLen)
{
    int nAnsiLen = nLen / sizeof(WCHAR);
    char* pAnsiStr = new char[nAnsiLen + 1];
    memset(pAnsiStr, 0, nAnsiLen + 1);
    for(int i = 0; i < nAnsiLen; i++)
    {
        pAnsiStr[i] = *((char*)pStr + i * sizeof(WCHAR));
    }
    QString sTemp = QString::fromStdString(pAnsiStr);
    SAFE_DELETE_ARRAY(pAnsiStr);
    return sTemp;
}

/**************************************************************************
* 函数名: GetWString
* 功能: 获取宽字符串
* 参数:
*    @[in ] str: 字符串
*    @[out] pWStr: WCHAR字符串指针
* 返回值: 字符串字节数
* 时间: 2016/01/13 19:40
* 作者: ZDJ
*/
int CUtil::GetWString(const QString& str, WCHAR* pWStr)
{
    string sAnsi = str.toStdString();
    const char* pAnsiStr = sAnsi.c_str();
    int nAnsiLen = sAnsi.length();
    int nOutLen = (nAnsiLen + 1) * sizeof(WCHAR);

    memset(pWStr, 0, nOutLen);
    for(int i = 0; i < nAnsiLen; i++)
    {
        memcpy(pWStr + i, pAnsiStr + i, 1);
    }
    return nOutLen;
}

/**************************************************************************
* 函数名: ReplaceFileExt
* 功能: 替换文件后缀名
* 参数:
*    @[in ] sSrcPath: 文件路径
*    @[in ] sNewExt: 文件新后缀
* 返回值: 新文件路径
* 时间: 2015/09/18 20:29
* 作者: ZDJ
*/
QString CUtil::ReplaceFileExt(const QString& sSrcPath, const QString& sNewExt)
{
    QString sRet;
    int nDotIndex = sSrcPath.lastIndexOf('.');
    if(sNewExt.length() > 0 )
    {
        if(sNewExt[0] == '.')
        {
            sRet = sSrcPath.left(nDotIndex) + sNewExt;
        }
        else
        {
            sRet = sSrcPath.left(nDotIndex + 1) + sNewExt;
        }
    }
    else
    {
        sRet = sSrcPath;
    }

    return sRet;
}
/**************************************************************************
* FunctionName: listFilter
* Description : get the filted list
* Parameter:
*     @[in] inStringList: the input list will be filted
*     @[in] filterList: the filter list will be removed
* ReturnValue: QStringList
* date  : 20160601
* author: suhaiwu
*/
QStringList CUtil::listFilter(const QStringList &inStringList, const QStringList &filterList)
{
    if(inStringList.isEmpty() || filterList.isEmpty())
    {
        return inStringList;
    }

    QStringList retList;
    retList = inStringList;

    const int nCountFilter = filterList.count();
    for(int index = 0; index < nCountFilter; ++index)
    {
        if(retList.contains(filterList.at(index)))
        {
            retList.removeAll(filterList.at(index));
        }
    }

    return retList;
}

bool CUtil::moveToLastPoint(const char *pChar)
{
    if(!pChar)
    {
        return false;
    }

    int nLength = strlen(pChar);

    pChar = pChar + nLength -1;

    return true;
}

void CUtil::cleanLayout(QLayout *layout)
{
    while (layout->count() != 0) {
        auto layoutItem = layout->takeAt(0);
        if (auto layout = layoutItem->layout()) {
            cleanLayout(layout);
        }
        else if (auto w = layoutItem->widget()) {
            w->hide();
            w->deleteLater();
        }
        delete layoutItem;
    }
}

int CUtil::getActTotalTime(const QString &htsFilePath)
{
//    const int HTS_PER_PAGE_SIZE = 33;
//    const int HTS_HEAD_PART_LENGTH = 33;
//    const int HTS_TAIL_PART_LENGTH = 33;
//    const int HTS_PROTOCOL_HEAD = 0XFB;
//    const int HTS_PROTOCOL_END = 0XED;
//    enum emHtsFlagPos
//    {
//        ePosStart = 0,
//        ePosFrameStat = 3,
//        ePosFrameTotalCount = 4,
//        ePosFrameCur = 6,
//        ePosSteerAngle = 8,
//        ePosActTime = 28,
//        ePosActTotalTime = 29,
//        ePosCheck = 31,

//        ePosEnd = 32
//    };

    const int HTS_TAIL_PART_LENGTH = 33;
    const int ePosActTotalTime = 29;

    int totalTimeMillisecond = -1;

    char* pInDataHts = nullptr;
    long inHtsDataLen = CFileDirHandle::read(htsFilePath, &pInDataHts);

    // CHtsParser::getActTotalTime((const char *)pInDataHts, inHtsDataLen, totalTimeMillisecond);
    // CHtsParser::getActTotalTime(const char *const pInDataHts, int64 inHtsDataLen, int &totalTimeMillisecond)

    if(!pInDataHts) {
        return false;
    }

    if( HTS_TAIL_PART_LENGTH > inHtsDataLen) {
        return false;
    }

    char * pHtsData = pInDataHts;
    //to the last point
    pHtsData = pHtsData + inHtsDataLen - 1;

    //to the tail part first point
    pHtsData = pHtsData - HTS_TAIL_PART_LENGTH + 1;

    uchar time0 = pHtsData[ePosActTotalTime];
    uchar time1 = pHtsData[ePosActTotalTime + 1];
    uchar time2 = pHtsData[ePosActTotalTime + 2];
    uchar time3 = pHtsData[ePosActTotalTime + 3];

    int ntime0 = (int)time0;
    int ntime1 = ((int)time1)<<8;
    int ntime2 = ((int)time2)<<16;
    int ntime3 = ((int)time3)<<24;

    totalTimeMillisecond = ntime0 + ntime1 + ntime2 + ntime3;//ntime0 union is ms

    return totalTimeMillisecond;
}

bool CUtil::getFileVersion(const QString &sAppPath, FILEVERINFO &fileinfo)
{
    bool bRet = false;

#ifdef Q_OS_WIN
    LPVOID pBlock = NULL;

    enum VersionKeyEnum
    {
        eVersionKeyEnumComments = 0,
        eVersionKeyEnumInternalName,     //内部名称
        eVersionKeyEnumProductName,      //产品名称
        eVersionKeyEnumCompanyName,      //公司名称
        eVersionKeyEnumLegalCopyright,   //合法版权
        eVersionKeyEnumProductVersion,   //产品版本
        eVersionKeyEnumFileDescription,  //文件描述
        eVersionKeyEnumLegalTrademarks,  //合法商标
        eVersionKeyEnumPrivateBuild,
        eVersionKeyEnumFileVersion,      //文件版本
        eVersionKeyEnumOriginalFilename, //原始文件名
        eVersionKeyEnumSpecialBuild,
    };

    QStringList sVersionKeyArr;
    sVersionKeyArr.append("Comments");
    sVersionKeyArr.append("InternalName");
    sVersionKeyArr.append("ProductName");
    sVersionKeyArr.append("CompanyName");
    sVersionKeyArr.append("LegalCopyright");
    sVersionKeyArr.append("ProductVersion");
    sVersionKeyArr.append("FileDescription");
    sVersionKeyArr.append("LegalTrademarks");
    sVersionKeyArr.append("PrivateBuild");
    sVersionKeyArr.append("FileVersion");
    sVersionKeyArr.append("OriginalFilename");
    sVersionKeyArr.append("SpecialBuild");

    do
    {
        WCHAR wszTemp[STRING_LEN_1024] = {0};
        QStringToWCHAR(wszTemp, sAppPath);

        const WCHAR* pwszFilePath = wszTemp;//sFilePath.toStdWString().c_str();
        DWORD dwSize = GetFileVersionInfoSize(pwszFilePath, NULL);
        if(dwSize == 0)
            break;

        pBlock = malloc(dwSize);
        if(!GetFileVersionInfo(pwszFilePath, 0, dwSize, pBlock))
            break;

        char* pVerValue = NULL;
        UINT nSize = 0;
        if(!VerQueryValue(pBlock, QString("\\VarFileInfo\\Translation").toStdWString().c_str(), (LPVOID*)&pVerValue, &nSize))
            break;

        QString strTranslation;
        strTranslation.sprintf("%04x%04x", *((unsigned short int*)pVerValue), *((unsigned short int*)&pVerValue[2]));//080404b0为中文，040904E4为英文

        for(int i=0; i<sVersionKeyArr.size(); i++)
        {
            QString strSubBlock;
            strSubBlock.sprintf("\\StringFileInfo\\%s\\%s", strTranslation.toStdString().c_str(), sVersionKeyArr.at(i).toStdString().c_str());
            if(!VerQueryValue(pBlock, strSubBlock.toStdWString().c_str(), (LPVOID*)&pVerValue, &nSize))
                continue;

            QString sTemp = QString::fromWCharArray((WCHAR*)pVerValue);
            //lstrcpy(sTemp.GetBufferSetLength(nSize), (WCHAR*)pVerValue);
            //sTemp.ReleaseBuffer();

            switch(i)
            {
            case eVersionKeyEnumFileDescription:
                fileinfo.sFileDesc = sTemp;
                break;
            case eVersionKeyEnumInternalName:
                fileinfo.sInternalName = sTemp;
                break;
            case eVersionKeyEnumLegalCopyright:
                fileinfo.sLegalCopyright = sTemp;
                break;
            case eVersionKeyEnumOriginalFilename:
                fileinfo.sOriginName = sTemp;
                break;
            case eVersionKeyEnumProductName:
                fileinfo.sProductName = sTemp;
                break;
            case eVersionKeyEnumProductVersion:
                fileinfo.sProductVer = sTemp;
                break;
            case eVersionKeyEnumCompanyName:
                fileinfo.sCompanyName = sTemp;
                break;
            case eVersionKeyEnumFileVersion:
                fileinfo.sFileVer = sTemp;
                break;
            //...添加其他属性信息

            }
        }

        bRet = TRUE;
    } while (0);

    if(pBlock)
        free(pBlock);
#else
    do
    {
        QString sInfoPath = sAppPath;
        sInfoPath = sInfoPath.left(sInfoPath.lastIndexOf('/')) + "/../Info.plist";
        if (!PathFileExists(sInfoPath))
        {
            break;
        }

        QFile xmlFile(sInfoPath);
        if (!xmlFile.open(QIODevice::ReadOnly))
        {
            break;
        }

        QTextStream readStream(&xmlFile);
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        readStream.setCodec(codec);
        QString sXmlDataStr = readStream.readAll();
        xmlFile.close();

        QDomDocument xmlDoc;
        if (!xmlDoc.setContent(sXmlDataStr))
        {
            break;
        }
        QDomElement xmlRoot = xmlDoc.documentElement();
        if (QString::compare(xmlRoot.tagName(), "plist", Qt::CaseInsensitive) != 0)
        {
            break;
        }

        QDomNode dictNode = xmlRoot.namedItem("dict");
        if (dictNode.isNull())
        {
            break;
        }
        QDomElement dictElement = dictNode.toElement();

        QDomNode childNode = dictElement.firstChild();
        while(!childNode.isNull())
        {
            //QDomElement childElement = childNode.toElement();
            QString sChildTagName = childNode.toElement().tagName();
            QString sChildText = childNode.toElement().text();

            if(QString::compare(sChildTagName, "key", Qt::CaseInsensitive) == 0)
            {
                if(QString::compare(sChildText, "CFBundleVersion", Qt::CaseInsensitive) == 0)
                {
                    childNode = childNode.nextSibling();
                    fileverinfo.sFileVer = childNode.toElement().text();
                }
                else if(QString::compare(sChildText, "CFBundleGetInfoString", Qt::CaseInsensitive) == 0)
                {
                    childNode = childNode.nextSibling();
                    fileverinfo.sLegalCopyright = childNode.toElement().text();
                }
            }

            childNode = childNode.nextSibling();
        }

        bRet = true;

    }while(0);
#endif
    return bRet;
}
