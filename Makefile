worm.x:  main.o getChar.o PlayingArea.o
	g++ -ggdb -std=c++11 -o worm.x main.o getChar.o PlayingArea.o -lcurses -ltermcap

main.o: main.cpp
	g++ -c -ggdb -std=c++11 main.cpp -o main.o

PlayingArea.o: PlayingArea.cpp PlayingArea.hpp
	g++ -c -ggdb -std=c++11 PlayingArea.cpp -o PlayingArea.o

getChar.o: getChar.cpp 
	gcc -c -ggdb -std=c++11 getChar.cpp -o getChar.o

clean:
	/bin/rm -f screen.x screen.o getChar.o main.o PlayingArea.o *~
