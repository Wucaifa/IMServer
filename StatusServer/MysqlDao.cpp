#include "MysqlDao.h"
#include "ConfigMgr.h"
#include "const.h"

MysqlDao::MysqlDao()
{
	auto& cfg = ConfigMgr::Inst();
	const auto& host = cfg["Mysql"]["Host"];
	const auto& port = cfg["Mysql"]["Port"];
	const auto& pwd = cfg["Mysql"]["Passwd"];
	const auto& schema = cfg["Mysql"]["Schema"];
	const auto& user = cfg["Mysql"]["User"];
	pool_.reset(new MySqlPool(host + ":" + port, user, pwd, schema, 5));
}

MysqlDao::~MysqlDao() {
	pool_->Close();
}

int MysqlDao::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
	auto con = pool_->getConnection();
	try {
		if (con == nullptr) {
			pool_->returnConnection(std::move(con));
			return false;
		}
		// ׼�����ô洢����
		unique_ptr < sql::PreparedStatement > stmt(con->prepareStatement("CALL reg_user(?,?,?,@result,@error_message)"));
		// �����������
		stmt->setString(1, name);
		stmt->setString(2, email);
		stmt->setString(3, pwd);

		// ����PreparedStatement��ֱ��֧��ע�����������������Ҫʹ�ûỰ������������������ȡ���������ֵ

		  // ִ�д洢����
		stmt->execute();
		// ����洢���������˻Ự��������������ʽ��ȡ���������ֵ�������������ִ��SELECT��ѯ����ȡ����
	   // ���磬����洢����������һ���Ự����@result���洢������������������ȡ��
		unique_ptr<sql::Statement> stmtResult(con->createStatement());
		unique_ptr<sql::ResultSet> res(stmtResult->executeQuery("SELECT @result AS result, @error_message AS error_message"));
		if (res->next()) {
			int result = res->getInt("result");
			string error_message = res->getString("error_message");
			cout << "Result: " << result << endl;
			cout << "Error_message��" << error_message << endl;
			pool_->returnConnection(std::move(con));
			return result;
		}
		pool_->returnConnection(std::move(con));
		return -1;
	}
	catch (sql::SQLException& e) {
		pool_->returnConnection(std::move(con));
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return -1;
	}
}

bool MysqlDao::CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userInfo) {
	auto con = pool_->getConnection();
	Defer defer([this, &con]() {
		pool_->returnConnection(std::move(con));
		});

	try {
		if (con == nullptr) {
			return false;
		}

		// ׼��SQL���
		std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("SELECT * FROM user WHERE name = ?"));
		pstmt->setString(1, name); // ��username�滻Ϊ��Ҫ��ѯ���û���

		// ִ�в�ѯ
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
		std::string origin_pwd = "";
		// ���������
		while (res->next()) {
			origin_pwd = res->getString("pwd");
			// �����ѯ��������
			std::cout << "Password: " << origin_pwd << std::endl;
			break;
		}

		if (pwd != origin_pwd) {
			return false;
		}
		userInfo.name = name;
		userInfo.email = res->getString("email");
		userInfo.uid = res->getInt("uid");
		userInfo.pwd = origin_pwd;
		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}