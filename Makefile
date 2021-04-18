all : program1 pr1merge program2 program3 miniShell

program1 : program1.c
	gcc -o $@ $<

pr1merge : pr1merge.c
	gcc -o $@ $<

program2 : program2.c
	gcc -o $@ $<

program3 : program3.c
	gcc -o $@ $< -lpthread

miniShell : miniShell.c
	gcc -o $@ $<

clean :
	rm -f prog program1 pr1merge program2 program3 miniShell