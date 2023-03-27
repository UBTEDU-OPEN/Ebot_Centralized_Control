#include "ubtactionlibwidget.h"
#include "ui_ubtactionlibwidget.h"
#include "../UBTServerCommunication/ubtservercommunication.h"
#include "ubtactionwidget.h"
#include "../UBTUtil/ubtutil.h"

#include <QFileDialog>
#include <QTimer>
#include <QDebug>
#include <QJsonDocument>
#include <QHBoxLayout>
#include <QVBoxLayout>

UbtActionLibWidget::UbtActionLibWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UbtActionLibWidget)
{
    ui->setupUi(this);
    SetDefaultStyle();
    setSearchLineEditorExpand(false);
    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->loginBtn, &QPushButton::clicked, this, &UbtActionLibWidget::onLoginClicked);

    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::addActionDownloadRecordReply, this, &UbtActionLibWidget::onAddActionDownloadRecordReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::clearActionDownloadRecordReply, this, &UbtActionLibWidget::onClearActionDownloadRecordReply);
    connect(UBTServerCommunication::getInstance(), &UBTServerCommunication::getActionListReply, this, &UbtActionLibWidget::onGetActionListReply);

    connect(ui->searchLineEdit, &QLineEdit::returnPressed, this, &UbtActionLibWidget::onSearch);
    connect(ui->searchIconBtn, &QPushButton::clicked, this, [this]() {
        setSearchLineEditorExpand(ui->searchLineEdit->isHidden());
    });
}

UbtActionLibWidget::~UbtActionLibWidget()
{
    delete ui;
}

void UbtActionLibWidget::onSearch()
{
    UBTServerCommunication::getInstance()->getActionList(ui->searchLineEdit->text());
}

void UbtActionLibWidget::onAddActionDownloadRecordReply(int httpStatusCode, int error)
{
//    qDebug()<< "\t>>> UbtActionLibWidget::onAddActionDownloadRecordReply httpStatusCode:" <<  httpStatusCode << " error:" << error;
}

void UbtActionLibWidget::onClearActionDownloadRecordReply(bool succ, QString info)
{
}

void UbtActionLibWidget::onGetActionListReply(bool succ, QString info, QByteArray reply)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply);
    QJsonArray actionsJsonArray = jsonDoc.array();
    int actionsCount = actionsJsonArray.count();
    qDebug() << "UbtActionLibWidget::updateActionLib actionsCount:" << actionsCount;

    auto areaLayout = dynamic_cast<QVBoxLayout*>(ui->actionScrollAreaWidgetContents->layout());
    if (!areaLayout) {
        areaLayout = new QVBoxLayout(ui->actionScrollAreaWidgetContents);
    }
    else {
        CUtil::cleanLayout(areaLayout);
    }

    if (actionsCount > 0) {
        ui->noActionIconLbl->hide();
        ui->noActionTextLbl->hide();

        const int COLUMN_COUNT = 3;
        for (int i = 0; i < actionsCount; ++i) {
            if (actionsJsonArray[i].isObject()) {
                QJsonObject actionJsonObj = actionsJsonArray[i].toObject();
                qDebug() << "\nUbtActionLibWidget::updateActionLib" << i << "action info:";
                qDebug() << "\taction id:" << actionJsonObj["id"].toInt();
                qDebug() << "\taction name:" << actionJsonObj["name"].toString();
                qDebug() << "\taction url:" << actionJsonObj["url"].toString();
                qDebug() << "\taction download flag:" << actionJsonObj["flag"].toInt();
                qDebug() << "\taction thumbnail url:" << actionJsonObj["thumbnailUrl"].toString();

                int row = i / COLUMN_COUNT;
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
                auto actionWidget = new UBTActionWidget;
                lineLayout->insertWidget(column, actionWidget);

                actionWidget->setActionInfo(actionJsonObj);

                connect(actionWidget, &UBTActionWidget::actionDownloadFinished, this, &UbtActionLibWidget::actionDownloadFinished);
                connect(actionWidget, &UBTActionWidget::actionDownloadFailed, this, &UbtActionLibWidget::actionDownloadFailed);
            }
            else {
                qDebug() << "UbtActionLibWidget::updateActionLib" << i << "value in array is not object";
            }
        }
        areaLayout->addStretch(1);
    }
    else {
        ui->noActionIconLbl->show();
        ui->noActionTextLbl->show();
    }
}

void UbtActionLibWidget::onLoginClicked()
{
    UBTServerCommunication::getInstance()->setLoginEntrance(UBTServerCommunication::MyPlay);
    emit loginClicked();
}

void UbtActionLibWidget::updateContent()
{
    if (UBTServerCommunication::getInstance()->isLogin()) {
        ui->stackedWidget->setCurrentWidget(ui->actionOnSvrListPage);
    }
    else {
        ui->stackedWidget->setCurrentWidget(ui->logInNotifyPage);
    }
}

void UbtActionLibWidget::SetDefaultStyle()
{
    QFile styleSheet(ActionLib_WIDGET_DEFAULT_QSS_PATH);
    if(styleSheet.open(QIODevice::ReadOnly)) {
        setStyleSheet(styleSheet.readAll());
    }
}

void UbtActionLibWidget::setSearchLineEditorExpand(bool expand)
{
    if (expand) {
        ui->searchIconBtn->setProperty("showAlone", false);
        ui->searchIconBtn->style()->unpolish(ui->searchIconBtn);
        ui->searchIconBtn->style()->polish(ui->searchIconBtn);
        ui->searchIconBtn->update();
        ui->searchLineEdit->show();
    }
    else {
        ui->searchIconBtn->setProperty("showAlone", true);
        ui->searchIconBtn->style()->unpolish(ui->searchIconBtn);
        ui->searchIconBtn->style()->polish(ui->searchIconBtn);
        ui->searchIconBtn->update();
        ui->searchLineEdit->hide();
    }
}
