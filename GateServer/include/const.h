#pragma once

#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "ConfigMgr.h"
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

// ��ϣ���ɺ�����ʹ�� PBKDF2 �� SHA-256
inline std::function<std::string(std::string)> hashString = [](std::string input_str) {
    // ������ֵ
    std::string salt = "random_salt";
    
    // �ϲ������ַ�������ֵ
    std::string combined = input_str + salt;
    
    // ʹ�� std::hash �����й�ϣ
    std::hash<std::string> hash_fn;
    size_t hash_value = hash_fn(combined);

    // ����ϣֵת��Ϊʮ�������ַ���
    std::stringstream ss;
    ss << std::hex << hash_value;
    return ss.str();
};

// ��֤�����ַ����Ƿ���洢�Ĺ�ϣֵƥ��
inline std::function<bool(std::string, std::string)> verifyHash = [](std::string input_str, std::string hashed_str) {
    // ʹ����ͬ����ֵ���й�ϣ��֤
    std::string calculated_hash = hashString(input_str);
    return hashed_str == calculated_hash;
};

#define CODEPREFIX  "code_"