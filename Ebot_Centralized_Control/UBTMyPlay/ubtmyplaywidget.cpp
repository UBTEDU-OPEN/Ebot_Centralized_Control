#include "ubtmyplaywidget.h"
#include "ui_ubtmyplaywidget.h"
#include "../UBTUtil/filedirhandle.h"
#include "../UBXRobotController/Robot/SerialPort.h"
#include "../common/UBXPublic.h"
#include "../UBTUtil/cstringhandle.h"
#include "../UBTUtil/configs.h"
#include "ubtactionlistitemdelegate.h"
#include "../UBTWidget/ubtinfodlg.h"
#include "../UBTWidget/ubtconfirmdlg.h"

#include <QTimer>
#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QDir>

#ifdef ESTABLISH_TCP_SERVER
#include <QSettings>
#endif

//#include <QMessageBox>

UBTMyPlayWidget::UBTMyPlayWidget(QWidget *parent)
    : QWidget(parent)
    , playOrder(Loop)
    , ui(new Ui::UBTMyPlayWidget)
    , m_strFileName(NULLSTR)
    , m_pSendFileThread(NULL)
    , m_pWriteSemp(NULL)
    , m_bConnect(false)
    , playingItemRow(-1)
    , nextPlayItemRow(-1)
    , actionListModel(nullptr)
#ifdef ESTABLISH_TCP_SERVER
    , tcpServer(nullptr)
#endif
{
    actionListModel = new QStandardItemModel(this);

    ui->setupUi(this);
    ui->FrequencyWidget->hide();
    m_vecControlDataBuffer.clear();
    m_pWriteSemp = new UBTSemaphore("ControlWrite");
    InitWindow();

    playTimer = new QTimer(this);
    playTimer->setSingleShot(true);
    connect(playTimer, &QTimer::timeout, this, &UBTMyPlayWidget::onPlayTimerTimeout);

#ifdef ESTABLISH_TCP_SERVER
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any, getTcpServerPort());
    connect(tcpServer, &QTcpServer::newConnection, this, &UBTMyPlayWidget::onTcpNewConnectionComming);
#endif
}

UBTMyPlayWidget::~UBTMyPlayWidget()
{
    delete ui;
    SAFE_DELETE(m_pSendFileThread);
    SAFE_DELETE(m_pWriteSemp);
    clearControlDataBuffer();
}


/**************************************************************************
* 函数名:InitWindow
* 功能:初始化窗口
* 参数:
*    @[in ]
* 返回值: void
* 时间: 2018/03/19
* 作者: Joker
*/
void UBTMyPlayWidget::InitWindow()
{
    for (int i = 0; i < UBX_CHANNEL_COUNT; i++)
    {
        QString strText = NULLSTR;
        int nChannel = i + 1;
        strText.sprintf("%d", nChannel);
        ui->comboBoxComListFre->addItem(strText);
    }
    ui->comboBoxComListFre->setCurrentIndex(0);
    m_nChannel = 1;
    updatePlayOrderBtnProperty();
    ui->actionListView->setAlternatingRowColors(true);
    ui->actionListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    UBTActionListItemDelegate *actionListItemDelegate = new UBTActionListItemDelegate;
    ui->actionListView->setModel(actionListModel);
    ui->actionListView->setItemDelegate(actionListItemDelegate);
    connect(ui->actionListView, &QListView::pressed, this, &UBTMyPlayWidget::onListWidgetActionItemPressed);

    InitActionList();
    SetDefaultStyle();

    m_pSendFileThread = new UBTSendFileThread();
    connect(m_pSendFileThread, &QThread::finished, this, &UBTMyPlayWidget::onSynChronizeFinished);
    connect(ui->pBWakeUp, &QPushButton::clicked, this, &UBTMyPlayWidget::onWakeUpClicked);
    connect(ui->pBTest, &QPushButton::clicked, this, &UBTMyPlayWidget::onTestClicked);
    connect(ui->pBRest, &QPushButton::clicked, this, &UBTMyPlayWidget::onRestClicked);
    connect(ui->pBPowerOff, &QPushButton::clicked, this, &UBTMyPlayWidget::onPowerOffClicked);
    connect(ui->pBSynchronize, &QPushButton::clicked, this, &UBTMyPlayWidget::onSynchronizeClicked);
    connect(this, &UBTMyPlayWidget::sigEnableWindow, this, &UBTMyPlayWidget::slotEnableWindow);
    connect(ui->pBEmptyList, &QPushButton::clicked, this, &UBTMyPlayWidget::onEmptyListClicked);
    connect(ui->pBAddAction, &QPushButton::clicked, this, &UBTMyPlayWidget::onAddActionClicked);
    connect(ui->playOrderBtn, &QPushButton::clicked, this, &UBTMyPlayWidget::onPlayOrderBtnClicked);
    connect(ui->pBPlay, &QPushButton::clicked, this, &UBTMyPlayWidget::onPlayClicked);
    connect(ui->pBReset, &QPushButton::clicked, this, &UBTMyPlayWidget::onResetClicked);
    connect(ui->batteryCapacityBtn, &QPushButton::clicked, this, &UBTMyPlayWidget::onBatteryCapacityBtnClicked);
    EnableControlWindow(false);
    SetWindowByConnectState(false);
    m_hThreadRecv = CreateThread(NULL, 0, ThreadSend, this, 0, NULL);
}

