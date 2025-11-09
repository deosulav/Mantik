build:
	clang++ -g3 -std=c++17 -Wno-nontrivial-memcall deps/imgui/*.cpp deps/imnodes/*.cpp src/*.cpp -Iinclude -Ideps -Ideps -lSDL2 -lGL -lGLU -lGLEW -o build/Mantik

clean:
	rm -f build/Mantik
