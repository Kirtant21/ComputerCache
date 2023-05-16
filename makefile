CXX = g++
CXXFLAGS = -Wall

mytest.o: cache.o mytest.cpp
	$(CXX) $(CXXFLAGS) cache.o mytest.cpp -o mytest
cache.o: cache.cpp cache.h
	$(CXX) $(CXXFLAGS) -c cache.cpp

run:
	./mytest
val:
	valgrind ./mytest
fullCheck:
	valgrind --leak-check=full ./mytest
debug:
	gdb ./mytest
clean:
	rm *.o*
	rm *~
	rm *#
s:
	emacs cache.cpp
sh:
	emacs cache.h
t:
	emacs mytest.cpp
mk:
	emacs makefile
submit:
	cp cache.h cache.cpp mytest.cpp ~/cs341proj/proj4/
chkSub:
	ls ~/cs341proj/proj4/
