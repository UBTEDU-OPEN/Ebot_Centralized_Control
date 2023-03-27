#ifndef UBXPUBLIC_H
#define UBXPUBLIC_H

#include <vector>
#include <set>
#include <map>
using namespace std;

#include <QString>
#include <QUuid>
#include <QMetaType>
#include <QList>
#include <QEvent>

#ifdef USEXML
#include <QDomDocument>
#include <QDomElement>
#endif


#define UBX_MEMCPY_INC(t, s) memcpy(&t, s, sizeof(t));s+=sizeof(t);

#define SAFE_DELETE(p)       if(p){ delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p){ delete [] p; p = NULL;}



#ifndef Q_OS_WIN
typedef unsigned short   WCHAR;
#endif

typedef unsigned char    byte;



////////////////////////////////////////////////////////////////////////////////////////
/*****************************/
//全局变量定义BEGIN
/*****************************/

#include "../zxLib/zxLib.h"
using namespace zxLib;

//extern zxThreadPool g_threadpool; //全局线程池

#define ALPHA_APP_TYPE 1  //客户端类型
#define MAX_DESCRIPTION_SIZE 30   //分组描述字符数

#define RECOMMEND_TYPE 3 // recommend类型


extern QString    g_sActPath;                       //动作库路径
extern QString    g_sDownloadedActInfoXMLPath;      //已下载动作列表配置文件路径
extern QString    g_sDownloadingActInfoXMLPath;     //正在下载动作列表配置文件路径

extern QString      g_sServer;  //服务器地址
extern QString      g_sUpdateURL;  //更新地址

//动作类型前缀
//extern const QString g_sActTypePrefix[];


extern const QString g_sIsRecordLogAppName;    //是否记录日志节名
extern const QString g_sIsRecordLogKeyName;    //是否记录日志键名
extern bool          g_bIsRecordLogValue;      //是否记录日志键值

#if 1
extern int           g_nBigFrameDelay;    //大帧延时，单位毫秒
extern int           g_nSmallFrameDelay;  //小帧延时，单位毫秒
#endif

/*****************************/
//全局变量定义END
/*****************************/

// 动作二进制文件扩展名
#define     HTS_FILE_EXT      ".hts"
// 音乐文件扩展名
#define     MUSIC_FILE_EXT   ".mp3"

#define  QUAZIP_ENABLE

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*****************************/
//全局函数定义BEGIN
/*****************************/
int AddVectorData(vector<char>* pVerData, char* pData, int nLen);
void ModifyVertorData(vector<char>& pVerData, int nPos, char* pData, int nLen);
int CopyVertorToPointer(vector<char>& pVerData, char* pData);

/**************************************************************************
* 函数名: UnicodeToAnsi
* 功能: Unicode字符串转换为Ansi字符串
* 参数:
*    @[in ] sUnicodeStr: Unicode字符串
*    @[out] pOutStr: Ansi字符串，需在外释放
* 返回值: Ansi字符串长度
* 时间: 2015/12/25 17:38
* 作者: ZDJ
*/
int UnicodeToAnsi(const QString& sUnicodeStr, char*& pOutStr);

/**************************************************************************
* 函数名: UnicodeToUtf8
* 功能: Unicode字符串转换为Utf8字符串
* 参数:
*    @[in ] sUnicodeStr: Unicode字符串
*    @[out] pOutStr: Utf8字符串，需在外释放
* 返回值: Utf8字符串长度
* 时间: 2015/12/25 17:38
* 作者: ZDJ
*/
int UnicodeToUtf8(const QString& sUnicodeStr, char*& pOutStr);

/**************************************************************************
* 函数名: AnsiToUnicode
* 功能: Ansi字符串转换为Unicode字符串
* 参数:
*    @[in ] pszAnsiStr: Ansi字符串
* 返回值: Unicode字符串
* 时间: 2015/12/25 17:38
* 作者: ZDJ
*/
QString AnsiToUnicode(const char* pszAnsiStr);

/**************************************************************************
* 函数名: AnsiToUtf8
* 功能: Ansi字符串转换为Utf8字符串
* 参数:
*    @[in ] pszAnsiStr: Ansi字符串
*    @[out] pOutStr: Utf8字符串，需在外释放
* 返回值: Utf8字符串长度
* 时间: 2015/12/25 17:38
* 作者: ZDJ
*/
int AnsiToUtf8(const char* pszAnsiStr, char*& pOutStr);

