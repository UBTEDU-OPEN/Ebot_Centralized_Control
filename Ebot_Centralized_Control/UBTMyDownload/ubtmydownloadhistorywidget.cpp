#include "ubtmydownloadhistorywidget.h"
#include "ui_ubtmydownloadhistorywidget.h"
#include "../UBTUtil/configs.h"
#include "../UBTDownloadListItemWidget/ubtdownloadlistitemwidget.h"
#include "../UBTUtil/filedirhandle.h"
#include "../UBTServerCommunication/ubtservercommunication.h"

#include <QFile>
#include <QString>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QByteArray>
#include <QDesktopServices>
#include <qDebug>

const QString UBTMyDownloadHistoryWidget::QSS_PATH(":/res/qss/ubtmydownloadhistorywidget.qss");

const QString UBTMyDownloadHistoryWidget::JSON_KEY_ID("id");
const QString UBTMyDownloadHistoryWidget::JSON_KEY_NAME("name");
const QString UBTMyDownloadHistoryWidget::JSON_KEY_DOWNLOAD_STATUS("downloadStatus");
const QString UBTMyDownloadHistoryWidget::JSON_KEY_URL("url");
const QString UBTMyDownloadHistoryWidget::JSON_KEY_FILE_PATH("filePath");

UBTMyDownloadHistoryWidget::UBTMyDownloadHistoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UBTMyDownloadHistoryWidget)
{
    ui->setupUi(this);
    ui->downloadHistoryDelBtn->setEnabled(false);
    listHeader = new UBTDownloadListItemWidget;
    listHeader->setName(tr("name"));
    listHeader->asTitle();
    ui->downloadVideoList->setAlternatingRowColors(true);
    ui->downloadVideoListContainerLayout->insertWidget(0, listHeader);
    setDefaultStyle();
    updateContent();

    connect(ui->savePathModifyBtn, &QPushButton::clicked, this, [this]() {
        QString newDir = QFileDialog::getExistingDirectory(this, tr("select action library directory"),
                                                        CConfigs::instance()->getVideoDownloadPath(),
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
        CConfigs::instance()->setVideoDownloadPath(newDir);
        loadVideoPath();
    });

    connect(ui->downloadHistoryDelBtn, &QPushButton::clicked, this, &UBTMyDownloadHistoryWidget::onDeleteClicked);
    connect(ui->downloadVideoList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
        if (item){
            if (auto widget = dynamic_cast<UBTDownloadListItemWidget*>(ui->downloadVideoList->itemWidget(item))) {
                QFileInfo fileInfo(widget->getFilePath());
                QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
            }
        }
    });
}

UBTMyDownloadHistoryWidget::~UBTMyDownloadHistoryWidget()
{
    delete ui;
}

void UBTMyDownloadHistoryWidget::updateContent()
{
    loadListContent();
    updateListArea();
    updateDelBtnStatus();
    loadVideoPath();
}

void UBTMyDownloadHistoryWidget::onVideoDownloadStart(int id, QString name, QUrl url, QString filePath)
{
    if (auto w = getTargetItemWidget(id, filePath)) {
        w->setDownloadStatus(UBTDownloadListItemWidget::Downloading);
    }
    else {
//        qDebug() << "UBTMyDownloadHistoryWidget::onVideoDownloadStart id:" << id << "name:" << name << "url:" << url << "file path:" << filePath;
        auto item = new QListWidgetItem;
        item->setToolTip(filePath);
        ui->downloadVideoList->addItem(item);
        UBTDownloadListItemWidget *widget = new UBTDownloadListItemWidget;
        widget->setId(id);
        widget->setName(name);
        widget->setDownloadProgress(0);
        widget->setUrl(url);
        widget->setFilePath(filePath);
        widget->setDownloadStatus(UBTDownloadListItemWidget::Downloading);
        ui->downloadVideoList->setItemWidget(item, widget);

        connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::downloadVideoProgress, this, &UBTMyDownloadHistoryWidget::onVideoDownloadProgress);
        connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::downloadVideoFinished, this, &UBTMyDownloadHistoryWidget::onVideoDownloadFinished);
        connect(listHeader, &UBTDownloadListItemWidget::selected, widget, &UBTDownloadListItemWidget::setSelected);
        connect(widget, &UBTDownloadListItemWidget::selected, this, &UBTMyDownloadHistoryWidget::updateDelBtnStatus);
        connect(widget, &UBTDownloadListItemWidget::redownloadClicked, this, [this, widget]() {
            widget->setDownloadStatus(UBTDownloadListItemWidget::Downloading);
            UBTServerCommunication::getInstance()->downloadVideo(widget->getId(), widget->getUrl(), widget->getFilePath());
        });
    }

    updateListArea();
    updateDelBtnStatus();
    saveListContent();
}

