# Wymagana wersja CMake
cmake_minimum_required(VERSION 3.10)

# Nazwa projektu
project(BazyDanych)

# Ustaw standard C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Dodaj wszystkie pliki .cpp z katalogu src
file(GLOB SRC_FILES src/*.cpp)

# Utwórz plik wykonywalny
add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

# Dodaj katalog z nagłówkami (include)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)


# Dodaj bibliotekę bcrypt
file(GLOB BCRYPT_SOURCES lib/bcrypt/*.c lib/bcrypt/crypt_blowfish/*.c)
add_library(bcrypt STATIC ${BCRYPT_SOURCES})

# Flagi linkera (OpenSSL i inne)
target_link_libraries(${PROJECT_NAME} PRIVATE bcrypt ssl crypto crypt mysqlcppconn)
