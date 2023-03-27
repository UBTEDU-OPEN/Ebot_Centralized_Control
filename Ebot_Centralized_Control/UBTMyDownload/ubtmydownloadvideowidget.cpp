#include "ubtmydownloadvideowidget.h"
#include "ui_ubtmydownloadvideowidget.h"
#include "../UBTServerCommunication/ubtservercommunication.h"
#include "../UBTUtil/ubtutil.h"
#include "ubtvideowidget.h"
#include "../UBTPlayer/ubtframecapture.h"
#include "../UBTWidget/ubtconfirmdlg.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

const QString UBTMyDownloadVideoWidget::QSS_PATH(":/res/qss/ubtmydownloadvideowidget.qss");

UBTMyDownloadVideoWidget::UBTMyDownloadVideoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UBTMyDownloadVideoWidget)
{
    ui->setupUi(this);
    ui->videoMoreBtn->setProperty("expand", false);
    ui->performanceMoreBtn->setProperty("expand", false);
    setDefaultStyle();
    setVideoSearchLineEditorExpand(false);
    setPerformanceSearchLineEditorExpand(false);

//    frameCapture = new UBTFrameCapture(this);

    connect(ui->videoSearchIconBtn, &QPushButton::clicked, this, [this]() {
        setVideoSearchLineEditorExpand(ui->videoSearch->isHidden());
    });
    connect(ui->performanceSearchIconBtn, &QPushButton::clicked, this, [this]() {
        setPerformanceSearchLineEditorExpand(ui->performanceSearch->isHidden());
    });
    connect(ui->videoMoreBtn, &QPushButton::clicked, this, &UBTMyDownloadVideoWidget::onMoreVideoClicked);
    connect(ui->performanceMoreBtn, &QPushButton::clicked, this, &UBTMyDownloadVideoWidget::onMorePerformanceClicked);
    connect(ui->videoSearch, &QLineEdit::returnPressed, this, &UBTMyDownloadVideoWidget::onVideoSearch);
    connect(ui->performanceSearch, &QLineEdit::returnPressed, this, &UBTMyDownloadVideoWidget::onPerformanceSearch);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::getVideoListReply, this, &UBTMyDownloadVideoWidget::onGetVideoListReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::getPerformanceListReply, this, &UBTMyDownloadVideoWidget::onGetPerformanceRepley);
}

UBTMyDownloadVideoWidget::~UBTMyDownloadVideoWidget()
{
    delete ui;
}

void UBTMyDownloadVideoWidget::updateContent()
{
    setVideoSearchLineEditorExpand(false);
    setPerformanceSearchLineEditorExpand(false);
}

void UBTMyDownloadVideoWidget::updateVideoOrPerformanceContent(VideoType videoType)
{
    QJsonDocument jsonDoc;
    if (videoType == Video) {
        jsonDoc = QJsonDocument::fromJson(videoList);
    }
    else if (videoType == Performance) {
        jsonDoc = QJsonDocument::fromJson(performanceList);
    }
    QJsonArray videoJsonArray = jsonDoc.array();
    int videoCount = videoJsonArray.count();
    qDebug() << "UBTMyDownloadVideoWidget::updateVideoOrPerformanceContent video count:" << videoCount;

    QWidget *contentWidget = nullptr;
    bool expand = false;
    int MAX_ROW_COUNT = 0;
    if (videoType == Video) {
        contentWidget = ui->videoContent;
        expand = ui->videoMoreBtn->property("expand").toBool();
        if (!expand) {
            MAX_ROW_COUNT = 2;
        }
    }
    else if (videoType == Performance) {
        contentWidget = ui->performanceContent;
        expand = ui->performanceMoreBtn->property("expand").toBool();
        if (!expand) {
            MAX_ROW_COUNT = 1;
        }
    }
    auto areaLayout = dynamic_cast<QVBoxLayout*>(contentWidget->layout());
    if (!areaLayout) {
        areaLayout = new QVBoxLayout(contentWidget);
    }
    else {
        CUtil::cleanLayout(areaLayout);
    }
    areaLayout->setContentsMargins(0, 0, 0, 0);
    QMap<QString, QString> url2captureFile;
    const int COLUMN_COUNT = 3;
    for (int i = 0; i < videoCount; ++i) {
        if (videoJsonArray[i].isObject()) {
            QJsonObject videoJsonObj = videoJsonArray[i].toObject();
            qDebug() << "\UBTMyDownloadVideoWidget::updateVideoOrPerformanceContent" << i << "video info:";
            qDebug() << "\tvideo id:" << videoJsonObj["id"].toInt();
            qDebug() << "\tvideo name:" << videoJsonObj["name"].toString();
            qDebug() << "\tvideo url:" << videoJsonObj["url"].toString();
            qDebug() << "\tvideo thumbnail url:" << videoJsonObj["thumbnailUrl"].toString();

            int row = i / COLUMN_COUNT;
            if (MAX_ROW_COUNT != 0 && row >= MAX_ROW_COUNT)
                break;
            int column = i % COLUMN_COUNT;
            QHBoxLayout *lineLayout = nullptr;
            if (row < areaLayout->count()) {
                lineLayout = dynamic_cast<QHBoxLayout*>(areaLayout->itemAt(row)->layout());
            }
            if (!lineLayout) {
                lineLayout = new QHBoxLayout;
                lineLayout->insertStretch(0, 1);
                areaLayout->insertLayout(row, lineLayout);
            }
            lineLayout->setContentsMargins(15, 0, 15, 0);
            auto videoWidget = new UBTVideoWidget;
            lineLayout->insertWidget(column, videoWidget);
            videoWidget->setVideoInfo(videoJsonObj);

            url2captureFile.insert(videoJsonObj["url"].toString(), videoWidget->getThumbnailFullLocalName());

            connect(videoWidget, &UBTVideoWidget::videoDownloadStart, this, &UBTMyDownloadVideoWidget::videoDownloadStart);
            connect(videoWidget, &UBTVideoWidget::playOnlineVideo, this, &UBTMyDownloadVideoWidget::playOnlineVideo);
        }
        else {
            qDebug() << "UBTMyDownloadVideoWidget::updateVideoList" << i << "value in array is not object";
        }
    }
    areaLayout->addStretch(1);
//    frameCapture->setUrlAndCaptureFile(url2captureFile);
//    frameCapture->startCapture();
}

