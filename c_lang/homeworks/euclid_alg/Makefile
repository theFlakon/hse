.PHONY: clear
CC := cc
SRC = ./src/main.c ./src/math_funcs.c ./src/io_funcs.c 
OBJS = $(patsubst ./src/%.c, ./build/%.o, $(SRC))
TARGET = pr

$(shell mkdir -p build)

$(TARGET): $(OBJS)
	$(CC) -lm $^ -o $@

./build/%.o: ./src/%.c
	$(CC) -c -Iheaders $< -o $@

clean:
	@rm -rf ./build $(TARGET)
