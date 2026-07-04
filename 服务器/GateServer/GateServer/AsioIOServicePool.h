#pragma once
#include <vector>
#include <boost/asio.hpp>
#include "Singleton.h"
class AsioIOServicePool :public Singleton<AsioIOServicePool>
{
	friend Singleton<AsioIOServicePool>;
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::executor_work_guard<IOService::executor_type>;//*10
	using WorkPtr = std::unique_ptr<Work>;//对Work的一层封装，封装为智能指针
	~AsioIOServicePool();
	AsioIOServicePool(const AsioIOServicePool&) = delete;//单例
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;//单例
	// 使用 round-robin 的方式返回一个 io_service
	boost::asio::io_context& GetIOService();
	void Stop();//手动停止
private:
	AsioIOServicePool(std::size_t size = 2/*std::thread::hardware_concurrency()*/);//*10  线程个数
	std::vector<IOService> _ioServices;  //装的是io_context

	//装的是指向work的unique指针
	std::vector<WorkPtr> _works;		
	std::vector<std::thread> _threads;  //装的是线程
	std::size_t _nextIOService;//轮询指针(索引)
};

