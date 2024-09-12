.PHONY: all clean clean_bin clean_obj version

MAKE = make
GXX = g++ -std=c++20

WOLF = src/wolf
OBJ_WOLF = obj/wolf

all: version clean wolf

version:
	@echo "MAKE:" && $(MAKE) --version
	@echo "GXX:" && $(GXX) --version

clean_bin:
	@rm -f bin/wolf
	@rm -f bin/*.so
	@echo "Binaries cleaned"
clean_obj:
	@rm -f $(OBJ_WOLF)/*.o
	@echo "Object files cleaned"
clean: clean_bin clean_obj

obj/wolf/map.o: src/wolf/map.cpp
	@echo "Compiling src/wolf/map.cpp"
	@$(GXX) -c -o $@ $< -Isrc/wolf -lSDL2
obj/wolf/main.o: src/wolf/main.cpp
	@echo "Compiling src/wolf/main.cpp"
	@$(GXX) -c -o $@ $< -Isrc/wolf -lSDL2
obj/wolf/window.o: src/wolf/window.cpp
	@echo "Compiling src/wolf/window.cpp"
	@$(GXX) -c -o $@ $< -Isrc/wolf -lSDL2
wolf: obj/wolf/map.o obj/wolf/main.o obj/wolf/window.o
	@echo "Combining wolf"
	@$(GXX) -o bin/wolf $^ -Isrc/wolf -lSDL2
