#include "global.h"

std::function<void(QWidget*)> repolish = [](QWidget* w){
    w->style()->unpolish(w);//清除原有格式
    w->style()->polish(w);//刷新样式
};

bool isValidEmail(const QString &email) {
    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex.match(email).hasMatch();
}
QString gate_url_prefix = "";

//用户密码加密函数
extern std::function<QString(QString)> xorString = [](QString str){
    QString temp = str;
    int length = str.length()%255;
    for(int i=0;i<length;++i)
    {
        //对每个字节进行异或操作
        temp[i] = QChar(static_cast<ushort>(str[i].unicode() ^ static_cast<ushort>(length)));
    }
    return temp;
};