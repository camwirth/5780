part4: part4.o task.o taskEDF.o
	g++ part4.o task.o taskEDF.o -o part4

part4.o: part4.cpp priority.hpp task.hpp taskEDF.hpp
	g++ -c part4.cpp

task.o: task.cpp task.hpp
	g++ -c task.cpp

taskEDF.o: taskEDF.cpp task.hpp
	g++ -c taskEDF.cpp

clean: 
	rm -f part4.o task.o taskEDF.o part4


