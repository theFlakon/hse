.PHONY: clear
CC := cc
SRC = ./src/main.c ./src/files_io_funcs.c ./src/err_funcs.c ./src/util_funcs.c
OBJS = $(patsubst ./src/%.c, ./build/%.o, $(SRC))
TARGET = ls

CFLAGS = -O2 -c -Iheaders -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
         -Wswitch-default -Wfloat-equal -Wpointer-arith -Wtype-limits \
         -Wempty-body -Wno-logical-op -Wstrict-prototypes -Wold-style-declaration \
         -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers \
         -Wnested-externs -Wno-pointer-sign -Wno-unused-result -std=gnu99
LDFLAGS = -lm

$(shell mkdir -p build)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

./build/%.o: ./src/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -rf ./build $(TARGET)
