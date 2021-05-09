CC = g++
OUT = jetstrap

debug:
	$(CC) -o $(OUT) src/*.cpp -g -Og -Wall

release:
	$(CC) -o $(OUT) src/*.cpp -s -O3 -Wall

clean:
	rm *.o jetstrap
