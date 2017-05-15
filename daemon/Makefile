COMPILER = clang++ 
FLAGS = -std=c++11
CPP_FILES = $(wildcard ./*.cpp)
EXECUTABLES = $(CPP_FILES:.cpp=)

all: $(EXECUTABLES)

%: %.cpp %.h
	$(COMPILER) $(FLAGS) -o $@ $<

run: 
	./daemon
