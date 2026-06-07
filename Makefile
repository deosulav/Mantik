CXXFLAGS = -g3 -std=c++17 -Wno-nontrivial-memcall  
INCLUDES = -Iinclude -Ideps
SOURCES = deps/imgui/*.cpp deps/imnodes/*.cpp src/*.cpp
LIBS = 
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin) #APPLE
	LIBS += -Ldeps/SDL3/libs/mac
endif

LIBS += -lSDL3

build:
	mkdir -p build && clang++ $(CXXFLAGS) $(SOURCES)\
	 $(INCLUDES) $(LIBS) -o build/Mantik

clean:
	rm -rf build