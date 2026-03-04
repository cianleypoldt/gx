CC = gcc
CFLAGS = -Wall -Wextra -I. -Ithird_party
LDFLAGS = -lglfw -lm

THIRD_PARTY = third_party/glad.c
MAIN_SRCS = main.c
RENDER_SRCS = $(shell find render -name "*.c" | sort)
COMMON_SRCS = $(shell find common -name "*.c" | sort)
ALL_SRCS = $(THIRD_PARTY) $(MAIN_SRCS) $(RENDER_SRCS) $(COMMON_SRCS)

TARGET = build/gx

.PHONY: all run bear clean

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(ALL_SRCS) | build
	$(CC) $(CFLAGS) $(ALL_SRCS) $(LDFLAGS) -o $(TARGET)

build:
	mkdir -p build

bear: $(ALL_SRCS) | build
	bear -- $(CC) -c $(ALL_SRCS) $(LDFLAGS) -I. -Ithird_party
	@echo "Generated compile_commands.json"
	$(MAKE) all

clean:
	rm -rf build
