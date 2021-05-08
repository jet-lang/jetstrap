CC = g++
OUT = jetstrap

debug:
	$(CC) -o $(OUT) src/main.cpp src/parser.cpp -g -Og -Wall

release:
	$(CC) -o $(OUT) src/main.cpp src/parser.cpp -s -O3 -Wall

clean:
	rm *.o jetstrap
