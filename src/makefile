#compiler
CXX = clang++

#all the library info
SDL = -framework SDL2
OPENGL = -framework OpenGL
GLEWPATH = /usr/local/Cellar/glew/1.11.0
GLEW = -L$(GLEWPATH)/lib -lGLEW
GLMPATH = /usr/local/Cellar/glm/0.9.5.4
INC = -I$(GLEWPATH)/include -I$(GLMPATH)/include

#flags
# If your compiler is a bit older you may need to change -std=c++11 to -std=c++0x
CXXFLAGS = $(INC) -Wall -c -std=c++11 -F/Library/Frameworks
LDFLAGS = $(INC) $(SDL) $(OPENGL) $(GLEW) -F/Library/Frameworks

#executable name, plus method for finding all the .cpp and .o files
EXE = FishOO.app
CPP_FILES = $(wildcard *.cpp)
OBJ_FILES = $(addprefix ../obj/,$(notdir $(CPP_FILES:.cpp=.o)))

all: $(EXE) run

$(EXE): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o ../bin/$@ $^

../obj/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

.PHONY : clean
clean:
	rm ../obj/*.o && rm ../bin/$(EXE)

.PHONY : run
run: $(EXE)
	../bin/$(EXE)