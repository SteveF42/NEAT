all: compile link

test:
	g++ -c ./tests/*.cpp -I ./include
	g++ *.o -o test
	./test

test_link:
	g++ *.o -o test
	./test

compile:
	g++ -c ./src/*.cpp -I ./include

link:
	g++ *.o -o main

clean:
	rm -f *.o main

run:
	./main