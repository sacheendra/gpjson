DIR = bin

CC = g++
CC_FLAGS = -O3 -std=c++11 -mavx -mavx2 -msse -msse2 -msse4 -msse4.2 -mpclmul
POST_FLAGS = -lpthread -mcmodel=medium -static-libstdc++

all: %

%: %.cpp
	mkdir -p $(DIR)
	$(CC) $(CC_FLAGS) -o $(DIR)/$@ src/*.cpp $@.cpp $(POST_FLAGS)

clean:
	-$(RM) $(DIR)/*