void UBTMyPlayWidget::InitActionList()
{
//    ui->listWidgetAction->clear();
    actionListModel->clear();
    QString strActionPath;
    if (!CConfigs::instance()->getLocalActLibDir(strActionPath)) {
        qDebug() << "UBTMyPlayWidget::InitActionList invalid act lib dir";
        return;
    }
    QDir dir(strActionPath);
    //设置过滤器(目录，文件或非上级目录)
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);
    //取得目录中文件列表(包含目录)
    QFileInfoList list = dir.entryInfoList();
    if (list.size() > 0) {
        int i=0;
        do {
            QFileInfo fileInfo = list.at(i);
            QString strFileName = fileInfo.fileName();
            int pos = strFileName.indexOf('.');
            QString strExtName = strFileName.right(strFileName.length() - pos);
            if (strExtName.compare(HTS_FILE_EXT, Qt::CaseInsensitive) != 0) {
                i++;
                continue;
            }
            QString fullPathName = fileInfo.absoluteFilePath();
            int actionDuration = CUtil::getActTotalTime(fullPathName);
            actionDuration += 4000;

            QStandardItem *item = new QStandardItem;
            item->setData(strFileName.left(pos), Qt::DisplayRole);
            item->setData(false, UBTActionListItemDelegate::PlayingFlagRole);
            item->setData(actionDuration / 1000, UBTActionListItemDelegate::ActionDurationRole);
            actionListModel->appendRow(item);

            i++;
        } while (i<list.size());
    }
    auto curIdx = ui->actionListView->model()->index(0, 0);
    ui->actionListView->setCurrentIndex(curIdx);
    UpdateUIStatusAsListChanged();
}

QString UBTMyPlayWidget::getCurActionName()
{
    QString strFileName = NULLSTR;
    auto index = ui->actionListView->currentIndex();
    if (index.isValid()) {
        strFileName = index.data(Qt::DisplayRole).toString();
    }
    return strFileName;
}

QString UBTMyPlayWidget::getActionFileNameByRow(int nRow)
{
    QString strFileName = NULLSTR;
    auto index = ui->actionListView->model()->index(nRow, 0);
    if (index.isValid()) {
        strFileName = index.data(Qt::DisplayRole).toString() + HTS_FILE_EXT;
    }
    return strFileName;
}

void UBTMyPlayWidget::SetDefaultStyle()
{
    QFile styleSheet(MYPLAY_WIDGET_DEFAULT_QSS_PATH);
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        setStyleSheet(styleSheet.readAll());
    }
}
#if 0
void UBTMyPlayWidget::run()
{


}
#endif
void UBTMyPlayWidget::onSynchronizeClicked()
{
    EnableControlWindow(false);
    stopPlayTimerAndResetUI();
    bool bOk = false;
    do {
        QString actLibDir;
        if (!CConfigs::instance()->getLocalActLibDir(actLibDir)) {
            qDebug() << "UBTMyPlayWidget::onSynchronizeClicked invalid act lib dir";
            break;
        }
        QString strText = getCurActionName();
        if (strText.isEmpty()) {
            break;
        }
        QString strFileName = actLibDir + QDir::toNativeSeparators("/") + strText + HTS_FILE_EXT;
        m_strFileName = strFileName;
        if (m_pSendFileThread)
        {
            bOk = true;
            m_pSendFileThread->SetCurFileName(strFileName);
            m_pSendFileThread->quit();
            m_pSendFileThread->start();
        }

    }while(0);
    if (!bOk)
    {
        UBTInfoDlg *dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("Sync.failed"), this);
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
        dlg->show();
        EnableControlWindow(true);
    }
}

void UBTMyPlayWidget::onPlayClicked()
{
    qDebug() << "UBTMyPlayWidget::onPlayClicked";
    EnableControlWindow(false);
    stopPlayTimerAndResetUI();
    playAndStartPlayTimer(ui->actionListView->currentIndex().row());
//    EnableControlWindow(true);
}

void UBTMyPlayWidget::onResetClicked()
{
    EnableControlWindow(false);
    stopPlayAndReset();
//    EnableControlWindow(true);
}

void UBTMyPlayWidget::onEmptyListClicked()
{
    EnableControlWindow(false);
    stopPlayTimerAndResetUI();
    UBTConfirmDlg *dlg = new UBTConfirmDlg("", tr("Are you sure you want to empty list?"), tr("yes"), tr("no"));
    connect(dlg, &UBTConfirmDlg::acceptClicked, this, [this, dlg]() {
        QString actLibDir;
        if (!CConfigs::instance()->getLocalActLibDir(actLibDir)) {
            qDebug() << "UBTMyPlayWidget::onEmptyListClicked invalid act lib dir";
            return;
        }
        if (ui->actionListView->model()->rowCount() == 0) {
            EnableControlWindow(true);
        }
        while (ui->actionListView->model()->rowCount() > 0) {
            QString strText = getCurActionName();
            if (strText.isEmpty()) {
                break;
            }
            QString strFileName = actLibDir + QDir::toNativeSeparators("/") + strText + HTS_FILE_EXT;
            qDebug() <<"UBTMyPlayWidget::onEmptyListClicked act File Name:" << strFileName << endl;
            QFile::remove(strFileName);
            strFileName = actLibDir + QDir::toNativeSeparators("/") + strText + MUSIC_FILE_EXT;
            qDebug() <<"UBTMyPlayWidget::onEmptyListClicked mp3 File Name:" << strFileName << endl;
            QFile::remove(strFileName);

            QString strBasename =  CFileDirHandle::getFileBaseName(strText);
            qDebug()<<"UBTMyPlayWidget::onEmptyListClicked strBasename:" <<strBasename<<endl;
            char* pBaseName = NULL;
            UnicodeToUtf8(strBasename, pBaseName);
            size_t nLen = strlen(pBaseName);
            byte* pDataTemp = new  byte[nLen+1];
            byte* pData = pDataTemp;
            memcpy(pDataTemp, &nLen,  sizeof(byte));
            pDataTemp+=sizeof(byte);
            memcpy(pDataTemp, pBaseName, nLen);

            addToControlDataBuffer(UBXPROTOCOL_CMDID_DELETE_FILE, (byte*)pData, nLen + 1);
            auto curIdx = ui->actionListView->currentIndex();
            ui->actionListView->model()->removeRow(curIdx.row());
            SAFE_DELETE_ARRAY(pData);
        }
        InitActionList();
        ui->myPerformanceFileListContainer->setCurrentIndex(1);
        dlg->close();
    });
    connect(dlg, &UBTConfirmDlg::rejectClicked, this, [this, dlg]() {
        EnableControlWindow(true);
        dlg->close();
    });
    dlg->showInCenter();
//    EnableControlWindow(true);
}

