#include "AsioIOServicePool.h"
#include <iostream>
using namespace std;
AsioIOServicePool::AsioIOServicePool(std::size_t size) :_ioServices(size),
_works(size), _nextIOService(0) //为什么不初始化vector<thread>容器的个数//*10
{
	//绑定对应的work
	for (std::size_t i = 0; i < size; ++i) 
	{
		_works[i] = std::make_unique<Work>(_ioServices[i].get_executor());//_ioServices是否为空？P10
	}

	//遍历多个ioservice，创建多个线程，并且每个线程内部启动ioservice(创建即启动)
	for (std::size_t i = 0; i < _ioServices.size(); ++i) 
	{
		_threads.emplace_back([this, i]() 
		{
			_ioServices[i].run();
		});
	}
}

AsioIOServicePool::~AsioIOServicePool() 
{
	Stop();
	std::cout << "AsioIOServicePool destruct" << endl;
}

//索引轮询设置
boost::asio::io_context& AsioIOServicePool::GetIOService()
{
	auto& service = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size()) 
	{
		_nextIOService = 0;
	}
	return service;
}

void AsioIOServicePool::Stop() 
{
	//因为仅仅执行work.reset并不能让iocontext从run的状态中退出
	//当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务。
	// 停止所有 io_context
	for (auto& io : _ioServices) 
	{
		io.stop();
	}

	// 释放 work guard
	for (auto& work : _works)
	{
		work.reset();
	}

	// 等待线程结束
	for (auto& t : _threads) 
	{
		t.join();
	}
}