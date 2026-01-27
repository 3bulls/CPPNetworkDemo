# 编译器设置
CXX = g++
# 编译选项: 
# -std=c++17: 使用 C++17 标准
# -Wall -g: 显示警告和调试信息
# -I../TCPDemo: 告诉编译器去隔壁 TCPDemo 目录找头文件 (.hpp)
CXXFLAGS = -std=c++17 -Wall -g -I../TCPDemo

# 最终生成的可执行文件名字 (通常用含有 main 函数的文件名命名)
TARGET = httpTest

# 需要的目标文件列表
# 1. 当前目录的测试主程序
# 2. 当前目录的 HttpClient 实现
# 3. 隔壁目录的 TCPClient 实现
OBJS = httpTest.o httpClient.o ../TCPDemo/TCPClient.o

# ----------------------------------------------------

# 默认目标
all: $(TARGET)

# 链接步骤
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# ----------------------------------------------------
# 编译规则

# 1. 编译 httpTest.o (包含 main 函数)
httpTest.o: httpTest.cpp httpClient.hpp ../TCPDemo/utils.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 2. 编译 httpClient.o
httpClient.o: httpClient.cpp httpClient.hpp ../TCPDemo/TCPClient.hpp ../TCPDemo/utils.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 3. 编译隔壁的 TCPClient.o
# 注意：这里会生成到 ../TCPDemo/ 目录下，而不是当前目录
../TCPDemo/TCPClient.o: ../TCPDemo/TCPClient.cpp ../TCPDemo/TCPClient.hpp ../TCPDemo/utils.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ----------------------------------------------------
# 清理规则
clean:
	rm -f *.o $(TARGET)
	# 如果你也想清理隔壁的 TCPClient.o，可以取消下面这行的注释
	# rm -f ../TCPDemo/TCPClient.o

.PHONY: all clean