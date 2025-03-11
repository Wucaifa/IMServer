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
	Error_Json = 1001,  //Json解析错误
	RPCFailed = 1002,  //RPC请求错误
	VarifyExpired = 1003, //验证码过期
	VarifyCodeErr = 1004, //验证码错误
	UserExist = 1005,       //用户已经存在
	PasswdErr = 1006,    //密码错误
	EmailNotMatch = 1007,  //邮箱不匹配
	PasswdUpFailed = 1008,  //更新密码失败
	PasswdInvalid = 1009,   //密码更新失败
	TokenInvalid = 1010,   //Token失效
	UidInvalid = 1011,  //uid无效
};


// Defer类
class Defer {
public:
	// 接受一个lambda表达式或者函数指针
	Defer(std::function<void()> func) : func_(func) {}

	// 析构函数中执行传入的函数
	~Defer() {
		func_();
	}

private:
	std::function<void()> func_;
};

inline std::function<std::string(std::string)> hashString = [](std::string input_str) {
	// 使用 bcrypt 自动生成哈希及随机盐值
	return BCrypt::generateHash(input_str);
};
inline std::function<bool(std::string, std::string)> verifyHash = [](std::string input_str, std::string hashed_str) {
	// 验证输入字符串是否匹配已存储的哈希
	return BCrypt::validatePassword(input_str, hashed_str);
};

#define CODEPREFIX  "code_"