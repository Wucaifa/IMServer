#include "const.h"
#include "CServer.h"
#include <iostream>
#include "RedisMgr.h"
#include "MysqlMgr.h"

int main()
{
	try
	{
		MysqlMgr::GetInstance();
		RedisMgr::GetInstance();
		auto& gCfgMgr = ConfigMgr::Inst();
		std::string gate_port_str = gCfgMgr["GateServer"]["Port"];
		unsigned short gate_port = atoi(gate_port_str.c_str());
		net::io_context ioc{ 1 };
		boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
		signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {

			if (error) {
				return;
			}
			ioc.stop();
		});
		std::make_shared<CServer>(ioc, gate_port)->Start();
		std::cout << "Gate Server listen on port: " << gate_port << std::endl;
		ioc.run();
		RedisMgr::GetInstance()->Close();
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		RedisMgr::GetInstance()->Close();
		return EXIT_FAILURE;
	}
}

//启动服务器，在浏览器输入`http://localhost:8080/get_test`
//
//会看到服务器回包`receive get_test req`
//
//如果我们输入带参数的url请求`http://localhost:8080/get_test?key1=value1&key2=value2`
//
//会收到服务器反馈`url not found`
//
//所以对于get请求带参数的情况我们要实现参数解析，我们可以自己实现简单的url解析函数



