CC = g++

INSTALLDIR = $(PWD)
RESOURCESDIR = $(INSTALLDIR)/data

SDLLIBS=`sdl2-config --libs`
SDLLIBS=-Wl,-Bstatic `sdl2-config --static-libs` -Wl,-Bdynamic

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

CFLAGS = -g -O3 -Wall -std=c++11 -DRESOURCES_DIR=\"$(RESOURCESDIR)\" $(SDL_CFLAGS)
CPPLIBS = $(SDL_LDFLAGS) -lSDL2_image -lSDL2_ttf -lSDL2_mixer 

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
