all : main.o HTGame.o HTFrame.o HTCards.o
	g++ -g main.o HTGame.o HTFrame.o HTCards.o -o HeroTales -O2 -s -flto
main.o : src/main.cpp
	g++ -g -c -I include src/main.cpp
HTGame.o : src/HTGame.cpp
	g++ -g -c -I include src/HTGame.cpp
HTCards.o : src/HTCards.cpp
	g++ -g -c -I include src/HTCards.cpp
HTFrame.o : src/HTFrame.cpp
	g++ -g -c -I include src/HTFrame.cpp

clean:
	del *.o
