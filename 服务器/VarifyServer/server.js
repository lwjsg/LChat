
const grpc = require('@grpc/grpc-js')
const message_proto = require('./proto')
const const_module = require('./const')
const { v4: uuidv4 } = require('uuid');
const emailModule = require('./email');
const redis_module = require('./redis')

/**
 * GetVarifyCode grpc响应获取验证码的服务
 * @param {*} call 为grpc请求 
 * @param {*} callback 为grpc回调
 * @returns  
 */
//call, callback两个参数类似于C++中的grpc::ServerContext，只不过nodejs通过两个上下文管理
async function GetVarifyCode(call, callback) {
    console.log("email is ", call.request.email)
    try{
        let query_res = await redis_module.GetRedis(const_module.code_prefix+call.request.email);
        console.log("query_res is ", query_res)
        if(query_res == null){

        }
        let uniqueId = query_res;
        if(query_res ==null){
            uniqueId = uuidv4();
            if (uniqueId.length > 4) {
                uniqueId = uniqueId.substring(0, 4);
            } 
            let bres = await redis_module.SetRedisExpire(const_module.code_prefix+call.request.email, uniqueId,600)
            if(!bres){
                callback(null, { email:  call.request.email,
                    error:const_module.Errors.RedisErr
                });
                return;
            }
        }

        console.log("uniqueId is ", uniqueId)
        let text_str =  '您的验证码为'+ uniqueId +'请三分钟内完成注册'
        //发送邮件
        let mailOptions = {
            from: '19196301103@163.com',
            to: call.request.email,
            subject: '验证码',
            text: text_str,
        };
    
        let send_res = await emailModule.SendMail(mailOptions);
        console.log("send res is ", send_res)

        callback(null, { email:  call.request.email,
            error:const_module.Errors.Success
        }); 
        
 
    }catch(error){
        console.log("catch error is ", error)

        callback(null, { email:  call.request.email,
            error:const_module.Errors.Exception
        }); 
    }
     
}

function main() {
    //1.创建 gRPC 服务器实例，服务器负责接收和处理 RPC 请求
    var server = new grpc.Server()

    //2.注册服务实现
        //注册方法函数
    server.addService(message_proto.VarifyService.service, { GetVarifyCode: GetVarifyCode })
    server.bindAsync('0.0.0.0:50051', grpc.ServerCredentials.createInsecure(), () => {
        server.start()
        console.log('varify server started')        
    })
}

main()
//1.message_proto.VarifyService.service这样理解：这里是注册一个服务(VarifyService)下面的全部
//  方法(GetVarifyReq)，这里的配置文件只定义了一个方法

//2.GetVarifyCode: GetVarifyCode这样理解：“：”左边的我已将在proto文件定义好了该服务的具体名称
// ，不能更改，但是：右边的具体实现函数可以改，但是最好与该服务的名称保持一致。意思是当客户端
//调用GetVarifyCode(左)时，服务器就调用GetVarifyCode(右)的方法函数

//3.bindAsync中的三个参数(监听IP地址端口，通信是否加密，回调函数)，这是异步的

//4.service VarifyService {rpc GetVarifyCode (GetVarifyReq) returns (GetVarifyRsp) {}}
//这里定义了入参是GetVarifyReq，对应call；出参是GetVarifyRsp，对应callback
//所以call.request.只能拿到GetVarifyReq里面的字段