/**************************************************************************
* 函数名: Utf8ToAnsi
* 功能: Utf8字符串转换为Ansi字符串
* 参数:
*    @[in ] pszUtf8Str: Utf8字符串
*    @[out] pOutStr: Ansi字符串，需在外释放
* 返回值: Ansi字符串长度
* 时间: 2015/12/25 17:38
* 作者: ZDJ
*/
int Utf8ToAnsi(const char* pszUtf8Str, char*& pOutStr);

/**************************************************************************
* 函数名: Utf8ToAnsiEx
* 功能: Utf8字符串转换为Ansi字符串
* 参数:
*    @[in ] pszUtf8Str: Utf8字符串
*    @[in ] nUtf8Len: Utf8数据长度
* 返回值: Ansi字符串
* 时间: 2015/12/25 17:38
* 作者: ZDJ
*/
string Utf8ToAnsiEx(const byte *pszUtf8Str, int nUtf8Len);

/**************************************************************************
* 函数名: Utf8ToUnicode
* 功能: Utf8字符串转换为Unicode字符串
* 参数:
*    @[in ] pszUtf8Str: Utf8字符串
* 返回值: Unicode字符串长度
* 时间: 2015/12/25 17:38
* 作者: ZDJ
*/
QString Utf8ToUnicode(const char* pszUtf8Str);

/**************************************************************************
* 函数名: AnsiToWChar
* 功能: Ansi字符串转换为WChar字符串
* 参数:
*    @[in ] pszAnsiStr: Ansi字符串
*    @[in ] nAnsiLen: Ansi字符串字节长度
*    @[out] pWCharStr: WChar字符串
* 返回值: WChar字符串字节长度
* 时间: 2015/12/25 17:38
* 作者: ZDJ
*/
int AnsiToWChar(const char* pszAnsiStr, int nAnsiLen, WCHAR* pWCharStr);

/**************************************************************************
* 函数名: WCharToAnsi
* 功能: WChar字符串转换为Ansi字符串
* 参数:
*    @[in ] pWCharStr: WChar字符串
*    @[in ] nWCharLen: WChar字符串字节长度
*    @[out] pszAnsiStr: Ansi字符串
* 返回值: Ansi字符串字节长度
* 时间: 2015/12/25 17:38
* 作者: ZDJ
*/
int WCharToAnsi(const WCHAR* pWCharStr, int nWCharLen, char* pszAnsiStr);

/**************************************************************************
* 函数名: PathFileExists
* 功能: 判断路径是否存在
* 参数:
*    @[in ] sPath: 路径
* 返回值: 存在返回true，失败返回false
* 时间: 2015/12/25 17:38
* 作者: ZDJ
*/
bool PathFileExists(const QString& sPath);

/**************************************************************************
* 函数名: GetFormatSizeString
* 功能: 获取格式化后的大小字符串
* 参数:
*    @[in ] nSize: 大小
* 返回值: 格式化后的大小字符串
* 时间: 2015/12/25 17:38
* 作者: ZDJ
*/
QString GetFormatSizeString(qint64 nSize);

/**************************************************************************
* 函数名: WCSLen
* 功能: 获取WCHAR字符串元素长度
* 参数:
*    @[in ] pWchar: WCHAR字符串
* 返回值: 字符串元素长度
* 时间: 2015/12/25 17:38
* 作者: ZZP
*/
int  WCSLen(WCHAR * pWchar);

/**************************************************************************
* 函数名: QStringToWCHAR
* 功能: QString字符串转换为WCHAR字符串
* 参数:
*    @[out] pWchar: WCHAR字符串
*    @[in ] str: QString字符串
* 返回值: WCHAR字符串长度
* 时间: 2015/12/25 17:38
* 作者: ZZP
*/
int  QStringToWCHAR(OUT WCHAR* pWchar, IN const QString& str);

/**************************************************************************
* 函数名: WCHARToQString
* 功能: WCHAR字符串转换为QString字符串
* 参数:
*    @[in ] pwchar: WCHAR字符串
*    @[in ] nbyteLen: WCHAR字符串长度
* 返回值: QString字符串
* 时间: 2015/12/25 17:38
* 作者: ZZP
*/
QString  WCHARToQString(IN WCHAR* pwchar, IN int nbyteLen);

