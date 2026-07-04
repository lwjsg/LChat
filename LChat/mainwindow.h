#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QIcon>
#include"logindialog.h"
#include"zcdialog.h"
#include"resetdialog.h"
#include"chatdialog.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    LoginDialog* _log_dlg;
    zcDialog* _zc_dlg;
    ResetDialog* _reset_dlg;
    ChatDialog* _chat_dlg;
private slots:
    void SlotSwitchReg();
    void SlotSwitchLogin();
    void SlotSwitchReset();
    void SlotSwitchLogin2();
    void SlotSwitchChat();
};
#endif // MAINWINDOW_H
