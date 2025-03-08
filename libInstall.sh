# 安装 Boost
sudo apt-get install -y libboost-all-dev

# 安装 JSON（一般 C++ 直接使用 nlohmann/json 头文件，无需安装）
sudo apt-get install -y nlohmann-json3-dev

# 安装 gRPC
sudo apt-get install -y protobuf-compiler grpc-dev

# 安装 Hiredis
sudo apt-get install -y libhiredis-dev

# 安装 MySQL Connector（C++ 版）
sudo apt-get install -y libmysqlcppconn-dev

# 安装 Node.js
curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash -
sudo apt-get install -y nodejs

# 安装 MySQL 服务器
sudo apt-get install -y mysql-server

# 安装 Redis 服务器
sudo apt-get install -y redis-server