void UBTMyPlayWidget::onTestClicked()
{
    stopPlayTimerAndResetUI();
    EnableControlWindow(false);
    addToControlDataBuffer(UBXPROTOCOL_CMDID_PLAY_TEST, NULL, 1);
//    EnableControlWindow(true);
}

void UBTMyPlayWidget::onDeleteFile()
{
    EnableControlWindow(false);

    QString strText = getCurActionName();
    if (strText.isEmpty()) {
        return;
    }
    QString actLibDir;
    if (!CConfigs::instance()->getLocalActLibDir(actLibDir)) {
        qDebug() << "UBTMyPlayWidget::onDeleteFile invalid act lib dir";
        return;
    }
    UBTConfirmDlg *dlg = new UBTConfirmDlg("", tr("Are you sure you want to delete the selected file?"), tr("yes"), tr("no"));
    connect(dlg, &UBTConfirmDlg::acceptClicked, this, [this, dlg, actLibDir, strText]() {
        QString strFileName = actLibDir + QDir::toNativeSeparators("/") + strText + HTS_FILE_EXT;
        qDebug()<<"UBTMyPlayWidget::onSynchronizeClicked act File Name:" <<strFileName<<endl;
        QFile::remove(strFileName);
        strFileName = actLibDir + QDir::toNativeSeparators("/") + strText + MUSIC_FILE_EXT;
        qDebug() << "UBTMyPlayWidget::onSynchronizeClicked mp3 File Name:" << strFileName << endl;
        QFile::remove(strFileName);

        InitActionList();

        QString strBasename =  CFileDirHandle::getFileBaseName(strText);
        qDebug()<<"UBTMyPlayWidget::onSynchronizeClicked strBasename:" <<strBasename<<endl;
        char* pBaseName = NULL;
        UnicodeToUtf8(strBasename, pBaseName);
        size_t nLen = strlen(pBaseName);
        byte* pDataTemp = new  byte[nLen+1];
        byte* pData = pDataTemp;
        memcpy(pDataTemp, &nLen,  sizeof(byte));
        pDataTemp+=sizeof(byte);
        memcpy(pDataTemp, pBaseName, nLen);

    //     m_objDataProcessor.Req_SendData(m_pPort, UBXPROTOCOL_CMDID_STOP, NULL, 1);

        addToControlDataBuffer(UBXPROTOCOL_CMDID_DELETE_FILE, (byte*)pData, nLen + 1);
        SAFE_DELETE_ARRAY(pData);

        dlg->close();
    });
    connect(dlg, &UBTConfirmDlg::rejectClicked, this, [this, dlg]() {
        EnableControlWindow(true);
        dlg->close();
    });
    dlg->showInCenter();

    //    EnableControlWindow(true);
}

void UBTMyPlayWidget::openActFileDirectory()
{
    QString actLibDir;
    if (!CConfigs::instance()->getLocalActLibDir(actLibDir)) {
        qDebug() << "UBTMyPlayWidget::openActFileDirectory invalid act lib dir";
        return;
    }
//    QMessageBox::information(this, "", "path" + actLibDir);
    QFileInfo dir(actLibDir);
    QDesktopServices::openUrl(QUrl::fromLocalFile(dir.absoluteFilePath()));
}

//void UBTMyPlayWidget::on_pBPause_clicked()
//{
//    EnableControlWindow(false);
//    m_objDataProcessor.Req_SendData(m_pPort, UBXPROTOCOL_CMDID_PAUSE, NULL, 1);
//    EnableControlWindow(true);
//}

//void UBTMyPlayWidget::on_pBResume_clicked()
//{
//    EnableControlWindow(false);
//    m_objDataProcessor.Req_SendData(m_pPort, UBXPROTOCOL_CMDID_RESUME, NULL, 1);
//    EnableControlWindow(true);
//}

void UBTMyPlayWidget::onPowerOffClicked()
{
    stopPlayTimerAndResetUI();
    EnableControlWindow(false);
    addToControlDataBuffer(UBXPROTOCOL_CMDID_POWEROFF, NULL, 1);
}

void UBTMyPlayWidget::onRestClicked()
{
    stopPlayTimerAndResetUI();
    EnableControlWindow(false);
    addToControlDataBuffer(UBXPROTOCOL_CMDID_REST, NULL, 1);
    auto playingItemIndex = ui->actionListView->model()->index(playingItemRow, 0);
    if (playingItemIndex.isValid()) {
        ui->actionListView->model()->setData(playingItemIndex, false, UBTActionListItemDelegate::PlayingFlagRole);
    }
}

