# Make file for Ultima 2.0
# Drake Wood, James Giegerich

# Variables to control Makefile operation

CXX = g++
LINKS = -lpthread -lncurses
CXXFLAGS = -Wall -g

# Targets needed to bring the executable up to date

main: main.o scheduler.o semaphore.o window.o ipc.o memory.o
	$(CXX) $(CXXFLAGS) -o main main.o scheduler.o semaphore.o window.o ipc.o memory.o $(LINKS)
	
main.o: main.cpp scheduler.h semaphore.h queue.h window.h
	$(CXX) $(CXXFLAGS) -c main.cpp $(LINKS)
	
window.o: window.h window.cpp semaphore.h ipc.h
	$(CXX) $(CXXFLAGS) -c window.cpp $(LINKS)

scheduler.o: scheduler.h scheduler.cpp queue.h window.h
	$(CXX) $(CXXFLAGS) -c scheduler.cpp $(LINKS)

semaphore.o: semaphore.h semaphore.cpp queue.h window.h memory.h
	$(CXX) $(CXXFLAGS) -c semaphore.cpp $(LINKS)
	
ipc.o: ipc.h ipc.cpp scheduler.h queue.h window.h
	$(CXX) $(CXXFLAGS) -c ipc.cpp $(LINKS)

memory.o: memory.h memory.cpp semaphore.h
	$(CXX) $(CXXFLAGS) -c memory.cpp $(LINKS)
		
clean:
	rm *.o