void UBTMyDownloadHistoryWidget::onVideoDownloadProgress(int id, QString filePath, qint64 bytesReceived, qint64 bytesTotal)
{
    if (auto w = getTargetItemWidget(id, filePath)) {
        auto progress = static_cast<double>(bytesReceived) * 100 / static_cast<double>(bytesTotal);
        w->setDownloadProgress(progress);
    }
}

void UBTMyDownloadHistoryWidget::onVideoDownloadFinished(int id, QString filePath, bool succ)
{
    if (auto w = getTargetItemWidget(id, filePath)) {
        if (succ) {
            w->setDownloadStatus(UBTDownloadListItemWidget::Downloaded);
        }
        else {
            w->setDownloadStatus(UBTDownloadListItemWidget::DownloadFailed);
        }
        saveListContent();
    }
}

void UBTMyDownloadHistoryWidget::loadListContent()
{
    ui->downloadVideoList->clear();

    QFile videoDownloadHistory(CConfigs::instance()->getVideoDownloadHistoryFile());
    if (videoDownloadHistory.open(QIODevice::ReadOnly)) {
        auto data = videoDownloadHistory.readAll();
        QJsonDocument doc = QJsonDocument::fromBinaryData(data);
        QJsonArray array = doc.array();
        for (int i = 0; i < array.count(); ++i) {
            QJsonObject obj = array.at(i).toObject();
            auto item = new QListWidgetItem;
            item->setToolTip(obj.value(JSON_KEY_FILE_PATH).toString());
            ui->downloadVideoList->addItem(item);
            UBTDownloadListItemWidget *widget = new UBTDownloadListItemWidget;
            widget->setId(obj.value(JSON_KEY_ID).toInt());
            widget->setName(obj.value(JSON_KEY_NAME).toString());
            widget->setDownloadStatus(static_cast<UBTDownloadListItemWidget::DownloadStatus>(obj.value(JSON_KEY_DOWNLOAD_STATUS).toInt()));
            widget->setUrl(QUrl(obj.value(JSON_KEY_URL).toString()));
            widget->setFilePath(obj.value(JSON_KEY_FILE_PATH).toString());
            ui->downloadVideoList->setItemWidget(item, widget);

            connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::downloadVideoProgress, this, &UBTMyDownloadHistoryWidget::onVideoDownloadProgress);
            connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::downloadVideoFinished, this, &UBTMyDownloadHistoryWidget::onVideoDownloadFinished);
            connect(listHeader, &UBTDownloadListItemWidget::selected, widget, &UBTDownloadListItemWidget::setSelected);
            connect(widget, &UBTDownloadListItemWidget::selected, this, &UBTMyDownloadHistoryWidget::updateDelBtnStatus);
            connect(widget, &UBTDownloadListItemWidget::redownloadClicked, this, [this, widget]() {
                UBTServerCommunication::getInstance()->downloadVideo(widget->getId(), widget->getUrl(), widget->getFilePath());
            });
        }
        videoDownloadHistory.close();
    }
    else {
        qDebug() << "UBTMyDownloadHistoryWidget::saveListContent open file failed";
    }
}

