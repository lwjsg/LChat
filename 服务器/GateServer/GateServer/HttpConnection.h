#pragma once
#include"const.h"
class HttpConnection:public std::enable_shared_from_this<HttpConnection>
{
	friend class LogicSystem;
public:
	HttpConnection(boost::asio::io_context& ioc);
	void Start();
	tcp::socket& GetSocket()
	{
		return _socket;
	}
private:
	void CheckDeadline();//超时检查（防止客户端一直不发数据、卡死连接）如果 60 秒没数据，就关闭这个连接
	void WriteResponse();//构造响应并发送给客户端
	void HandleReq();//读取并解析 HTTP 请求
	void PreParseGetParam();
	//用来完整保存客户端（浏览器 / Qt 前端）发过来的整个 HTTP 请求。
	http::request<http::dynamic_body> _request;

	// HTTP 响应对象1.你要返回给客户端的数据：2.状态码 200 / 404 / 500 3.响应头4.响应体（你的 JSON 数据）
	http::response<http::dynamic_body> _response;

	tcp::socket  _socket;

	//定时器：创建对象时，定时器创建并开始倒计时
	//_socket.get_executor():与当前socket绑定在一个IO线程，倒计时60秒
	net::steady_timer deadline_{
		_socket.get_executor(), std::chrono::seconds(60) };

	beast::flat_buffer  _buffer{ 8192 };//为什么不能写成 _buffer[8192]?P4 


	// “/get_test?key1=value1&key2=value2”
	std::string _get_url;//记录路径：/get_test
	std::unordered_map<std::string, std::string> _get_params;//记录查询参数：key1=value1&key2=value2
};

