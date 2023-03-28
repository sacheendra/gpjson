DIR = bin

CC = g++
CC_FLAGS = -O3 -std=c++11 -mavx -mavx2 -msse -msse2 -msse4 -msse4.2 -mpclmul
POST_FLAGS = -lpthread -mcmodel=medium -static-libstdc++

PISON_DIR = $(DIR)/pison
PISON_SRC = ../../engine/pison.cpp ../../src/pison/*.cpp

RAPIDJSON_DIR = $(DIR)/rapidjson
RAPIDJSON_SRC = ../../engine/rapidjson.cpp

SIMDJSON_DIR = $(DIR)/simdjson
SIMDJSON_SRC = ../../engine/simdjson.cpp ../../src/simdjson/*.cpp

all: %

pison: batch1.cpp sizes.cpp selectivity.cpp
	mkdir -p $(DIR)
	mkdir -p $(PISON_DIR)
	$(CC) $(CC_FLAGS) -o $(PISON_DIR)/batch1 batch1.cpp $(PISON_SRC) $(POST_FLAGS)
	$(CC) $(CC_FLAGS) -o $(PISON_DIR)/sizes sizes.cpp $(PISON_SRC) $(POST_FLAGS)
	$(CC) $(CC_FLAGS) -o $(PISON_DIR)/selectivity selectivity.cpp $(PISON_SRC) $(POST_FLAGS)

rapidjson: batch1.cpp
	mkdir -p $(DIR)
	mkdir -p $(RAPIDJSON_DIR)
	$(CC) $(CC_FLAGS) -o $(RAPIDJSON_DIR)/batch1 batch1.cpp $(RAPIDJSON_SRC) $(POST_FLAGS)
	$(CC) $(CC_FLAGS) -o $(RAPIDJSON_DIR)/sizes sizes.cpp $(RAPIDJSON_SRC) $(POST_FLAGS)
	$(CC) $(CC_FLAGS) -o $(RAPIDJSON_DIR)/selectivity selectivity.cpp $(RAPIDJSON_SRC) $(POST_FLAGS)

simdjson: batch1.cpp
	mkdir -p $(DIR)
	mkdir -p $(SIMDJSON_DIR)
	$(CC) $(CC_FLAGS) -o $(SIMDJSON_DIR)/batch1 batch1.cpp $(SIMDJSON_SRC) $(POST_FLAGS)
	$(CC) $(CC_FLAGS) -o $(SIMDJSON_DIR)/sizes sizes.cpp $(SIMDJSON_SRC) $(POST_FLAGS)
	$(CC) $(CC_FLAGS) -o $(SIMDJSON_DIR)/selectivity selectivity.cpp $(SIMDJSON_SRC) $(POST_FLAGS)

clean:
	-$(RM) $(DIR)/*