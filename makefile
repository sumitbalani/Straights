CXX = g++
CXXFLAGS = -g -Wall -MMD
OBJECTS = Card.o Command.o Deck.o Player.o ComputerPlayer.o HumanPlayer.o Game.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = straights
${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} main.cpp ${OBJECTS} -o ${EXEC}
clean :
	rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}
-include ${DEPENDS}