#include "CServer.h"
#include"HttpConnection.h"
#include"AsioIOServicePool.h"
CServer::CServer(boost::asio::io_context& ioc, unsigned short &port):_ioc(ioc), _acceptor(ioc, tcp::endpoint(tcp::v4(), port))
{
	std::cout << "服务器启动成功,监听端口号为"<<port<<"......\n";
}

void CServer::Start()
{
	auto self = shared_from_this();
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);
	_acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code ec) {
		try {//不能将try放在Start函数开始，为什么？P4
			//出错则放弃这个连接，继续监听新链接
			if (ec) {//想一想这里的错误和catch捕获的错误有什么区别？P4
				self->Start();
				return;
			}

			//处理新链接，创建HpptConnection类管理新连接
			new_con->Start();
			//继续监听
			self->Start();
		}
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
			self->Start();
		}
		});
}