void UBTMyPlayWidget::onWakeUpClicked()
{
    stopPlayTimerAndResetUI();
    EnableControlWindow(false);
    addToControlDataBuffer(UBXPROTOCOL_CMDID_WAKEUP, NULL, 1);
}

void UBTMyPlayWidget::on_comboBoxComListFre_currentIndexChanged(int index)
{
    float fFrequency = (float)index + UBX_BASE_FREQUENCY;
    QString strText = NULLSTR;
    strText.sprintf("%.2f", fFrequency);
    ui->lEFrequency->setText(strText);
}

//void UBTMyPlayWidget::on_pBSetFrequency_clicked()
//{
//    EnableControlWindow(false);
//    byte data[UBX_FREQUENCY_PARAM_COUNT] = {0xAA, 0xFA, 0x03, 0x01, 0x01, 0x03
//                                           ,0x07, 0x03, 0x02, 0x01, 0x01, 0x00
//                                           ,0x00, 0x00, 0x00, 0x00, 0x00};
//    int nIndex = ui->comboBoxComListFre->currentIndex();
//    int nChannel = nIndex + 1;
//    data[UBX_FREQUENCY_PARAM_SET_INDEX] = nChannel;
//    m_objDataProcessor.Req_SendData(UBXPROTOCOL_CMDID_SET_FREQUENCY, data, UBX_FREQUENCY_PARAM_COUNT);
//    EnableControlWindow(true);
//}


/**************************************************************************
* 函数名:EnableControlWindow
* 功能:窗口使能
* 参数:
*    @[in ]
* 返回值: void
* 时间: 2018/03/19
* 作者: Joker
*/
void UBTMyPlayWidget::EnableControlWindow(bool bEnable)
{
    ui->playOrderBtn->setEnabled(bEnable);
    ui->pBPlay->setEnabled(bEnable);
    ui->pBAddAction->setEnabled(bEnable);
    ui->pBSynchronize->setEnabled(bEnable && ui->actionListView->model()->rowCount() > 0);
    ui->pBEmptyList->setEnabled(bEnable && ui->actionListView->model()->rowCount() > 0);
    ui->pBTest->setEnabled(bEnable);
//    ui->batteryCapacityBtn->setEnabled(bEnable);
    ui->batteryCapacityBtn->setEnabled(false);
    ui->pBPowerOff->setEnabled(bEnable);
    ui->pBRest->setEnabled(bEnable);
    ui->pBWakeUp->setEnabled(bEnable);
    ui->pBSetFrequency->setEnabled(bEnable);
    ui->pBReset->setEnabled(bEnable);
    ui->pBPre->setEnabled(bEnable);
    ui->pBNext->setEnabled(bEnable);
//    ui->listWidgetAction->setEnabled(bEnable);
    ui->actionListView->setEnabled(bEnable);
//    if (bEnable)
//    {
//        ui->widgetNotConnected->hide();
//        ui->listWidgetAction->show();
//    }
//    else
//    {
//        ui->widgetNotConnected->show();
//        ui->listWidgetAction->hide();
//    }
//    if (bEnable)
//    {
//        ui->labelWakeup->setPixmap(QPixmap(":/res/images/ic_wakeup.png"));
//        ui->labelRest->setPixmap(QPixmap(":/res/images/ic_sleep.png"));
//        ui->labelPowerOff->setPixmap(QPixmap(":/res/images/ic_trunoff.png"));
//        ui->labelTest->setPixmap(QPixmap(":/res/images/ic_test.png"));
//    }
//    else
//    {
//        ui->labelWakeup->setPixmap(QPixmap(":/res/images/ic_wakeup_disable.png"));
//        ui->labelRest->setPixmap(QPixmap(":/res/images/ic_sleep_disable.png"));
//        ui->labelPowerOff->setPixmap(QPixmap(":/res/images/ic_turnoff_disable.png"));
//        ui->labelTest->setPixmap(QPixmap(":/res/images/ic_test_disable.png"));
    //    }
}

/**************************************************************************
* 函数名: SetWindowByConnectState
* 功能:根据连接状态更新窗体
* 参数:
*    @[in ]
* 返回值: void
* 时间: 2018/03/19
* 作者: Joker
*/
void UBTMyPlayWidget::SetWindowByConnectState(bool bConnect)
{
    if (bConnect)
    {
//        ui->widgetNotConnected->hide();
//        ui->listWidgetAction->show();
        m_bConnect = true;
		UpdateUIStatusAsListChanged();
    }
    else
    {
        ui->myPerformanceFileListContainer->setCurrentIndex(0);
//        ui->widgetNotConnected->show();
//        ui->listWidgetAction->hide();
        m_bConnect = false;
    }
}

void UBTMyPlayWidget::onAddActionClicked()
{
    EnableControlWindow(false);
    stopPlayTimerAndResetUI();
    QString actLibDir;
    if (!CConfigs::instance()->getLocalActLibDir(actLibDir)) {
        qDebug() << "UBTMyPlayWidget::onAddActionClicked invalid act lib dir";
        return;
    }
    QString sFmtTitle = tr("Hts File"); //格式名称
    QString sFmtExt = HTS_FILE_EXT;  //格式后缀
    QString sFmt;
    QString strFilePath = QDir::currentPath();
#ifdef Q_OS_MAC
    sFmt.sprintf("%s(*%s);;%s(*%s)", sFmtTitle.toStdString().c_str(), sFmtExt.toStdString().c_str(), sFmtTitle.toStdString().c_str(), sFmtExt.toStdString().c_str());
#else
    sFmt.sprintf("%s(*%s)", sFmtTitle.toStdString().c_str(), sFmtExt.toStdString().c_str());
#endif
    QString sHtscFile = QFileDialog::getOpenFileName(NULL, tr("Choose a hts file"), strFilePath, sFmt);

    if(!sHtscFile.isEmpty())
    {
        QString strFileName =CFileDirHandle::getFileName(sHtscFile);
        QString strDesName = actLibDir + QDir::toNativeSeparators("/") + strFileName;
        QFile::copy(sHtscFile, strDesName);
        UBTInfoDlg *dlg = new UBTInfoDlg(UBTInfoDlg::NoIcon, tr("Added"), this);
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
        dlg->show();
    }
    InitActionList();
    EnableControlWindow(true);
}

