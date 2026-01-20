.PHONY: clean
CC = cc
SRCS = $(wildcard ./src/*.c)
OBJS = $(patsubst ./src/%.c, ./obj/%.o, $(SRCS))
TARGET = program

$(shell mkdir -p obj)

$(TARGET): $(OBJS)
	@$(CC) -lm $^ -o $@

./obj/%.o: ./src/%.c
	@$(CC) -c -Iheaders $< -o $@

clean:
	@rm -rf ./obj $(TARGET)
