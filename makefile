# Kompilator
CXX = g++

# Flagi kompilacji
CXXFLAGS = -I./bcrypt -I./bcrypt/crypt_blowfish -Wall -Wextra -std=c++11

# Flagi linkera (OpenSSL)
LDFLAGS = -L./bcrypt -lssl -lcrypto -lcrypt -lmysqlcppconn

# Plik wykonywalny
TARGET = prog.o

# Pliki źródłowe
SRCS = main.cpp bcrypt/bcrypt.c bcrypt/crypt_blowfish/*.c

# Pliki obiektowe
OBJS = $(SRCS:.cpp=.o)

# Domyślna reguła
all: $(TARGET)

# Reguła budowy pliku wykonywalnego
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Reguła kompilacji plików źródłowych
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Reguła czyszczenia plików
clean:
	rm -f $(OBJS) $(TARGET)