void UBTMyPlayWidget::onSynChronizeFinished()
{
    if (m_bConnect) {
        UBTInfoDlg *dlg = new UBTInfoDlg(UBTInfoDlg::SuccessIcon, tr("synchronized"), this);
        dlg->show();
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
        EnableControlWindow(true);
    }
    else {
        UBTInfoDlg *dlg = new UBTInfoDlg(UBTInfoDlg::FailureIcon, tr("synchronization failed"), this);
        dlg->show();
        dlg->move((width() - dlg->width()) / 2, (height() - dlg->height()) / 2);
    }
}

//QString UBTMyPlayWidget::getLastMusicPath()
//{
//    QString strFilePath;
//    CIniConfig::getInstance().getPath(eFilePathImportMusic, strFilePath);
//    QDir dir(strFilePath);
//    if (strFilePath.isEmpty() || !dir.exists())
//    {
//        strFilePath = QDir::currentPath();
//    }

//    return strFilePath;
//}


void UBTMyPlayWidget::on_pBPre_clicked()
{
    EnableControlWindow(false);

    do {
        QString actLibDir;
        if (!CConfigs::instance()->getLocalActLibDir(actLibDir)) {
            qDebug() << "UBTMyPlayWidget::on_pBPre_clicked invalid act lib dir";
            break;
        }
//        int nIndex = ui->listWidgetAction->currentRow();
        int nIndex = ui->actionListView->currentIndex().row();

        int nCurIndex = nIndex;

        if (nIndex != 0) {
            nCurIndex = nIndex - 1;
            auto idx = ui->actionListView->model()->index(nCurIndex, 0);
            ui->actionListView->setCurrentIndex(idx);
//            ui->listWidgetAction->setCurrentRow(nCurIndex);
        }
        QString strText = getActionFileNameByRow(nCurIndex);

        if (strText.isEmpty())
        {
            break;
        }

        QString strFileName = actLibDir +QDir::toNativeSeparators("/") + strText;
        qDebug()<<"UBTMyPlayWidget::onSynchronizeClicked strFileName:" <<strFileName<<endl;

        QString strBasename =  CFileDirHandle::getFileBaseName(strFileName);
        qDebug()<<"UBTMyPlayWidget::onSynchronizeClicked strBasename:" <<strBasename<<endl;
        char* pBaseName = NULL;
        UnicodeToUtf8(strBasename, pBaseName);
        size_t nLen = strlen(pBaseName);
        byte* pDataTemp = new  byte[nLen+1];
        byte* pData = pDataTemp;
        memcpy(pDataTemp, &nLen,  sizeof(byte));
        pDataTemp+=sizeof(byte);
        memcpy(pDataTemp, pBaseName, nLen);


    //     m_objDataProcessor.Req_SendData(m_pPort, UBXPROTOCOL_CMDID_STOP, NULL, 1);
        addToControlDataBuffer(UBXPROTOCOL_CMDID_PLAY, (byte*)pData, strlen(pBaseName) + 1);

//        m_objDataProcessor.Req_SendData(UBXPROTOCOL_CMDID_PLAY, (byte*)pData, strlen(pBaseName) + 1);
        SAFE_DELETE_ARRAY(pData);
    }while(0);

}

void UBTMyPlayWidget::on_pBNext_clicked()
{
    EnableControlWindow(false);
    do
    {
        QString actLibDir;
        if (!CConfigs::instance()->getLocalActLibDir(actLibDir)) {
            qDebug() << "UBTMyPlayWidget::on_pBNext_clicked invalid act lib dir";
            break;
        }
//        int nIndex = ui->listWidgetAction->currentRow();
        int nIndex = ui->actionListView->currentIndex().row();
//        int nLastIndex = ui->listWidgetAction->count() - 1;
        int nLastIndex = ui->actionListView->model()->rowCount() - 1;
        int nCurIndex = nIndex;
        if (nIndex != nLastIndex)
        {
            nCurIndex = nIndex + 1;
            auto idx = ui->actionListView->model()->index(nCurIndex, 0);
            ui->actionListView->setCurrentIndex(idx);
//            ui->listWidgetAction->setCurrentRow(nCurIndex);
        }
        QString strText = getActionFileNameByRow(nCurIndex);

        if (strText.isEmpty())
        {
            break;
        }

        QString strFileName = actLibDir +QDir::toNativeSeparators("/") + strText;
        qDebug()<<"UBTMyPlayWidget::onSynchronizeClicked strFileName:" <<strFileName<<endl;

        QString strBasename =  CFileDirHandle::getFileBaseName(strFileName);
        qDebug()<<"UBTMyPlayWidget::onSynchronizeClicked strBasename:" <<strBasename<<endl;
        char* pBaseName = NULL;
        UnicodeToUtf8(strBasename, pBaseName);
        size_t nLen = strlen(pBaseName);
        byte* pDataTemp = new  byte[nLen+1];
        byte* pData = pDataTemp;
        memcpy(pDataTemp, &nLen,  sizeof(byte));
        pDataTemp+=sizeof(byte);
        memcpy(pDataTemp, pBaseName, nLen);


    //     m_objDataProcessor.Req_SendData(m_pPort, UBXPROTOCOL_CMDID_STOP, NULL, 1);

        addToControlDataBuffer(UBXPROTOCOL_CMDID_PLAY, (byte*)pData, strlen(pBaseName) + 1);
        SAFE_DELETE_ARRAY(pData);
    }while(0);
}


