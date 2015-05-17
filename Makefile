CC = g++
CFLAGS = -g -O3 -Wall -std=c++11
CPPLIBS = -lSDL2 -lSDL2_image 

CPPFILES = $(wildcard **/*.cpp)
CPPFILES = $(shell find src/ -type f -name '*.cpp')
OBJFILES = $(patsubst src/%.cpp, obj/%.o, $(CPPFILES))
DEPS = 

all: $(OBJFILES)
	$(CC) $(CFLAGS) -o jumpinjack $(OBJFILES) $(CPPLIBS)

obj/%.o: src/%.cpp $(DEPS)
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c -o $@ $< 

clean:
	rm -rf obj
