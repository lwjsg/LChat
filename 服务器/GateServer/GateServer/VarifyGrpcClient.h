#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"
using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;


class RPConPool {
public:
	RPConPool(size_t poolSize, std::string host, std::string port);

	~RPConPool();

	void returnConnection(std::unique_ptr<VarifyService::Stub> context);

	void Close();

	std::unique_ptr<VarifyService::Stub> getConnection();
private:
	std::atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;
	std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
	std::mutex mutex_;
	std::condition_variable cond_;
};


class VerifyGrpcClient :public Singleton<VerifyGrpcClient>
{
	friend class Singleton<VerifyGrpcClient>;
public:

	//封装grpc发送请求，并接受返回的响应
	GetVarifyRsp GetVarifyCode(std::string email);
private:
	VerifyGrpcClient();

	std::unique_ptr<VarifyService::Stub> stub_;//相当于客户端与服务端通信的信使

	std::unique_ptr<RPConPool> pool_;
};

