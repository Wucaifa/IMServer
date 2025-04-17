#include "AsioIOServicePool.h"
#include <iostream>
using namespace std;
AsioIOServicePool::AsioIOServicePool(std::size_t size) : _ioServices(size), _works(size), _nextIOService(0) {
    // Ϊÿ�� io_service ����һ�� WorkGuard
    for (std::size_t i = 0; i < size; ++i) {
        _works[i] = std::make_unique<WorkGuard>(_ioServices[i].get_executor());
    }

    // ������� io_service����������̣߳�ÿ���߳��ڲ����� io_service
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
    // ֹͣ���е� io_context
    for (auto& work : _works) {
        work.reset();  // �ͷ� WorkGuard ��Դ
    }

    // ֹͣÿ�� io_context
    for (auto& io_service : _ioServices) {
        io_service.stop();
    }

    // �ȴ������߳����
    for (auto& t : _threads) {
        t.join();
    }
}
