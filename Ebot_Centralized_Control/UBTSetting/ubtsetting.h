#ifndef UBTSETTING_H
#define UBTSETTING_H

#include "ubtsetting_global.h"

#include <QWidget>
#include <QVector>

class QPixmap;
class QButtonGroup;

namespace Ui {
class UBTSetting;
}

class UBTSETTINGSHARED_EXPORT UBTSetting : public QWidget
{
    Q_OBJECT

private:
    static const QString QSS_PATH;

public:
    explicit UBTSetting(QWidget *parent = 0);
    ~UBTSetting();

    void upgradeRequest();
    void onManualBtnClicked();
    void onVersionBtnClicked();

protected:
    void setDefaultStyle();
    void onUpgradeBtnClicked();
    void onUpgradeRequestReply(bool succ, QString info, QByteArray reply);
    QString getCurrentVersion() const;

private:
    Ui::UBTSetting *ui;

    QButtonGroup *btnGrp;
    QString newSoftwareUrl;

    QPixmap* manual;
};

#endif // UBTSETTING_H
