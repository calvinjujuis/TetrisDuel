CXX = g++
CXXFLAGS =-I/opt/X11/include -std=c++14 -Wall -g -MMD -Werror=vla
EXEC = biquadris
OBJECTS = block.o main.o baseblock.o cell.o controller.o decorator.o graphicsdisplay.o grid.o nextblock.o observer.o SpecialActions.o textdisplay.o window.o 
DEPENDS = ${OBJECTS:.o=.d}
SRC = ${OBJECTS:.o=.cc}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${SRC} -o ${EXEC} -lX11 -L/usr/X11/lib -I/usr/X11/include

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}

.PHONY: clean

