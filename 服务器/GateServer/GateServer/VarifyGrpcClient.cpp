#include "VarifyGrpcClient.h"
#include"ConfigMgr.h"
RPConPool::RPConPool(size_t poolSize, std::string host, std::string port)
	: poolSize_(poolSize), host_(host), port_(port), b_stop_(false) {
	for (size_t i = 0; i < poolSize_; ++i) {

		// 创建gRPC通道：连接本地50051端口（服务端地址）
		std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port,
			grpc::InsecureChannelCredentials());
		// 创建gRPC客户端存根（stub），用于发起服务调用
		connections_.push(VarifyService::NewStub(channel));//connections_存放的是unique_ptr为什么能使用.push函数?P10
	}
}

void RPConPool::Close() {
	b_stop_ = true;
	cond_.notify_all();
}

RPConPool::~RPConPool() {
	std::lock_guard<std::mutex> lock(mutex_);
	Close();
	while (!connections_.empty()) {
		connections_.pop();
	}
}

std::unique_ptr<VarifyService::Stub> RPConPool::getConnection() {
	std::unique_lock<std::mutex> lock(mutex_);
	cond_.wait(lock, [this] {
		if (b_stop_) {
			return true;
		}
		return !connections_.empty();
		});
	//如果停止则直接返回空指针
	if (b_stop_) {
		return  nullptr;
	}
	auto context = std::move(connections_.front());
	connections_.pop();
	return context;
}

void RPConPool::returnConnection(std::unique_ptr<VarifyService::Stub> context) {
	std::lock_guard<std::mutex> lock(mutex_);
	if (b_stop_) {
		return;
	}
	connections_.push(std::move(context));
	cond_.notify_one();
}



//封装grpc发送请求，并接受返回的响应
GetVarifyRsp VerifyGrpcClient::GetVarifyCode(std::string email) {
	ClientContext context;  // gRPC请求上下文
	GetVarifyRsp reply;     // 响应对象
	GetVarifyReq request;   // 请求对象
	request.set_email(email);  // 给请求设置邮箱参数

	auto stub = pool_->getConnection();

	//真正向 gRPC 服务端发送请求，参数三需要的是指针
	//这里的GetVarifyCode与函数虽是同一个名字，但作用完全不同
	Status status = stub->GetVarifyCode(&context, request, &reply);
	//status代表的状态：RPC 调用本身成功了吗？（网络通不通？服务端挂没挂？）
	//reply代表的状态：业务逻辑成功了吗？（邮箱是否有效？验证码是否发送成功？）
	if (status.ok()) {
		pool_->returnConnection(std::move(stub));
		return reply;
	}
	else {
		pool_->returnConnection(std::move(stub));
		reply.set_error(ErrorCodes::RPCFailed);
		return reply;
	}
}

VerifyGrpcClient::VerifyGrpcClient() {
	auto& gCfgMgr = ConfigMgr::Inst();
	std::string host = gCfgMgr["VarifyServer"]["Host"];
	std::string port = gCfgMgr["VarifyServer"]["Port"];
	pool_.reset(new RPConPool(5, host, port));
}