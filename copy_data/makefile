CC = g++
CXXFLAGS = -std=c++14 -Wall
LDFLAGS =  -L/opt/pgsql/lib -L/usr/lib/x86_64-linux-gnu
LIBS = -lpq -lcrypto++

SRCDIR = src
INCDIR = -I/root/pakages/copy/HashJoinOverEncryptedData/copy_data
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hh)
OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))
Target = main

.PHONY: all clean

all: $(Target)

$(OBJDIR)/%.o:%.cpp
	$(CXX) $(CXXFLAGS) $(INCDIR) -c $< -o $@

$(Target): $(OBJECTS)
	$(CXX)  $^ -o $@  $(LDFLAGS) $(LIBS)


clean:
	$(RM) *.o $(Target)