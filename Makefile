
CFLAGS = -std=c++20
ALL = stream/blockbuilder.cpp stream/bitstream.cpp timeseries/timeseries.cpp


all: main.cpp $(ALL)
	g++ $(CFLAGS) main.cpp $(ALL)

clean:
	rm *.out
	rm stream/*.gch
	rm timeseries/*.gch

tests: test1 test2

test1:
	g++ $(CFLAGS) tests/bitstream_test.cpp $(ALL) -o test1.o && ./test1.o && rm test1.o

test2:
	g++ $(CFLAGS) tests/blockbuilder_test.cpp $(ALL) -o test1.o && ./test1.o && rm test1.o

