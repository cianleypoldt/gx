CC      = gcc
CFLAGS  = -Wall -Wextra -I. -Ithird_party -MMD -MP
LDFLAGS = -lglfw -lm

THIRD_PARTY = third_party/glad.c
MAIN_SRCS   = main.c
RENDER_SRCS = $(shell find render  -name "*.c" | sort)
COMMON_SRCS = $(shell find common  -name "*.c" | sort)

ALL_SRCS = $(THIRD_PARTY) $(MAIN_SRCS) $(RENDER_SRCS) $(COMMON_SRCS)

# Derive object files under build/, preserving subdirectory structure
ALL_OBJS = $(patsubst %.c, build/%.o, $(ALL_SRCS))
ALL_DEPS = $(ALL_OBJS:.o=.d)

TARGET = build/gx

.PHONY: all run bear clean

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(ALL_OBJS) | build
	$(CC) $(ALL_OBJS) $(LDFLAGS) -o $(TARGET)

# Pattern rule: compile any .c -> build/<path>.o
build/%.o: %.c | build
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Pull in generated header dependencies (silently skip if absent)
-include $(ALL_DEPS)

build:
	mkdir -p build

bear:
	rm -f compile_commands.json
	bear -- $(MAKE) all
	@echo "Generated compile_commands.json"

clean:
	rm -rf build compile_commands.json
