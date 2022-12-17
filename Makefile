
FLAGS=-std=c++17 -Wall -Wextra -Wpedantic -D_GNU_SOURCE -Werror=all
COMPILER=g++

all: smalldb sdbsh

smalldb: main.cpp student.o parsing.o db.o query.o utils.o server.o query_handler.o
	${COMPILER} -o smalldb main.cpp parsing.o student.o db.o query.o utils.o server.o query_handler.o ${FLAGS} -lpthread
	rm *.o

sdbsh: client.cpp client.h	
	${COMPILER} -o sdbsh client.cpp ${FLAGS} -lpthread

run:
	make main && ./smalldb

parsing.o: parsing.cpp parsing.h
	${COMPILER} -c parsing.cpp ${FLAGS}

student.o: student.cpp student.h
	${COMPILER} -c student.cpp ${FLAGS}

query.o: query.cpp query.h
	${COMPILER} -c query.cpp ${FLAGS}

db.o: db.cpp db.h
	${COMPILER} -c db.cpp ${FLAGS}

server.o: server.cpp server.h
	${COMPILER} -c server.cpp ${FLAGS}

query_handler.o: query_handler.cpp query_handler.h
	${COMPILER} -c query_handler.cpp ${FLAGS}

utils.o: utils.cpp utils.h
	${COMPILER} -c utils.cpp ${FLAGS}

tests: tests/run_tests.py
	./tests/run_tests.py

clean:
	-rm *.o
	rm logs/*
