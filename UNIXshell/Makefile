Exec: ShellExec.c
	gcc -w ShellExec.c -o Exec

a.out: Exec shell.c
	gcc -w shell.c -lreadline

clean:
	rm -f Exec
	rm -f a.out
