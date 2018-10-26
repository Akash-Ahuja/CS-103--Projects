SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
HEADERS := $(wildcard *.h)
TARGETS := wc sort uniq shuf tr

COMMON   := -O2 -Wall -Wno-narrowing -DNDEBUG -std=c++11
CFLAGS   := $(CFLAGS) $(COMMON)
CC       := g++
LD       := $(CC)

# IMPL := wc.o sort.o uniq.o nl.o shuf.o
ifdef skel
# IMPL := $(IMPL:.o=-skel.o)
IMPL := -skel
endif

all : $(TARGETS)
.PHONY : all

# {{{ for debugging
debug : CFLAGS += -g -DDEBUG=1 -U NDEBUG
debug : $(TARGETS)
.PHONY : debug
# }}}

$(OBJECTS) : %.o : %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGETS) : % : %$(IMPL).o
	$(LD) -o $@ $^

.PHONY : clean
clean :
	rm -f $(OBJECTS) $(TARGETS)
