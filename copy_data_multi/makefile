CC = g++
CXXFLAGS = -g -O0  -fno-omit-frame-pointer -Wall -std=c++11
LDFLAGS =  -L/data/local/pgsql/lib -L/data/lib/x86_64-linux-gnu -L/usr/local/bin
LIBS = -lcryptopp -lpq -lssl -lcrypto

CUR_PATH := $(CURDIR)
SRCDIR = src
INCDIR = -I$(CUR_PATH) -I/data/local/pgsql/include
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hh)
OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))
Target = Copy_data_zipf_v1

.PHONY: all clean

all: $(Target)



$(Target): $(OBJECTS)
	$(CXX)   $(CXXFLAGS) $^ -o $@  $(LDFLAGS) $(LIBS)

$(OBJDIR)/%.o:%.cpp
	$(CXX)  $(INCDIR) $(CXXFLAGS) $(LIBS) -c $< -o $@ 

clean:
	$(RM) $(OBJDIR)/*.o $(Target)