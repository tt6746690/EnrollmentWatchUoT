COMPILER = clang++ 
FLAGS = -std=c++11 
CPP_FILES = $(wildcard ./*.cpp)
EXECUTABLES = $(CPP_FILES:.cpp=)

all: $(EXECUTABLES)

%: %.cpp %.h
	$(COMPILER) $(FLAGS) -o $@ $<

run: 
	./daemon
	cat /var/log/system.log | grep enrol | tail

show:
	cat /var/log/system.log | grep enrol 

kill:
	kill "$(cat *.pid)"


	
