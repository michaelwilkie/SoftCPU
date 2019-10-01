all:
	g++ -std=c++11 -o main ALU.cpp CPU.cpp MEM.cpp RF.cpp Register.cpp main.cpp
