# 编译器设置
CXX = g++
# 编译选项: C++17标准, 显示警告, 生成调试信息
CXXFLAGS = -std=c++17 -Wall -g

# 定义最终生成的可执行文件名
TARGET_SERVER = server
TARGET_CLIENT = client

# 定义 Server 需要的目标文件
OBJS_SERVER = ServerMain.o TCPServer.o

# 定义 Client 需要的目标文件
OBJS_CLIENT = ClientMain.o TCPClient.o

# ----------------------------------------------------

# 默认目标：输入 'make' 时执行，同时编译两个程序
all: $(TARGET_SERVER) $(TARGET_CLIENT)

# 1. 链接 Server
$(TARGET_SERVER): $(OBJS_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 2. 链接 Client
$(TARGET_CLIENT): $(OBJS_CLIENT)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 3. 通用编译规则：将 .cpp 编译为 .o
# $< 代表源文件, $@ 代表目标文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ----------------------------------------------------
# 4. 头文件依赖关系 (关键部分)
# 当 utils.hpp 或 对应的 .hpp 修改时，重新编译相关文件

# Server 部分依赖
ServerMain.o: ServerMain.cpp TCPServer.hpp utils.hpp
TCPServer.o: TCPServer.cpp TCPServer.hpp utils.hpp

# Client 部分依赖
ClientMain.o: ClientMain.cpp TCPClient.hpp utils.hpp
TCPClient.o: TCPClient.cpp TCPClient.hpp utils.hpp

# ----------------------------------------------------
# 清理规则
clean:
	rm -f *.o $(TARGET_SERVER) $(TARGET_CLIENT)

# 伪目标防止文件名冲突
.PHONY: all clean