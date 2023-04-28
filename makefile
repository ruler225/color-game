LDFLAGS= -lGL -lGLU -lglut -lfreeimage
CXXFLAGS= -g -Wall -std=c++17
CXX=g++
EXEEXT=
RM=rm -f

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXEEXT=.exe #on windows applications must have .exe extension
	RM=del #rm command for windows powershell
	LDFLAGS = -lfreeglut -lglu32 -lopengl32
else
# OS X
	OS := $(shell uname)
	ifeq ($(OS), Darwin)
					LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT -lfreeimage
	endif
endif

HEADERS := $(patsubst %.hpp, src/header/%.hpp ,$(notdir $(wildcard src/header/*.hpp)))
SOURCES := $(notdir $(wildcard src/*.cpp))
OBJECTS := $(SOURCES:%.cpp=src/%.o)

default: $(OBJECTS) $(HEADERS)
	$(CXX) -o game $(OBJECTS) $(CXXFLAGS) $(LDFLAGS)

run: default
	./game$(EXEEXT)

debug: default
	gdb ./game

clean:
	find . -type f -iname "*.o" -exec rm {} \;
	$(RM) game
