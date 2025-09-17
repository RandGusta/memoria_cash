CPP = g++
MYLIB = ../my-lib
CPPFLAGS = -Wall -std=c++23 -I$(MYLIB)/include
LDFLAGS =
BIN_NAME = cache-sim
RM = rm

########################################################

SRC = $(wildcard *.cpp)

headerfiles = $(wildcard *.h)

OBJS = ${SRC:.cpp=.o}

########################################################

# implicit rules

%.o : %.cpp $(headerfiles)
	$(CPP) -c $(CPPFLAGS) $< -o $@

########################################################

all: $(BIN_NAME)
	@echo program compiled!
	@echo yes!

$(BIN_NAME): $(OBJS)
	$(CPP) -o $(BIN_NAME) $(OBJS) $(LDFLAGS)

clean:
	-$(RM) $(OBJS)
	-$(RM) $(BIN_NAME)

