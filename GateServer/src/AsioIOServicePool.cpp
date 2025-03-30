#include "AsioIOServicePool.h"
#include <iostream>
using namespace std;
AsioIOServicePool::AsioIOServicePool(std::size_t size) : _ioServices(size), _works(size), _nextIOService(0) {
    // 为每个 io_service 创建一个 WorkGuard
    for (std::size_t i = 0; i < size; ++i) {
        _works[i] = std::make_unique<WorkGuard>(_ioServices[i].get_executor());
    }

    // 遍历多个 io_service，创建多个线程，每个线程内部启动 io_service
    for (std::size_t i = 0; i < _ioServices.size(); ++i) {
        _threads.emplace_back([this, i]() {
            _ioServices[i].run();
        });
    }
}

AsioIOServicePool::~AsioIOServicePool() {
	Stop();
	std::cout << "AsioIOServicePool destruct" << endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
	auto& service = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size()) {
		_nextIOService = 0;
	}
	return service;
}

void AsioIOServicePool::Stop() {
    // 停止所有的 io_context
    for (auto& work : _works) {
        work.reset();  // 释放 WorkGuard 资源
    }

    // 停止每个 io_context
    for (auto& io_service : _ioServices) {
        io_service.stop();
    }

    // 等待所有线程完成
    for (auto& t : _threads) {
        t.join();
    }
}
