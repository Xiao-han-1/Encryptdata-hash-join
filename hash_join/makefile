CC = g++
CXXFLAGS = -g -O0  -fno-omit-frame-pointer -Wall -std=c++14 -pthread
LDFLAGS =  -I/root/pakages/copy/HashJoinOverEncryptedData -L/opt/pgsql/lib -L/usr/lib/x86_64-linux-gnu 
LIBS = -lpq -lcrypto++ -lssl -lcrypto

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