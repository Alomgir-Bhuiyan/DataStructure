CC ?= gcc
LIB := lib
INCLUDE := include
BUILD := build
SRCDIR:=src

SRC := $(wildcard $(LIB)/*.c)
CFILE := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(LIB)/%.c,$(BUILD)/%.so,$(SRC))

CFLAGS := -fPIC -Wall -Wextra -Werror -O2 -shared

.PHONY: all clean setup install

all: setup $(OBJ)

$(BUILD)/%.so: $(LIB)/%.c
	$(CC) $(CFLAGS) $< -o $@

setup:
	mkdir -p $(BUILD)

install: all
	mkdir -p $(PREFIX)/lib $(PREFIX)/include
	cp $(BUILD)/*.so $(PREFIX)/lib
	cp $(INCLUDE)/*.h $(PREFIX)/include

clean:
	rm -rf $(BUILD)

test:
	$(CC) $(CFILE) -llinkedlist -o $(BUILD)/test
	./$(BUILD)/test
