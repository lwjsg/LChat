#ifndef ZCDIALOG_H
#define ZCDIALOG_H

#include <QDialog>
#include<global.h>
namespace Ui {
class zcDialog;
}

class zcDialog : public QDialog
{
    Q_OBJECT

public:
    explicit zcDialog(QWidget *parent = nullptr);
    ~zcDialog();

signals:
    void sigSwitchLogin();
private slots:
    void on_btn_hq_clicked();
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
    void on_btn_qd_clicked();

    void on_sdakd_clicked();

    void on_btn_qx_clicked();

private:
    Ui::zcDialog *ui;
    void showTip(QString str,bool ok);
    void InitHttpHandler();
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;

    QMap<TipErr, QString> _tip_errs;
    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);
    bool checkUserValid();
    bool checkPassValid();
    bool checkEmailValid();
    bool checkConfirmValid();
    bool checkVarifyValid();

    void ChangeTipPage();

    QTimer* _countdown_timer;
    int _countdown;
};

#endif // ZCDIALOG_H