void UBTMyPlayWidget::onListWidgetActionItemPressed(const QModelIndex &index)
{
    if (qApp->mouseButtons() == Qt::RightButton) { //右键
        if(index.isValid()) {
            createPopMenuActions();
        }
    }
}

void UBTMyPlayWidget::onPlayOrderBtnClicked()
{
    EnableControlWindow(false);
    stopPlayTimerAndResetUI();
    auto playingItemIndex = ui->actionListView->model()->index(playingItemRow, 0);
    if (playingItemIndex.isValid()) {
        ui->actionListView->model()->setData(playingItemIndex, false, UBTActionListItemDelegate::PlayingFlagRole);
    }
    // change play order
    playOrder = static_cast<PlayOrder>((playOrder + 1) % PlayOrderCount);
    updatePlayOrderBtnProperty();
    EnableControlWindow(true);
}

#ifdef ESTABLISH_TCP_SERVER
void UBTMyPlayWidget::onTcpNewConnectionComming()
{
    qDebug() << "UBTMyPlayWidget::onTcpNewConnectionComming";
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);
    connect(clientConnection, &QAbstractSocket::readyRead, this, [clientConnection, this]() {
        QString command = QString(clientConnection->readAll());
        qDebug() << "\t+++command:" << command;
        updateTcpCommands();
        if (command2hts.contains(command) && m_bConnect) {
            QString actLibDir;
            if (!CConfigs::instance()->getLocalActLibDir(actLibDir)) {
                qDebug() << "\t+++invalid act lib dir";
                return;
            }
            QString strFileName = actLibDir + QDir::toNativeSeparators("/") + command2hts[command];
            qDebug() << "\t+++hts:" << strFileName;
            QString strBasename =  CFileDirHandle::getFileBaseName(strFileName);
            char* pBaseName = NULL;
            UnicodeToUtf8(strBasename, pBaseName);
            size_t nLen = strlen(pBaseName);
            byte* pDataTemp = new  byte[nLen+1];
            byte* pData = pDataTemp;
            memcpy(pDataTemp, &nLen,  sizeof(byte));
            pDataTemp += sizeof(byte);
            memcpy(pDataTemp, pBaseName, nLen);
            addToControlDataBuffer(UBXPROTOCOL_CMDID_PLAY, (byte*)pData, strlen(pBaseName) + 1);
            SAFE_DELETE_ARRAY(pBaseName);
            SAFE_DELETE_ARRAY(pData);
        }
    });
    connect(clientConnection, &QAbstractSocket::stateChanged, this, [clientConnection, this](QAbstractSocket::SocketState state) {
        qDebug() << "QAbstractSocket::stateChanged" << reinterpret_cast<int>(clientConnection) << state;
    });
}

void UBTMyPlayWidget::updateTcpCommands()
{
    command2hts.clear();
    QString settingFilePath = CConfigs::instance()->getTcpCommandFilePath();
    QFileInfo fileInfo(settingFilePath);
    if (fileInfo.exists()) {
        QSettings tcpCommands(settingFilePath, QSettings::IniFormat);
        tcpCommands.beginGroup("command");
        auto commands = tcpCommands.allKeys();
        qDebug() << "commands" << commands;
        for (auto itr = commands.begin(); itr != commands.end(); ++itr) {
            auto hts = tcpCommands.value(*itr).toString();
            command2hts.insert(*itr, hts);
        }
    } else {
        command2hts.insert("forward", "forward.hts");
        command2hts.insert("backward", "backward.hts");
        command2hts.insert("up", "up.hts");
        command2hts.insert("down", "down.hts");
    }
    qDebug() << "command2hts:" << command2hts;
}

int UBTMyPlayWidget::getTcpServerPort()
{
    int port = 8989;
    QString settingFilePath = CConfigs::instance()->getTcpCommandFilePath();
    QFileInfo fileInfo(settingFilePath);
    if (fileInfo.exists()) {
        QSettings settings(settingFilePath, QSettings::IniFormat);
        settings.beginGroup("server");
        port = settings.value("port", 8989).toInt();
    }
    qDebug() << "tcp server port:" << port;
    return port;
}
#endif

void UBTMyPlayWidget::onBatteryCapacityBtnClicked()
{
    EnableControlWindow(false);
    stopPlayTimerAndResetUI();
    addToControlDataBuffer(UBXPROTOCOL_CMDID_PLAY_TEST, NULL, 1);
}

void UBTMyPlayWidget::onPlayTimerTimeout()
{
    qDebug() << "UBTMyPlayWidget::onPlayTimerTimeout nextPlayItemRow:" << nextPlayItemRow;
    stopPlayTimerAndResetUI();
    playAndStartPlayTimer(nextPlayItemRow);
}

/***********************************************************************
 * 函数名： createPopMenuActions
 * 功能：  创建弹出菜单
 * 返回值： void
 * 作者：  周志平
 ***********************************************************************/
