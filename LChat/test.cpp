// #include "httpmgr.h"

// HttpMgr::HttpMgr()
// {
//     connect(this, &HttpMgr::sig_http_finish, this, &HttpMgr::slot_http_finish);
// }

// void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
// {
//     //将json转换成字节数组
//     QByteArray str = QJsonDocument(json).toJson();
//     //创建请求头
//     QNetworkRequest request(url);

//     request.setHeader(QNetworkRequest::ContentTypeHeader,"application/jason");
//     request.setHeader(QNetworkRequest::ContentLengthHeader,str.length());
//     QNetworkReply* reply = _manager.post(request,str);
//     auto self = shared_from_this();
//     connect(reply,&QNetworkReply::finished,[self,reply,req_id,mod](){
//         if(reply->error() != QNetworkReply::NoError){
//             qDebug()<<reply->errorString();
//             emit self->sig_http_finish()
//         }
//     });
// }

// void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
// {
//     if(mod == Modules::REGISTERMOD){
//         //发送信号通知指定模块http响应结束
//         emit sig_reg_mod_finish(id, res, err);
//     }
// }

// HttpMgr::~HttpMgr()
// {

// }