void UBTMyDownloadVideoWidget::setDefaultStyle()
{
    QFile styleSheet(QSS_PATH);
    if(styleSheet.open(QIODevice::ReadOnly)) {
        setStyleSheet(styleSheet.readAll());
    }
}

void UBTMyDownloadVideoWidget::setVideoSearchLineEditorExpand(bool expand)
{
    if (expand) {
        ui->videoSearchIconBtn->setProperty("showAlone", false);
        ui->videoSearchIconBtn->style()->unpolish(ui->videoSearchIconBtn);
        ui->videoSearchIconBtn->style()->polish(ui->videoSearchIconBtn);
        ui->videoSearchIconBtn->update();
        ui->videoSearch->show();
    }
    else {
        ui->videoSearchIconBtn->setProperty("showAlone", true);
        ui->videoSearchIconBtn->style()->unpolish(ui->videoSearchIconBtn);
        ui->videoSearchIconBtn->style()->polish(ui->videoSearchIconBtn);
        ui->videoSearchIconBtn->update();
        ui->videoSearch->hide();
    }
}

void UBTMyDownloadVideoWidget::setPerformanceSearchLineEditorExpand(bool expand)
{
    if (expand) {
        ui->performanceSearchIconBtn->setProperty("showAlone", false);
        ui->performanceSearchIconBtn->style()->unpolish(ui->performanceSearchIconBtn);
        ui->performanceSearchIconBtn->style()->polish(ui->performanceSearchIconBtn);
        ui->performanceSearchIconBtn->update();
        ui->performanceSearch->show();
    }
    else {
        ui->performanceSearchIconBtn->setProperty("showAlone", true);
        ui->performanceSearchIconBtn->style()->unpolish(ui->performanceSearchIconBtn);
        ui->performanceSearchIconBtn->style()->polish(ui->performanceSearchIconBtn);
        ui->performanceSearchIconBtn->update();
        ui->performanceSearch->hide();
    }
}

void UBTMyDownloadVideoWidget::onGetVideoListReply(bool succ, const QString &info, const QByteArray &reply)
{
    if (!succ) {;
        UBTConfirmDlg *dlg = new UBTConfirmDlg("", "UBTMyDownloadVideoWidget::onGetVideoListReply" + info + QString(reply), tr("OK"), "");
        connect(dlg, &UBTConfirmDlg::acceptClicked, this, [this, dlg]() {
            dlg->close();
        });
        dlg->showInCenter();
        return;
    }
    videoList = reply;
    updateVideoOrPerformanceContent(Video);
}

void UBTMyDownloadVideoWidget::onGetPerformanceRepley(bool succ, const QString &info, const QByteArray &reply)
{
    if (!succ) {
        UBTConfirmDlg *dlg = new UBTConfirmDlg("", "UBTMyDownloadVideoWidget::onGetVideoListReply" + info + QString(reply), tr("OK"), "");
        connect(dlg, &UBTConfirmDlg::acceptClicked, this, [this, dlg]() {
            dlg->close();
        });
        dlg->showInCenter();
        return;
    }
    performanceList = reply;
    updateVideoOrPerformanceContent(Performance);
}

void UBTMyDownloadVideoWidget::onMoreVideoClicked()
{
    auto expand = ui->videoMoreBtn->property("expand").toBool();
    ui->videoMoreBtn->setProperty("expand", !expand);
    ui->videoMoreBtn->style()->unpolish(ui->videoMoreBtn);
    ui->videoMoreBtn->style()->polish(ui->videoMoreBtn);
    ui->videoMoreBtn->update();
    updateVideoOrPerformanceContent(Video);
}

void UBTMyDownloadVideoWidget::onMorePerformanceClicked()
{
    auto expand = ui->performanceMoreBtn->property("expand").toBool();
    ui->performanceMoreBtn->setProperty("expand", !expand);
    ui->performanceMoreBtn->style()->unpolish(ui->performanceMoreBtn);
    ui->performanceMoreBtn->style()->polish(ui->performanceMoreBtn);
    ui->performanceMoreBtn->update();
    updateVideoOrPerformanceContent(Performance);
}

void UBTMyDownloadVideoWidget::onVideoSearch()
{
    UBTServerCommunication::getInstance()->getVideoList(ui->videoSearch->text());
}

void UBTMyDownloadVideoWidget::onPerformanceSearch()
{
    UBTServerCommunication::getInstance()->getPerformanceList(ui->performanceSearch->text());
}
