SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
HEADERS := $(wildcard *.h)
TARGETS := warmup

COMMON   := -O2 -Wall -Wno-narrowing -DNDEBUG -std=c++11
CFLAGS   := $(CFLAGS) $(COMMON)
CC       := g++
LD       := $(CC)

IMPL := warmup.o

all : $(TARGETS)
.PHONY : all

# {{{ for debugging
debug : CFLAGS += -g -DDEBUG=1 -U NDEBUG
debug : $(TARGETS)
.PHONY : debug
# }}}

$(OBJECTS) : %.o : %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGETS) : $(IMPL)
	$(LD) -o $@ $^

.PHONY : clean
clean :
	rm -f $(OBJECTS) $(TARGETS)
