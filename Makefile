main:IniFile.o Logger.o  System.o main.o
	g++ IniFile.o  Logger.o System.o main.o -std=c++11 -o main

IniFile.o:./utility/IniFile.cpp
	g++ -c ./utility/IniFile.cpp

Logger.o:./utility/Logger.cpp
	g++ -c ./utility/Logger.cpp

System.o:./utility/System.cpp
	g++ -c ./utility/System.cpp

main.o:main.cpp
	g++ -c main.cpp

clean:
	rm *.o