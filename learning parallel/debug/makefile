# The name of the main executable
TARGET=bug

# The objects that make up the main executables
OBJS=bug.o core.o

# The test executables
TESTS=core_test

# Flags and stuff, change as required
OPTIMZE=-O3
CFLAGS=-fopenmp -ggdb $(OPTIMIZE) -Wall
LDFLAGS=-fopenmp -lm
CC=gcc

# The first target (all) is allways the default target
all: build test

# Our buld target depends on the real target
.PHONY: build
build: $(TARGET)

# Our target is built up from the objects
$(TARGET): $(OBJS)

# Our test target
# 
# Here we instruct make to generate a line with all test
# which are all sequentially executed using the $(foreach)
# macro. We conclude with 'true' because the && construct
# awaits an argument on both sides. The 'true' command
# simply returns a success value
.PHONY: test
test: $(TESTS)
	$(foreach T,$(TESTS), ./$(T) &&) true


# Cleanup all generated files
clean:
	rm -Rf $(TESTS) $(OBJS) $(TARGET)