void UBTMyPlayWidget::createPopMenuActions()
{
    //创建菜单、菜单项
    QMenu* pop_menu = new QMenu();
    pop_menu->setGeometry(0,0,95,81);
    QAction* SysActionFile = new QAction(tr("Sync"),this);
    QAction* DeleteAction = new QAction(tr("Delete"),this);
    QAction* OpenActionFileDirAction = new QAction(tr("Open action file directory"), this);

    pop_menu->addAction(SysActionFile);
    pop_menu->addAction(DeleteAction);
    pop_menu->addAction(OpenActionFileDirAction);
    QAbstractItemDelegate::connect(SysActionFile,&QAction::triggered,this,&UBTMyPlayWidget::onSynchronizeClicked);
    QAbstractItemDelegate::connect(DeleteAction,&QAction::triggered,this,&UBTMyPlayWidget::onDeleteFile);
    QAbstractItemDelegate::connect(OpenActionFileDirAction,&QAction::triggered,this,&UBTMyPlayWidget::openActFileDirectory);

    //菜单出现的位置为当前鼠标的位置
    QFile styleSheet(":/res/qss/myPlayMenuDefault.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        pop_menu->setStyleSheet(styleSheet.readAll());
    }
    pop_menu->exec(QCursor::pos());
    pop_menu->deleteLater();
    //连接信号与槽

}

void UBTMyPlayWidget::UpdateUIStatusAsListChanged()
{
//    const bool bEnable = (ui->listWidgetAction->count() > 0);
    const bool bEnable = (ui->actionListView->model()->rowCount() > 0) && m_bConnect;
    ui->playOrderBtn->setEnabled(bEnable);
    ui->pBPlay->setEnabled(bEnable);
    ui->pBSynchronize->setEnabled(bEnable);
    ui->pBEmptyList->setEnabled(bEnable);
    ui->pBPre->setEnabled(bEnable);
    ui->pBNext->setEnabled(bEnable);
    if (ui->actionListView->model()->rowCount() > 0)
        ui->myPerformanceFileListContainer->setCurrentIndex(2);
    else
        ui->myPerformanceFileListContainer->setCurrentIndex(1);
}

/**************************************************************************
* 函数名: ThreadRecv
* 功能: 接收数据线程执行函数
* 参数:
*    @[in ] lpParam: 线程参数
* 返回值: 执行结果
* 时间: 2015/10/23 20:17
* 作者: ZDJ
*/
DWORD __stdcall UBTMyPlayWidget::ThreadSend(LPVOID lpParam)
{
    UBTMyPlayWidget* pMyPlayer= (UBTMyPlayWidget*)lpParam;
    while(true)
    {
        if (pMyPlayer->m_pWriteSemp)
        {
             pMyPlayer->m_pWriteSemp->WaitForSignal(0);
        }
        qDebug()<<"ThreadSend get Signal"<<endl;
        while(true)
        {
            CONTROL_DATA* pControlData = pMyPlayer->getControlDataFromBuffer();
            if (pControlData == NULL)
                break;
            pMyPlayer->m_objDataProcessor.Req_SendData(pControlData->nCmd, (byte*)(pControlData->pData), pControlData->nLen);

            if (pControlData->pData)
            {
                SAFE_DELETE_ARRAY(pControlData->pData);
            }

            if (pControlData)
            {
                SAFE_DELETE(pControlData);
            }
        }
        emit pMyPlayer->sigEnableWindow(true);
    }
    return 0;
}

/**************************************************************************
* 函数名: addToControlDataBuffer
* 功能: 添加到集控发送队列
* 参数: nCmd － 命令类型 pData－数据指针 nLen－数据长度
* 返回值: 成功返回设备路径，否则返回NULLSTR
* 时间: 2018/08/18
* 作者: Joker
*/
void UBTMyPlayWidget::addToControlDataBuffer(int nCmd, byte* pData, int nLen)
{
    CONTROL_DATA* pControlData = new CONTROL_DATA;

    pControlData->nCmd = nCmd;
    if (pData)
    {
        pControlData->pData = new byte[nLen];
        memcpy(pControlData->pData, pData, nLen);
    }
    else
    {
        pControlData->pData = NULL;
    }

    pControlData->nLen = nLen;

    m_hControlDataMutexWrite.Lock();
    m_vecControlDataBuffer.push_back(pControlData);
    m_hControlDataMutexWrite.Unlock();

    m_pWriteSemp->SendSignal();
}

/**************************************************************************
* 函数名: getControlDataFromBuffer
* 功能: 从集控发送队列里获取数据
* 参数: void
* 返回值: PCONTROL_DATA 数据
* 时间: 2018/08/18
* 作者: Joker
*/
PCONTROL_DATA UBTMyPlayWidget::getControlDataFromBuffer()
{
    PCONTROL_DATA pControlData = NULL;
    m_hControlDataMutexWrite.Lock();

    if (m_vecControlDataBuffer.size())
    {
        //pBlueData = m_vecControlDataBuffer[0];

        vector<PCONTROL_DATA>::iterator itr = m_vecControlDataBuffer.begin();
        pControlData = *itr;
        m_vecControlDataBuffer.erase(itr);
    }

    m_hControlDataMutexWrite.Unlock();

    return pControlData;
}

