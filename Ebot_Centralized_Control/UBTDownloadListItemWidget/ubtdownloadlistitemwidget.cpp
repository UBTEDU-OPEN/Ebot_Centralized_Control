#include "ubtdownloadlistitemwidget.h"
#include "ui_ubtdownloadlistitemwidget.h"

#include <QCheckBox>
#include <QFile>

const QString UBTDownloadListItemWidget::QSS_PATH(":/res/qss/ubtdownloadwidget.qss");

UBTDownloadListItemWidget::UBTDownloadListItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UBTDownloadListItemWidget)
{
    ui->setupUi(this);
    ui->downloadProgress->setMinimum(0);
    ui->downloadProgress->setMaximum(100);
    ui->downloadProgress->setValue(0);
    setDefaultStyle();

    connect(ui->downloadItemName, &QCheckBox::stateChanged, this, [this](int state) {
        emit selected(state != Qt::Unchecked);
    });
    connect(ui->videoRedownloadBtn, &QPushButton::clicked, this, &UBTDownloadListItemWidget::redownloadClicked);
}

UBTDownloadListItemWidget::~UBTDownloadListItemWidget()
{
    delete ui;
}

void UBTDownloadListItemWidget::asTitle()
{
    ui->downloadProgress->setValue(0);
    ui->videoRedownloadBtn->hide();
    ui->downloadProgress->hide();
    ui->statusLbl->setText(tr("status"));
    ui->statusLbl->show();
    setObjectName("ListHeader");
}

void UBTDownloadListItemWidget::setName(const QString &name)
{
    ui->downloadItemName->setText(name);
}

QString UBTDownloadListItemWidget::getName() const
{
    return ui->downloadItemName->text();
}

void UBTDownloadListItemWidget::setId(int id)
{
    this->id = id;
}

int UBTDownloadListItemWidget::getId() const
{
    return id;
}

void UBTDownloadListItemWidget::setDownloadStatus(UBTDownloadListItemWidget::DownloadStatus downloadStatus)
{
    this->downloadStatus = downloadStatus;

    switch(this->downloadStatus) {
    case Undownload: {
        ui->videoRedownloadBtn->hide();
        ui->downloadProgress->hide();
        ui->statusLbl->setText(tr("undownload"));
        ui->statusLbl->show();
    }
        break;
    case Downloading: {
        ui->videoRedownloadBtn->hide();
        ui->downloadProgress->show();
        ui->statusLbl->hide();
    }
        break;
    case Downloaded: {
        ui->videoRedownloadBtn->hide();
        ui->downloadProgress->hide();
        ui->statusLbl->setText(tr("downloaded"));
        ui->statusLbl->show();
    }
        break;
    case DownloadFailed: {
        ui->videoRedownloadBtn->show();
        ui->downloadProgress->hide();
        ui->statusLbl->hide();
    }
        break;
    default:
        break;
    }
}

UBTDownloadListItemWidget::DownloadStatus UBTDownloadListItemWidget::getDownloadStatus() const
{
    return downloadStatus;
}

void UBTDownloadListItemWidget::setDownloadProgress(int progress)
{
    if (progress >= 100) {
        ui->downloadProgress->setValue(100);
    }
    else if (progress < 0) {
        ui->downloadProgress->setValue(0);
    }
    else {
        ui->downloadProgress->setValue(progress);
    }
}

bool UBTDownloadListItemWidget::isSelected() const
{
    return ui->downloadItemName->isChecked();
}

void UBTDownloadListItemWidget::setSelected(bool selected)
{
    ui->downloadItemName->setChecked(selected);
}

void UBTDownloadListItemWidget::setFilePath(const QString &filePath)
{
    this->filePath = filePath;
}

QString UBTDownloadListItemWidget::getFilePath() const
{
    return filePath;
}

void UBTDownloadListItemWidget::setUrl(QUrl url)
{
    this->url = url;
}

QUrl UBTDownloadListItemWidget::getUrl() const
{
    return url;
}

void UBTDownloadListItemWidget::setDefaultStyle()
{
    QFile styleSheet(QSS_PATH);
    if(styleSheet.open(QIODevice::ReadOnly)) {
        setStyleSheet(styleSheet.readAll());
    }
}

