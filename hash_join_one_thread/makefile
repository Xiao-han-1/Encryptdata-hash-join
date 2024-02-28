CC = g++
CXXFLAGS = -g -O0  -fno-omit-frame-pointer -Wall -std=c++11 -pthread
LDFLAGS =  -L/data/local/pgsql/lib -L/usr/lib64
LIBS = -lpq -lcryptopp -lssl -lcrypto -lstdc++

CUR_PATH := $(CURDIR)
SRCDIR = src
INCDIR = -I$(CUR_PATH) -I/data/local/pgsql/include
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hh)
OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))
Target = Hash_join_3

.PHONY: all clean

all: $(Target)



$(Target): $(OBJECTS)
	$(CXX)   $(CXXFLAGS) $^ -o $@  $(LDFLAGS) $(LIBS)

$(OBJDIR)/%.o:%.cpp
	$(CXX)  $(INCDIR) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@ 

clean:
	$(RM) $(OBJDIR)/*.o $(Target)