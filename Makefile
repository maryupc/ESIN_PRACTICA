OPCIONS = -Wall -Wextra -Wno-sign-compare -std=c++11 -ansi -g

main.exe: main.o phone.o
	g++ -o main.exe main.o phone.o -lesin

main.o: main.cpp phone.hpp
	g++ -c main.cpp $(OPCIONS)

phone.o: phone.cpp phone.hpp phone.rep
	g++ -c phone.cpp $(OPCIONS)

clean:
	rm *.o
	rm *.exe
	rm *.gch