void UBTMyPlayWidget::playAndStartPlayTimer(int itemRow)
{
    do {
        QString actLibDir;
        if (!CConfigs::instance()->getLocalActLibDir(actLibDir)) {
            qDebug() << "UBTMyPlayWidget::playAndStartPlayTimer invalid act lib dir";
            break;
        }
        QString strText = getActionFileNameByRow(itemRow);
        if (strText.isEmpty()) {
            break;
        }
        QString strFileName = actLibDir +QDir::toNativeSeparators("/") + strText;
        qDebug() << "UBTMyPlayWidget::playAndStartPlayTimer strFileName:" << strFileName<<endl;

        QString strBasename =  CFileDirHandle::getFileBaseName(strFileName);
        qDebug() << "UBTMyPlayWidget::playAndStartPlayTimer strBasename:" << strBasename <<endl;
        char* pBaseName = NULL;
        UnicodeToUtf8(strBasename, pBaseName);
        size_t nLen = strlen(pBaseName);
        byte* pDataTemp = new  byte[nLen+1];
        byte* pData = pDataTemp;
        memcpy(pDataTemp, &nLen,  sizeof(byte));
        pDataTemp += sizeof(byte);
        memcpy(pDataTemp, pBaseName, nLen);

    //     m_objDataProcessor.Req_SendData(m_pPort, UBXPROTOCOL_CMDID_STOP, NULL, 1);
        m_objDataProcessor.Req_SendData(UBXPROTOCOL_CMDID_PLAY, (byte*)pData, strlen(pBaseName) + 1);

        addToControlDataBuffer(UBXPROTOCOL_CMDID_PLAY, (byte*)pData, strlen(pBaseName) + 1);
        SAFE_DELETE_ARRAY(pBaseName);
        SAFE_DELETE_ARRAY(pData);

        // start timer
        auto playItem = ui->actionListView->model()->index(itemRow, 0);
        if (!playItem.isValid())
            break;
        ui->actionListView->model()->setData(playItem, true, UBTActionListItemDelegate::PlayingFlagRole);
        playingItemRow = itemRow;
        bool getDataSucc = false;
        auto actionDuration = playItem.data(UBTActionListItemDelegate::ActionDurationRole).toInt(&getDataSucc);
        if (getDataSucc && actionDuration > 0) {
            int itemRow = playItem.row();
            qDebug() << "UBTMyPlayWidget::playAndStartPlayTimer current act item row:" << itemRow << "act duration:" << actionDuration << "play order:" << playOrder;
            if (playOrder == Random) {
                qsrand(QDateTime::currentMSecsSinceEpoch());
                nextPlayItemRow = qrand() % ui->actionListView->model()->rowCount();
                if (nextPlayItemRow == itemRow) {
                    nextPlayItemRow = (nextPlayItemRow + 1) % ui->actionListView->model()->rowCount();
                }
            }
            else if (playOrder == Loop) {
                ++itemRow;
                nextPlayItemRow = itemRow % ui->actionListView->model()->rowCount();
            }
            else if (playOrder == Single) {
                nextPlayItemRow = itemRow;
            }
            else {
                nextPlayItemRow = -1;
            }
            qDebug() << "UBTMyPlayWidget::playAndStartPlayTimer next item row:" << nextPlayItemRow;
            if (nextPlayItemRow != -1) {
                qDebug() << "UBTMyPlayWidget::playAndStartPlayTimer start play timer timeout in" << actionDuration << "sec";
                playTimer->start(actionDuration * 1000);
            }
        }
        else {
            qDebug() << "UBTMyPlayWidget::playAndStartPlayTimer get current act item cost time failed";
        }
    } while (0);
}

void UBTMyPlayWidget::stopPlayAndReset()
{
    stopPlayTimerAndResetUI();
    addToControlDataBuffer(UBXPROTOCOL_CMDID_STOP, NULL, 1);
}

void UBTMyPlayWidget::stopPlayTimerAndResetUI()
{
    playTimer->stop();

    auto prePlayItem = ui->actionListView->model()->index(playingItemRow, 0);
    if (prePlayItem.isValid()) {
        ui->actionListView->model()->setData(prePlayItem, false, UBTActionListItemDelegate::PlayingFlagRole);
    }
}

void UBTMyPlayWidget::updatePlayOrderBtnProperty()
{
    qDebug() << "UBTMyPlayWidget::updatePlayOrderBtnProperty" << QString::number(playOrder);
    ui->playOrderBtn->setProperty("order", QString::number(playOrder));
    ui->playOrderBtn->style()->unpolish(ui->playOrderBtn);
    ui->playOrderBtn->style()->polish(ui->playOrderBtn);
    ui->playOrderBtn->update();
}

/**************************************************************************
* 函数名: slotEnableWindow
* 功能: 启用窗体信号槽
* 参数:
*    @[in ]
* 返回值: void
* 时间: 2018/03/19
* 作者: Joker
*/
void UBTMyPlayWidget::slotEnableWindow(bool bEnable)
{
    EnableControlWindow(bEnable && m_bConnect);
    if (bEnable)
        UpdateUIStatusAsListChanged();//在EnableControlWindow之后检查，为了解决31479 【集控中心】列表中没有动作时，点击播放、上一首、下一首按钮，页面的按钮会一直置灰显示
}

/**************************************************************************
* 函数名: clearControlDataBuffer
* 功能: 清空集控发送数据
* 参数: void
* 返回值: void
* 时间: 2018/05/18
* 作者: Joker
*/
void UBTMyPlayWidget::clearControlDataBuffer()
{
    vector<PCONTROL_DATA>::iterator itr = m_vecControlDataBuffer.begin();
    m_hControlDataMutexWrite.Lock();
    while ( itr != m_vecControlDataBuffer.end())
    {
        PCONTROL_DATA pData = *itr;
        if (pData)
        {
            if (pData->pData)
            {
                SAFE_DELETE_ARRAY(pData->pData);
            }

            SAFE_DELETE(pData);
        }

        ++itr;
    }

    m_vecControlDataBuffer.clear();

    m_hControlDataMutexWrite.Unlock();
}
