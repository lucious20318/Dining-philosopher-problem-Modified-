all: q1 q2 q3
	
q1: quest1.c 
	gcc -pthread quest1.c -o q1o

q2: quest2.c
	gcc -pthread quest2.c -o q2o

q3: quest3.c
	gcc -pthread quest3.c -o q3o
	
