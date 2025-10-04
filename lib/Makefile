# -*- MakeFile -*-




myshell: myshell.o builtin.o nonbuiltin.o utils.o
	g++ myshell.o builtin.o nonbuiltin.o utils.o -o myshell -l readline

myshell.o: myshell.cpp builtin.h nonbuiltin.h utils.h
	g++ -c myshell.cpp

builtin.o: builtin.cpp builtin.h
	g++ -c builtin.cpp

nonbuiltin.o: nonbuiltin.cpp nonbuiltin.h
	g++ -c nonbuiltin.cpp

utils.o: utils.cpp utils.h
	g++ -c utils.cpp

clean:
	rm *.o myshell
