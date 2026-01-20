.PHONY: clear

CC := cc

SRC = ./src/main.c ./src/str_funcs.c
OBJS = $(patsubst ./src/%.c, ./build/%.o, $(SRC))

CFLAGS := -Wall -Wextra -Werror -O2 -std=c99 -Iheaders -c
LDFLAGS := -lm 

TARGET = pr

$(shell mkdir -p build)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

./build/%.o: ./src/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -rf ./build $(TARGET)
