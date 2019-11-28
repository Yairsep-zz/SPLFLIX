all: splflix


splflix:bin/main.o bin/Session.o bin/Watchable.o bin/User.o bin/Action.o
	g++ -o  bin/splflix bin/main.o  bin/Session.o   bin/Watchable.o bin/User.o bin/Action.o


bin/main.o:src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp
	

bin/User.o:src/User.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/User.o src/User.cpp


bin/Session.o:src/Session.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Session.o src/Session.cpp


bin/Watchable.o:src/Watchable.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Watchable.o src/Watchable.cpp


bin/Action.o:src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

clean:
	rm -f bin/*
