CC = g++
CXXFLAGS = -g -O0  -fno-omit-frame-pointer -Wall -std=c++14 
LDFLAGS =  -L/opt/pgsql/lib -L/usr/lib/x86_64-linux-gnu
LIBS = -lpq -lcrypto++ -lssl -lcrypto

CUR_PATH := $(CURDIR)
SRCDIR = src
INCDIR = -I$(CUR_PATH)
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hh)
OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))
Target = Copy_data

.PHONY: all clean

all: $(Target)



$(Target): $(OBJECTS)
	$(CXX)   $(CXXFLAGS) $^ -o $@  $(LDFLAGS) $(LIBS)

$(OBJDIR)/%.o:%.cpp
	$(CXX)  $(INCDIR) $(CXXFLAGS) $(LIBS) -c $< -o $@ 

clean:
	$(RM) $(OBJDIR)/*.o $(Target)