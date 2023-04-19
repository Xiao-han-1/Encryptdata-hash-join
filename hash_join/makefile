CC = g++
CXXFLAGS = -g -O0  -fno-omit-frame-pointer -Wall -std=c++11 -pthread
LDFLAGS =  -L/opt/pgsql/lib -L/usr/lib64
LIBS = -lpq -lcryptopp -lssl -lcrypto -lstdc++

CUR_PATH := $(CURDIR)
SRCDIR = src
INCDIR = -I$(CUR_PATH)
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hh)
OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))
Target = Hash_join

.PHONY: all clean

all: $(Target)



$(Target): $(OBJECTS)
	$(CXX)   $(CXXFLAGS) $^ -o $@  $(LDFLAGS) $(LIBS)

$(OBJDIR)/%.o:%.cpp
	$(CXX)  $(INCDIR) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@ 

clean:
	$(RM) $(OBJDIR)/*.o $(Target)