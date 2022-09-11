all:
	gcc -o newshell main.c -std=c11
clean:
	rm newshell -rf