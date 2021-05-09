CC = g++
OUT = jetstrap

DEBUG_DIR = bin/debug
RELEASE_DIR = bin/release

debug: $(DEBUG_DIR)
	$(CC) -o bin/debug/$(OUT) src/*.cpp -g -Og -Wall

release: $(RELEASE_DIR)
	$(CC) -o bin/release/$(OUT) src/*.cpp -s -O3 -Wall

$(DEBUG_DIR):
	@echo "Folder $(DEBUG_DIR) does not exist, creating it"
	mkdir -p $@

$(RELEASE_DIR):
	@echo "Folder $(RELEASE_DIR) does not exist, creating it"
	mkdir -p $@
