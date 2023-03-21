CC = g++ 
CPPFLAGS = -Wall -std=c++17 
LDFLAGS =  -lpq -lcryptopp

#Source files
SRC = main.cpp pg_copy.cpp Hash_Encrypt.cpp child_table.cpp extend_table.cpp MurmurHash3.cpp

#Object files
OBJ = $(SRC:.cpp=.o)
CPPFLAGS +=-I/root/pakages/copy/HashJoinOverEncryptedData/client 
            # -I/usr/include/cryptopp 
#Header files
DEPS = pg_copy.hh Hash_Encrypt.hh child_table.hh extend_table.hh MurmurHash3.hh

TARGET = main
.PHONY:all clean
all: $(TARGET)

$(TARGET): $(OBJ) 
	$(CC) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.cpp $(DEPS) 
	$(CC) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJ)
