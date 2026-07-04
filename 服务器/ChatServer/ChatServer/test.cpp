#include "AsioIOServicePool.h"
#include <iostream>
using namespace std;

AsioIOServicePool::AsioIOServicePool(std::size_t num) :_ioServices(num), _works(num), _nextIOService(0)
{
	for (int i = 0; i < num; i++)
	{
		_works[i] = std::make_unique<Work>(_ioServices[i].get_executor());
	}
	for (int i = 0; i < num; i++)
	{
		_threads.emplace_back([this,i]() {
			_ioServices[i].run();
			});
	}
}

