# 编译器设置
CXX = g++
# 编译选项: C++17标准 (也可改为c++11), 显示所有警告, 生成调试信息
CXXFLAGS = -std=c++17 -Wall -g

# 定义目标可执行文件的名字
TARGET_SERVER = server
TARGET_CLIENT = client

# 定义 Server 需要的目标文件 (.o)
# 对应 ServerMain.cpp 和 UDPServer.cpp
OBJS_SERVER = UDPServerTest.o UDPServer.o

# 定义 Client 需要的目标文件 (.o)
# 对应 ClientMain.cpp 和 UDPClient.cpp
OBJS_CLIENT = UDPClientTest.o UDPClient.o
# ----------------------------------------------------

# 默认目标：输入 'make' 时执行，同时编译两个程序
all: $(TARGET_SERVER) $(TARGET_CLIENT)

# 链接 Server
$(TARGET_SERVER): $(OBJS_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 链接 Client
$(TARGET_CLIENT): $(OBJS_CLIENT)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 通用编译规则：将 .cpp 编译为 .o
# $< 代表依赖项(source.cpp), $@ 代表目标(target.o)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ----------------------------------------------------
# 依赖关系补充 (如果修改了 utils.hpp，所有相关文件都会重新编译)
ServerMain.o: utils.hpp
TCPServer.o: utils.hpp
ClientMain.o: utils.hpp
TCPClient.o: utils.hpp

# ----------------------------------------------------
# 清理规则：输入 'make clean' 时执行
# 删除所有 .o 文件和可执行程序
clean:
	rm -f *.o $(TARGET_SERVER) $(TARGET_CLIENT)

# 声明伪目标，防止文件夹里有叫 all 或 clean 的文件导致冲突
.PHONY: all clean