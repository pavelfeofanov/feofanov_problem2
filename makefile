all: program

program: main.o KS_parser.o KS_solver.o
	@g++ -o program main.o KS_parser.o KS_solver.o

main.o: main.cpp KS_parser.h KS_solver.h
	@g++ -c main.cpp

KS_parser.o: KS_parser.cpp KS_parser.h
	@g++ -c KS_parser.cpp

KS_solver.o: KS_solver.cpp KS_solver.h
	@g++ -c KS_solver.cpp

clean:
	@rm -f *.o program
