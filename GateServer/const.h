#pragma once

#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "ConfigMgr.h"
#include <bcrypt/BCrypt.hpp>
#include <functional>

class ConfigMgr;
extern ConfigMgr gCfgMgr;

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,  //Json��������
	RPCFailed = 1002,  //RPC�������
	VarifyExpired = 1003, //��֤�����
	VarifyCodeErr = 1004, //��֤�����
	UserExist = 1005,       //�û��Ѿ�����
	PasswdErr = 1006,    //�������
	EmailNotMatch = 1007,  //���䲻ƥ��
	PasswdUpFailed = 1008,  //��������ʧ��
	PasswdInvalid = 1009,   //�������ʧ��
	TokenInvalid = 1010,   //TokenʧЧ
	UidInvalid = 1011,  //uid��Ч
};


// Defer��
class Defer {
public:
	// ����һ��lambda���ʽ���ߺ���ָ��
	Defer(std::function<void()> func) : func_(func) {}

	// ����������ִ�д���ĺ���
	~Defer() {
		func_();
	}

private:
	std::function<void()> func_;
};

inline std::function<std::string(std::string)> hashString = [](std::string input_str) {
	// ʹ�� bcrypt �Զ����ɹ�ϣ�������ֵ
	return BCrypt::generateHash(input_str);
};
inline std::function<bool(std::string, std::string)> verifyHash = [](std::string input_str, std::string hashed_str) {
	// ��֤�����ַ����Ƿ�ƥ���Ѵ洢�Ĺ�ϣ
	return BCrypt::validatePassword(input_str, hashed_str);
};

#define CODEPREFIX  "code_"