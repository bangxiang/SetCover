PROGRAM = TestSetCover.exe
FILES := $(shell find . ! -name "*.hh" -name "*.cc")
OBJS := $(patsubst %.cc, %.o, $(FILES))
CC = g++
CXXFLAGS+=-O3 -DNDEBUG -Werror -Wall -fPIC -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) -o $(PROGRAM)

clean:
	rm -f *.o

.PHONY: all clean
