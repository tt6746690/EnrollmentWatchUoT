COMPILER = clang++ 
FLAGS = -std=c++11
CPP_FILES = $(wildcard ./*.cpp)
EXECUTABLES = $(CPP_FILES:.cpp=)

all: $(EXECUTABLES)

%: %.cpp %.h
	$(COMPILER) $(FLAGS) -o $@ $<

run: 
	rm /tmp/enrolwatchd.pid
	./daemon
	cat /var/log/system.log | grep enrol | tail


	
