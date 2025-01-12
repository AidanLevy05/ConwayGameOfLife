PROG = prog 
CC = g++
CPPFLAGS = -g -Wall -std=c++11 -fopenmp
OBJS = main.o 

$(PROG) : $(OBJS)
	$(CC) $(CPPFLAGS) -o $(PROG) $(OBJS) 

main.o : main.cpp 
	$(CC) $(CPPFLAGS) -c main.cpp 

clean:
	rm -f core $(PROG) $(OBJS) 

rebuild:
	make clean
	make
