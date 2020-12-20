#
# Created by Andy on 12/20/2020.
#

all: basimcxx

basimcxx: SandBox.o PlayerDefender.o GameMap.o Item.o
	g++ -Wall -g -std=c++11 -o basimcxx SandBox.o PlayerDefender.o GameMap.o Item.o

SandBox.o: SandBox.cpp PlayerDefender.h GameMap.h Item.h
	g++ -Wall -g -std=c++11 -c SandBox.cpp

PlayerDefender.o: PlayerDefender.cpp PlayerDefender.h Player.h GameMap.h Item.h
	g++ -Wall -g -std=c++11 -c PlayerDefender.cpp

GameMap.o: GameMap.cpp GameMap.h Item.h
	g++ -Wall -g -std=c++11 -c GameMap.cpp

Item.o: Item.cpp Item.h
	g++ -Wall -g -std=c++11 -c Item.cpp

clean:
	rm -rf basimcxx *.o *~ basimcxx.dSYM