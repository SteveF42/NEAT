all: compile main link

main:
	g++ -c main.cpp -I./include -I"C:\msys64\mingw64\SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit\SFML-2.5.1\include" -DSFML_STATIC -g
	
test:
	g++ -c ./tests/*.cpp -I ./include -g
	g++ *.o -o test
	./test

test_link:
	g++ *.o -o test
	./test

compile:
	g++ -c ./src/*.cpp -I./include -I"C:\msys64\mingw64\SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit\SFML-2.5.1\include" -DSFML_STATIC -g 

link:
	g++ *.o -o main -L"C:\msys64\mingw64\SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit\SFML-2.5.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -lsfml-main
clean:
	rm -f *.o
	rm -f *.exe

run:
	./main