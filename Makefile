
CFLAGS = -std=c++14
ALL = timeseries/blockbuilder.cpp stream/bitstream.cpp


all: main.cpp $(ALL)
	g++ $(CFLAGS) main.cpp $(ALL)

clean:
	rm *.out
	rm stream/*.gch
	rm timeseries/*.gch

tests: test1

test1:
	g++ $(CFLAGS) tests/bitstream_test.cpp $(ALL) -o test1.o && ./test1.o && rm test1.o