void UBTMyDownloadHistoryWidget::saveListContent()
{
    QFile videoDownloadHistory(CConfigs::instance()->getVideoDownloadHistoryFile());
    if (videoDownloadHistory.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QJsonArray array;
        for (int i = 0; i < ui->downloadVideoList->count(); ++i) {
            auto item = ui->downloadVideoList->item(i);
            if (auto widget = dynamic_cast<UBTDownloadListItemWidget*>(ui->downloadVideoList->itemWidget(item))) {
                QJsonObject obj;
                obj.insert(JSON_KEY_ID, widget->getId());
                obj.insert(JSON_KEY_NAME, widget->getName());
                obj.insert(JSON_KEY_DOWNLOAD_STATUS, (widget->getDownloadStatus() == UBTDownloadListItemWidget::Downloaded) ? UBTDownloadListItemWidget::Downloaded : UBTDownloadListItemWidget::DownloadFailed);
                obj.insert(JSON_KEY_URL, widget->getUrl().toString());
                obj.insert(JSON_KEY_FILE_PATH, widget->getFilePath());
                array.append(obj);
            }
        }
        QJsonDocument doc(array);
        if (!videoDownloadHistory.write(doc.toBinaryData())) {
            qDebug() << "UBTMyDownloadHistoryWidget::saveListContent write file failed";
        }
        videoDownloadHistory.close();
    }
    else {
        qDebug() << "UBTMyDownloadHistoryWidget::saveListContent open file failed";
    }
}

void UBTMyDownloadHistoryWidget::updateListArea()
{
    if (ui->downloadVideoList->count() > 0) {
        ui->downloadHistoryContent->setCurrentWidget(ui->downloadVideoListContainer);
    }
    else {
        ui->downloadHistoryContent->setCurrentWidget(ui->noContent);
    }
}

void UBTMyDownloadHistoryWidget::updateDelBtnStatus()
{
    for (int i = 0; i < ui->downloadVideoList->count(); ++i) {
        auto item = ui->downloadVideoList->item(i);
        if (auto widget = dynamic_cast<UBTDownloadListItemWidget*>(ui->downloadVideoList->itemWidget(item))) {
            if (widget->isSelected()) {
                ui->downloadHistoryDelBtn->setEnabled(true);
                return;
            }
        }
    }
    ui->downloadHistoryDelBtn->setEnabled(false);
}

void UBTMyDownloadHistoryWidget::onDeleteClicked()
{
    QList<QListWidgetItem *> itemsToRm;
    for (int i = 0; i < ui->downloadVideoList->count(); ++i) {
        auto item = ui->downloadVideoList->item(i);
        if (auto widget = dynamic_cast<UBTDownloadListItemWidget*>(ui->downloadVideoList->itemWidget(item))) {
            if (widget->isSelected()) {
                itemsToRm.append(item);
            }
        }
    }
    while (itemsToRm.count() != 0) {
        auto item = itemsToRm.takeLast();
        item->setHidden(true);
        delete item;
    }
    updateListArea();
    updateDelBtnStatus();
    saveListContent();
}

UBTDownloadListItemWidget *UBTMyDownloadHistoryWidget::getTargetItemWidget(int id, QString filePath)
{
    for (int i = 0; i < ui->downloadVideoList->count(); ++i) {
        if (auto item = ui->downloadVideoList->item(i)) {
            if (auto widget = dynamic_cast<UBTDownloadListItemWidget*>(ui->downloadVideoList->itemWidget(item))) {
                if (widget->getId() == id && widget->getFilePath() == filePath) {
                    return widget;
                }
            }
        }
    }
    return nullptr;
}

void UBTMyDownloadHistoryWidget::setDefaultStyle()
{
    QFile styleSheet(QSS_PATH);
    if(styleSheet.open(QIODevice::ReadOnly)) {
        setStyleSheet(styleSheet.readAll());
    }
}

void UBTMyDownloadHistoryWidget::loadVideoPath()
{
    ui->savePathLineEdit->setText(CConfigs::instance()->getVideoDownloadPath());
}

