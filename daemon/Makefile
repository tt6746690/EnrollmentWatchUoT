COMPILER = clang++ 
CFLAGS = -std=c++11 -I/usr/local/Cellar/curl/7.54.0/include
CLIBS = -L/usr/local/Cellar/curl/7.54.0/lib -lcurl -lldap -lz

CPP_FILES = $(wildcard ./*.cpp)
EXECUTABLES = $(CPP_FILES:.cpp=)

all: watcherd

%.o: %.cpp %.h
	$(COMPILER) $(CFLAGS) -c $<

watcherd: daemon.o watcherd.o
	$(COMPILER) $(CFLAGS) $(CLIBS) -o $@ $^

clean:
	rm *.o watcherd

run: 
	./watcherd
	cat /var/log/system.log | grep enrol | tail

show:
	cat /var/log/system.log | grep watch

kill:
	kill "$(cat *.pid)"


	