//四舍五入函数，nDigits为四舍五入的位数，如nDigits为2 那么Round(2.378)为2.38
float Round(float fValue , int nDigits);

//时间字符串定制函数
QString TimeStringCustomise(int nHour , int nMinute ,int nSec);


/**************************************************************************
* 函数名: QtPostMessage
* 功能: 发送消息
* 参数:
*    @[in ] pDest: 目标窗口
*    @[in ] dwMsg: 消息ID
*    @[in ] dwContext: 消息上下文
*    @[in ] wParam: 消息数据长度
*    @[in ] lParam: 消息数据
* 返回值: void
* 时间: 2016/02/01
* 作者: ZDJ
*/
void QtPostMessage(QObject* pDest, DWORD dwMsg, DWORD dwContext, WPARAM wParam, LPARAM lParam);


/**************************************************************************
* 函数名: UnZipFile
* 功能: 解压zip文件
* 参数:
*    @[in ] sZipPath: zip文件路径
*    @[in ] pszDestDir: 解压文件夹路径，为NULL时解压到当前文件夹
* 返回值: 成功返回true，失败返回false
* 时间: 2016/03/10 16:59
* 作者: ZDJ
*/
bool UnZipFile(const QString& sZipPath, const char* pszDestDir = NULL);

/**************************************************************************
* 函数名: ZipFile
* 功能: 压缩为zip文件
* 参数:
*    @[in ] vecFilePathList: 待压缩文件路径列表
*    @[in ] sZipPath: zip文件路径
* 返回值: 成功返回true，失败返回false
* 时间: 2016/03/10 16:59
* 作者: ZDJ
*/
bool ZipFile(const vector<QString>& vecFilePathList, const QString& sZipPath);


#ifdef Q_OS_WIN
/**************************************************************************
* 函数名: delayUsec
* 功能: 微秒级延时
* 参数:
*    @[in ] uDelay: 延时时长，单位微秒
* 返回值: void
* 时间: 2016/06/06
* 作者: ZDJ
*/
void delayUsec(int uDelay);
#endif

#ifdef USEXML
/**************************************************************************
* 函数名: ReadModelXml
* 功能: 读取3D模型的xml配置文件
* 参数:
*    @[in ] element: dom元素
*    @[out] motorlist: 舵机数据列表
* 返回值: void
* 时间: 2016/01/26 10:15
* 作者: ZDJ
*/
void ReadModelXml(const QDomElement& element, vector<MOTOR_DATA*>& motorlist);

/**************************************************************************
* 函数名: ReadModelXml
* 功能: 读取3D模型的xml配置文件
* 参数:
*    @[in ] sXmlFilePath: xml文件路径
*    @[out] motorlist: 舵机数据列表
* 返回值: void
* 时间: 2016/01/26 10:15
* 作者: ZDJ
*/
void ReadModelXml(const QString& sXmlFilePath, vector<MOTOR_DATA*>& motorlist);

/**************************************************************************
* 函数名: getMotorCount
* 功能: 获取舵机数量
* 时间: 2016/06/23
* 作者: ZDJ
*/
int getMotorCount();

#endif

/*****************************/
//全局函数定义END
/*****************************/

/**************************************************************************
* 函数名: getFormatSizeString
* 功能: 获取格式化后的大小字符串
* 参数:
*    @[in ] nSize: 大小
* 返回值: 格式化后的大小字符串
* 时间: 2015/12/25 17:38
* 作者: ZDJ
*/
QString getFormatSizeString(qint64 nSize);

/**************************************************************************
* FunctionName:
*     ReleaseVector
* Description:
*     销毁vector指针列表
* Parameter:
*     @[in out] vec: vector列表
* ReturnValue:
*     void
*/
template<class T>
void ReleaseVector(vector<T>& vec)
{
    vector<T>::iterator iter;
    if (vec.size() > 0)
    {
        for(iter=vec.begin();iter!=vec.end();)
        {
            delete *iter;
            iter = vec.erase(iter);
        }
        vector<T>().swap(vec);
    }
}

#endif

