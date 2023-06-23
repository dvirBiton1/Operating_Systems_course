.PHONY: all clean
CXX=g++

all: server client test
server: Ex4.hpp server.o 
	$(CXX) server.o -o server -lpthread -ltbb
client: client.o Ex4.hpp
	$(CXX) client.o -o client -lpthread -ltbb
test: test.o Ex4.hpp
	$(CXX) test.o -o test -lpthread -ltbb
server.o: server.cpp Ex4.hpp
	$(CXX) -c server.cpp 
client.o: client.cpp Ex4.hpp
	$(CXX) -c client.cpp
test.o: test.cpp Ex4.hpp
	$(CXX) -c test.cpp

clean :
	rm -f *.o client server test