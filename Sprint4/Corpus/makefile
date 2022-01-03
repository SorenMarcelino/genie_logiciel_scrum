all:	exe
clean:
		rm *.o testSprint4 test

test.o:	test.c
		g++ -c test.c

testSprint4.o: testSprint4.cpp
		g++ -c testSprint4.cpp

exe: test.o testSprint4.o
		g++ -o test test.o
		g++ -o testSprint4 testSprint4.o