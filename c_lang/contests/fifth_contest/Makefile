.PHONY: clean all

CC = cc
CFLAGS = -O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
         -Wswitch-default -Wfloat-equal -Wpointer-arith -Wtype-limits \
         -Wempty-body -Wno-logical-op -Wstrict-prototypes -Wold-style-declaration \
         -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers \
         -Wnested-externs -Wno-pointer-sign -Wno-unused-result -std=gnu99 -DLOG_ALL
LDFLAGS = -lm

SRC = $(wildcard *.c)
TARGETS = $(SRC:.c=)

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGETS)
