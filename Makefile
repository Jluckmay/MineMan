all: MineMan

MineMan: main.o Mapa.o Block.o
	g++ -o MineMan main.o Mapa.o Block.o -lallegro -lallegro_image
	rm -rf *.o
main.o: src/main.cpp src/Mapa.h src/Block.h
	g++ -o main.o src/main.cpp -c
Mapa.o: src/Mapa.cpp src/Mapa.h
	g++ -o Mapa.o src/Mapa.cpp -c
Block.o: src/Block.cpp src/Block.h
	g++ -o Block.o src/Block.cpp -c

clean:
	rm -rf *.o *~ MineMan