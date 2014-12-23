PROJECT = life
SRCS    = $(wildcard *.cpp)
OBJS    = $(SRCS:.cpp=.o)

CXXFLAGS  += -I$(SYSTEMC)/include -Wall -Wextra -g -ggdb
LDFLAGS += -L$(SYSTEMC)/lib$(ARCH_SUFFIX)
LDLIBS += -lsystemc -lm -lncurses

$(PROJECT): $(OBJS) $(SYSTEMC)/lib$(ARCH_SUFFIX)/libsystemc.a
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS) 2>&1

clean:
	rm -f $(PROJECT) *.o
.PHONY: clean
