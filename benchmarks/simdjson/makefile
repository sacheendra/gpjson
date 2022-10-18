DIR = bin

SRC1 = batch1

EXEC1 = $(DIR)/$(SRC1)
TARGET = $(EXEC1)
all: $(SRC1)

CC = g++
CC_FLAGS = -O3 -std=c++11 -mavx -mavx2 -msse -msse2 -msse4 -msse4.2 -mpclmul
POST_FLAGS = -lpthread -mcmodel=medium -static-libstdc++

SOURCE1 = src/*.cpp $(SRC1).cpp
$(SRC1): $(SOURCE1)
	mkdir -p $(DIR)
	$(CC) $(CC_FLAGS) -o $(EXEC1) $(SOURCE1) $(POST_FLAGS)

clean:
	-$(RM) $(TARGET)