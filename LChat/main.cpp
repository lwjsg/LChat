#include "mainwindow.h"
#include <QFile>
#include <QApplication>
#include<QDebug>
#include<QDir>
#include<QSettings>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 加载 QSS 文件
    QFile qssFile(":/style/stylesheet.qss");   // 你的 qss 文件路径
    if(qssFile.open(QFile::ReadOnly))
    {
        qDebug()<<"open succcess\n";
        QString qssStr = qssFile.readAll();
        // 设置到整个程序全局（最常用）
        a.setStyleSheet(qssStr);
        qssFile.close();
    }
    else
    {
        qDebug()<<"open failed\n";
    }

    // 获取当前应用程序的路径
    QString app_path = QCoreApplication::applicationDirPath();
    // 拼接文件名
    QString fileName = "config.ini";
    QString config_path = QDir::toNativeSeparators(app_path +
                                                   QDir::separator() + fileName);

    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://"+gate_host+":"+gate_port;

    // qDebug() << "配置文件路径：" << config_path;
    // qDebug() << "最终请求前缀：" << gate_url_prefix;

    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
