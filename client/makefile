CC = g++ CPPFLAGS = -Wall -std=c++17 LDFLAGS = -lcryptopp -lpq

#Source files
SRC = main.cpp pg_copy.cpp Hash_Encrypt.cpp child_table.cpp extend_table.cpp

#Object files
OBJ = $(SRC:.cpp=.o)

#Header files
DEPS = pg_copy.hh Hash_Encrypt.hh child_table.hh extend_table.hh

#Executable file name
TARGET = my_program

all: $(TARGET)

$(TARGET): $(OBJ) $(CC) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.cpp $(DEPS) $(CC) $(CPPFLAGS) -c -o $@ $<

