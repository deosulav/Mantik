build:
	clang++ -g3 *.cpp src/imgui/*.cpp src/imnodes/*.cpp -Iinclude -Iinclude/imgui -Iinclude/imnodes -lSDL2 -lGL -lGLU -lGLEW -o bin/ProteusClone
