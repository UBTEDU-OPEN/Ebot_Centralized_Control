#ifndef UBTMYPLAYWIDGET_H
#define UBTMYPLAYWIDGET_H

#include "../UBXRobotController/ubxdataprocessor.h"
#include "ubtsendfilethread.h"
#include "ubtmyplay_global.h"
#include "ubtsemaphore.h"
#include "UBXCritSec.h"

#include <QWidget>
#include <QListWidgetItem>
#include <QMap>

#define ESTABLISH_TCP_SERVER

namespace Ui {
class UBTMyPlayWidget;
}

class QTimer;
class QStandardItemModel;

#ifdef ESTABLISH_TCP_SERVER
class QTcpServer;
#endif

//蓝牙发送数据结构
typedef struct _CONTROL_DATA
{
    int                     nCmd;   // 命令
    byte*                   pData;  // 数据指针
    int                     nLen;   // 数据长度
    _CONTROL_DATA()
    {
        nCmd = 0;
        pData = NULL;
        nLen = 0;
    }
}CONTROL_DATA,*PCONTROL_DATA;

typedef vector<PCONTROL_DATA> vectorControlData;  //蓝牙数据列表


const QString MYPLAY_WIDGET_DEFAULT_QSS_PATH = ":/res/qss/myPlay.qss";

class CSerialPort;

class UBTMYPLAYSHARED_EXPORT UBTMyPlayWidget : public QWidget
{
    Q_OBJECT

public:
    enum PlayOrder {
        Random,
        Loop,
        Single,
        PlayOrderCount
    };

public:
    explicit UBTMyPlayWidget(QWidget *parent = 0);
    ~UBTMyPlayWidget();

    /**************************************************************************
    * 函数名:InitWindow
    * 功能:初始化窗口
    * 参数:
    *    @[in ]
    * 返回值: void
    * 时间: 2018/03/19
    * 作者: Joker
    */
    void InitWindow();

    void InitActionList();

    QString getCurActionName();

    QString getActionFileNameByRow(int nRow);

    //设置窗体默认Style sheet样式
    void SetDefaultStyle();

//    QString getLastMusicPath();

private slots:
    void onSynchronizeClicked();

    void onPlayClicked();

    void onResetClicked();

    void onEmptyListClicked();

    void onTestClicked();

    void onDeleteFile();

    void openActFileDirectory();

//    void on_pBPause_clicked();

//    void on_pBResume_clicked();

    void onPowerOffClicked();

    void onRestClicked();

    void onWakeUpClicked();

    void on_comboBoxComListFre_currentIndexChanged(int index);

//    void on_pBSetFrequency_clicked();

    void onAddActionClicked();

    void onSynChronizeFinished();

    void on_pBPre_clicked();

    void on_pBNext_clicked();

    void onListWidgetActionItemPressed(const QModelIndex &index);

    void onPlayOrderBtnClicked();

#ifdef ESTABLISH_TCP_SERVER
    void onTcpNewConnectionComming();
    void updateTcpCommands();
    int getTcpServerPort();
#endif

    void onBatteryCapacityBtnClicked();

    void onPlayTimerTimeout();

public:
    /**************************************************************************
    * 函数名:EnableControlWindow
    * 功能:窗口使能
    * 参数:
    *    @[in ]
    * 返回值: void
    * 时间: 2018/03/19
    * 作者: Joker
    */
    void EnableControlWindow(bool bEnable);

    /**************************************************************************
    * 函数名: SetWindowByConnectState
    * 功能:根据连接状态更新窗体
    * 参数:
    *    @[in ]
    * 返回值: void
    * 时间: 2018/03/19
    * 作者: Joker
    */
    void SetWindowByConnectState(bool bConnect);

    void createPopMenuActions();

    void UpdateUIStatusAsListChanged();

protected:
    static DWORD __stdcall ThreadSend(LPVOID lpParam);
    /**************************************************************************
    * 函数名: clearControlDataBuffer
    * 功能: 清空集控发送数据
    * 参数: void
    * 返回值: void
    * 时间: 2018/05/18
    * 作者: Joker
    */
    void clearControlDataBuffer();

    /**************************************************************************
    * 函数名: addToControlDataBuffer
    * 功能: 添加到集控发送队列
    * 参数: nCmd － 命令类型 pData－数据指针 nLen－数据长度
    * 返回值: 成功返回设备路径，否则返回NULLSTR
    * 时间: 2018/08/18
    * 作者: Joker
    */
    void addToControlDataBuffer(int nCmd, byte *pData, int nLen);


    /**************************************************************************
    * 函数名: getControlDataFromBuffer
    * 功能: 从集控发送队列里获取数据
    * 参数: void
    * 返回值: PCONTROL_DATA 数据
    * 时间: 2018/08/18
    * 作者: Joker
    */
    PCONTROL_DATA getControlDataFromBuffer();

    void playAndStartPlayTimer(int itemRow);
    void stopPlayAndReset();
    void stopPlayTimerAndResetUI();

    void updatePlayOrderBtnProperty();

signals:
    void sigEnableWindow(bool bEnable);

private slots:
    /**************************************************************************
    * 函数名: slotEnableWindow
    * 功能: 启用窗体信号槽
    * 参数:
    *    @[in ]
    * 返回值: void
    * 时间: 2018/03/19
    * 作者: Joker
    */
    void slotEnableWindow(bool bEnable);

private:
    UBXDataProcessor  m_objDataProcessor;  //数据处理器
    int     m_nChannel;
    QString m_strFileName;
    UBTSendFileThread* m_pSendFileThread;
    HANDLE        m_hThreadRecv; //接收线程句柄
    vectorControlData      m_vecControlDataBuffer;         // 集控发送队列
    UBTSemaphore*       m_pWriteSemp;         // 线程等待信号量
    UBXCritSec     m_hControlDataMutexWrite;  //线程同步锁

private:
    Ui::UBTMyPlayWidget *ui;
    bool m_bConnect;

    QTimer *playTimer;
    PlayOrder playOrder;
    int playingItemRow;
    int nextPlayItemRow;

    QStandardItemModel *actionListModel;

#ifdef ESTABLISH_TCP_SERVER
    QTcpServer *tcpServer;
    QMap<QString, QString> command2hts;
#endif
};

#endif // UBTMYPLAYWIDGET_H
