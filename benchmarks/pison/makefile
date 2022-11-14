DIR = bin

SRC1 = batch1
SRC2 = sizes
SRC3 = selectivity

EXEC1 = $(DIR)/$(SRC1)
EXEC2 = $(DIR)/$(SRC2)
EXEC3 = $(DIR)/$(SRC3)
TARGET = $(EXEC1) $(EXEC2) $(EXEC3)
all: $(SRC1) $(SRC2) $(SRC3)

CC = g++
CC_FLAGS = -O3 -std=c++11 -mavx -mavx2 -msse -msse2 -msse4 -msse4.2 -mpclmul
POST_FLAGS = -lpthread -mcmodel=medium -static-libstdc++

SOURCE1 = src/*.cpp $(SRC1).cpp
$(SRC1): $(SOURCE1)
	mkdir -p $(DIR)
	$(CC) $(CC_FLAGS) -o $(EXEC1) $(SOURCE1) $(POST_FLAGS)

SOURCE2 = src/*.cpp $(SRC2).cpp
$(SRC2): $(SOURCE2)
	mkdir -p $(DIR)
	$(CC) $(CC_FLAGS) -o $(EXEC2) $(SOURCE2) $(POST_FLAGS)

SOURCE3 = src/*.cpp $(SRC3).cpp
$(SRC3): $(SOURCE3)
	mkdir -p $(DIR)
	$(CC) $(CC_FLAGS) -o $(EXEC3) $(SOURCE3) $(POST_FLAGS)

clean:
	-$(RM) $(TARGET)