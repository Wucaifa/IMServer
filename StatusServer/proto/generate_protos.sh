#!/bin/bash

# 如果没有设置 VCPKG_ROOT 环境变量，则默认使用 /home/wu/vcpkg
VCPKG_ROOT=${VCPKG_ROOT:-/home/wu/vcpkg}

# 使用 vcpkg 安装的 protoc 和 grpc_cpp_plugin 的路径
PROTOC_PATH="$VCPKG_ROOT/installed/x64-linux/tools/protobuf/protoc"
GRPC_CPP_PLUGIN_PATH="$VCPKG_ROOT/installed/x64-linux/tools/grpc/grpc_cpp_plugin"

# 检查是否存在并具有可执行权限
if [ ! -x "$PROTOC_PATH" ]; then
  echo "Error: protoc is not installed in vcpkg path: $PROTOC_PATH"
  exit 1
fi

if [ ! -x "$GRPC_CPP_PLUGIN_PATH" ]; then
  echo "Error: grpc_cpp_plugin is not installed in vcpkg path: $GRPC_CPP_PLUGIN_PATH"
  exit 1
fi

# 指定proto文件的路径和输出目录（相对于当前脚本目录）
PROTO_DIR="./proto"
OUT_DIR="./generated"

# 创建生成目录
mkdir -p "$OUT_DIR"

# 生成 C++ 文件（包括 protobuf 和 gRPC 代码）
for proto_file in "$PROTO_DIR"/*.proto; do
  echo "Processing $proto_file..."
  
  # 生成 Protobuf 的 C++ 文件
  "$PROTOC_PATH" --cpp_out="$OUT_DIR" --proto_path="$PROTO_DIR" "$proto_file"
  
  # 生成 gRPC 的 C++ 文件
  "$PROTOC_PATH" --grpc_out="$OUT_DIR" --plugin=protoc-gen-grpc="$GRPC_CPP_PLUGIN_PATH" --proto_path="$PROTO_DIR" "$proto_file"
done

echo "Proto files have been generated in $OUT_DIR"
