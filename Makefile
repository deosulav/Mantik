build:
	clang++ -g3 -std=c++17 main.cpp src/imgui/*.cpp src/imnodes/*.cpp src/*.cpp -Iinclude -Iinclude/imgui -Iinclude/imnodes -lSDL2 -lGL -lGLU -lGLEW -o bin/ProteusClone

ex_src=/home/rls/Documents/Projects/ProteusCloneSDL/dependencies/imnodes/example/

build-example:
	clang++ ${ex_src}/main.cpp ${ex_src}/save_load.cpp src/imgui/*.cpp src/imnodes/*.cpp -Iinclude -Iinclude/imgui -Iinclude/imnodes -lSDL2 -lGL -lGLU -lGLEW -o bin/ProteusClone
