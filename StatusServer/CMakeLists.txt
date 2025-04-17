cmake_minimum_required(VERSION 3.14)
project(StatusServer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF) 

add_definitions(-DGLIBCXX_USE_CXX11_ABI=0)

# 设置可执行文件输出目录为 bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# === 设置 vcpkg 工具链 ===
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "/home/wu/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file" FORCE)
endif()

# === 让 CMake 直接查找 vcpkg 的安装路径 ===
set(CMAKE_PREFIX_PATH "/home/wu/vcpkg/installed/x64-linux" CACHE STRING "vcpkg installed packages" FORCE)

# 将资源文件拷贝到 bin 目录下
file(COPY ${CMAKE_SOURCE_DIR}/res/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)

# === 确保 CMake 能找到 JSONCPP、hiredis、mysqlcppconn ===
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS filesystem system)
find_package(jsoncpp CONFIG REQUIRED)
find_package(hiredis CONFIG REQUIRED)
find_package(cryptopp CONFIG REQUIRED)

# **MySQL 连接库**
set(MySQL_ROOT "/home/wu/mysql-connector/usr")
find_library(MySQLConnector_LIB NAMES mysqlcppconn8-static PATHS "${MySQL_ROOT}/lib/x86_64-linux-gnu")

# === 生成 proto 文件 ===
execute_process(
    COMMAND bash ${CMAKE_SOURCE_DIR}/proto/generate_protos.sh
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# === 包含路径 ===
include_directories(
    ${CMAKE_SOURCE_DIR}/include  # 项目 include 目录
    ${CMAKE_SOURCE_DIR}/generated  # Protobuf 生成的文件路径
    ${Boost_INCLUDE_DIRS}
    ${Protobuf_INCLUDE_DIRS}
    ${gRPC_INCLUDE_DIRS}
    ${hiredis_INCLUDE_DIRS}
    ${jsoncpp_INCLUDE_DIRS}
    ${cryptopp_INCLUDE_DIRS}
    ${MySQL_ROOT}/include/mysql-cppconn-8
)

# === client target ===
add_executable(StatusServer
    generated/message.pb.cc
    generated/message.grpc.pb.cc
    src/AsioIOServicePool.cpp
    src/ChatGrpcClient.cpp
    src/ConfigMgr.cpp
    src/MysqlDao.cpp
    src/MysqlMgr.cpp
    src/RedisMgr.cpp
    src/StatusServiceImpl.cpp
    src/main.cpp
)

# === 目标链接库 ===
target_link_libraries(StatusServer
    PRIVATE
    protobuf::libprotobuf
    gRPC::grpc++
    Boost::boost
    Boost::filesystem
    Boost::system
    jsoncpp_static
    /home/wu/vcpkg/installed/x64-linux/lib/libhiredis.a
    /home/wu/mysql-connector/usr/lib/x86_64-linux-gnu/libmysqlcppconn-static.a  
    resolv
)


