#pragma once
#include "Singleton.h"
#include "const.h"
//逻辑类

//声明类，建议在.pp文件中包含目标类的头文件，如果目标类也会用到此类并在.h互相包含对方的.h文件会出现互引用的问题，解决方案就是在.cpp里包含
class HttpConnection;

typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;
class LogicSystem :public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem() {};
	bool HandleGet(std::string, std::shared_ptr<HttpConnection>);
	bool HandlePost(std::string, std::shared_ptr<HttpConnection>);
	void RegGet(std::string, HttpHandler handler);
	void RegPost(std::string, HttpHandler handler);
private:
	LogicSystem();
	std::map<std::string, HttpHandler> _post_handlers;
	std::map<std::string, HttpHandler> _get_handlers;
};
