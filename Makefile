CC = g++

INSTALLDIR = $(PWD)
RESOURCESDIR = $(INSTALLDIR)/data

CFLAGS = -g -O3 -Wall -std=c++11 -DRESOURCES_DIR=\"$(RESOURCESDIR)\"
CPPLIBS = -lSDL2 -lSDL2_image -lSDL2_ttf

CPPFILES = $(wildcard **/*.cpp)
CPPFILES = $(shell find src/ -type f -name '*.cpp')
OBJFILES = $(patsubst src/%.cpp, obj/%.o, $(CPPFILES))
DEPS = 

all: $(OBJFILES)
	$(CC) $(CFLAGS) -o jumpinjack $(OBJFILES) $(CPPLIBS)
	@echo $(INSTALLDIR)

obj/%.o: src/%.cpp $(DEPS)
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c -o $@ $< 

clean:
	rm -rf obj
