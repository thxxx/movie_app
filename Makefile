all : program1 program2 program3 miniShell

program1 : program1.c
	gcc -o $@ $<

program2 : program2.c
	gcc -o $@ $<

program3 : program3.c
	gcc -o $@ $<

miniShell : miniShell.c
	gcc -o $@ $<

clean :
	rm -f prog program1 program2 program3 miniShell