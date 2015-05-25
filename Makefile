EXEC = camera1
all: Utils.o VectorUtil.o Terrain.o Clip.o Light.o main.o Camera.o sdlglutils.o  $(EXEC)
CFLAGS = `sdl-config --cflags` -g
LDFLAGS = `sdl-config --libs` -lSDL_image -lGL -lGLU

Utils.o:Utils.cpp
	g++ -o Utils.o -c Utils.cpp $(CFLAGS)
VectorUtil.o:VectorUtil.cpp
	g++ -o VectorUtil.o -c VectorUtil.cpp $(CFLAGS)
Terrain.o:Terrain.cpp
	g++ -o Terrain.o -c Terrain.cpp $(CFLAGS)
Clip.o:Clip.cpp
	g++ -o Clip.o -c Clip.cpp $(CFLAGS)
Light.o:Light.cpp
	g++ -o Light.o -c Light.cpp $(CFLAGS)
main.o: main.cpp
	g++ -o main.o -c main.cpp $(CFLAGS)
sdlglutils.o: sdlglutils.cpp sdlglutils.h
	g++ -o sdlglutils.o -c sdlglutils.cpp $(CFLAGS)
Camera.o: Camera.cpp Camera.h
	g++ -o Camera.o -c Camera.cpp $(CFLAGS)
$(EXEC): main.o Camera.o sdlglutils.o 
	g++ main.o Camera.o sdlglutils.o -o $(EXEC) $(LDFLAGS)
clean:
	rm *o $(EXEC)
exec:
	make && ./$(EXEC)
