const path = require('path')
const grpc = require('@grpc/grpc-js')
const protoLoader = require('@grpc/proto-loader')

const PROTO_PATH = path.join(__dirname, 'message.proto')//找到message.proto的路径

//解析 proto 文件
const packageDefinition = protoLoader.loadSync(PROTO_PATH, { keepCase: true, longs: String, enums: String, defaults: true, oneofs: true })

// 生成 gRPC 对象
const protoDescriptor = grpc.loadPackageDefinition(packageDefinition)

//提取具体服务
const message_proto = protoDescriptor.message

module.exports = message_proto