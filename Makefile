all: compile main link

main:
	g++ -c main.cpp -I ./include -g
	
test:
	g++ -c ./tests/*.cpp -I ./include -g
	g++ *.o -o test
	./test

test_link:
	g++ *.o -o test
	./test

compile:
	g++ -c ./src/*.cpp -I ./include -g 

link:
	g++ *.o -o main

clean:
	rm -f *.o
	rm -f *.exe

run:
	./main