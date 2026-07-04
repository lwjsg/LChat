#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpmgr.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowIcon(QIcon(":/Icon/icon_lchat.ico"));
    ui->setupUi(this);
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _log_dlg = new LoginDialog(this);
    _log_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_log_dlg);

    //连接登录界面注册信号
    connect(_log_dlg, &LoginDialog::signal_switch, this, &MainWindow::SlotSwitchReg);
    //连接登录界面忘记密码信号
    connect(_log_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
    //连接创建聊天界面信号
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_swich_chatdlg, this, &MainWindow::SlotSwitchChat);

    //emit TcpMgr::GetInstance()->sig_swich_chatdlg();
}

void MainWindow::SlotSwitchReg()
{
    _zc_dlg = new zcDialog(this);
    _zc_dlg->hide();

    _zc_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    //连接注册界面返回登录信号
    connect(_zc_dlg, &zcDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);
    setCentralWidget(_zc_dlg);
    _log_dlg->hide();
    _zc_dlg->show();
}
//从注册界面返回登录界面
void MainWindow::SlotSwitchLogin()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _log_dlg = new LoginDialog(this);
    _log_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_log_dlg);

    _zc_dlg->hide();
    _log_dlg->show();
    //连接登录界面注册信号
    connect(_log_dlg, &LoginDialog::signal_switch, this, &MainWindow::SlotSwitchReg);
    //连接登录界面忘记密码信号
    connect(_log_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

void MainWindow::SlotSwitchReset()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _reset_dlg = new ResetDialog(this);
    _reset_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_reset_dlg);

    _log_dlg->hide();
    _reset_dlg->show();
    //注册返回登录信号和槽函数
    connect(_reset_dlg, &ResetDialog::switchLogin, this, &MainWindow::SlotSwitchLogin2);
}

void MainWindow::SlotSwitchLogin2()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _log_dlg = new LoginDialog(this);
    _log_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_log_dlg);

    _reset_dlg->hide();
    _log_dlg->show();
    //连接登录界面忘记密码信号
    connect(_log_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
    //连接登录界面注册信号
    connect(_log_dlg, &LoginDialog::signal_switch, this, &MainWindow::SlotSwitchReg);
    //_ui_status = LOGIN_UI;
}

//跳转聊天界面
void MainWindow::SlotSwitchChat()
{
    _chat_dlg = new ChatDialog();
    _chat_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_chat_dlg);
    _chat_dlg->show();
    _log_dlg->hide();
    this->setMinimumSize(QSize(800,630));
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}

MainWindow::~MainWindow()
{
    delete ui;
}
