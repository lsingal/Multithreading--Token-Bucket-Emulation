warmup2: warmup2.o my402list.o print.o signal.o thread.o time.o
	gcc -o warmup2 -g -lm warmup2.o my402list.o print.o signal.o thread.o time.o

main.o: warmup2.c
	gcc -g -c -Wall warmup2.c

print.o: print.c
	gcc -g -c -Wall print.c

signal.o: signal.c
	gcc -g -c -Wall signal.c

thread.o: thread.c
	gcc -g -c -Wall thread.c

time.o: time.c
	gcc -g -c -Wall time.c

my402list.o: my402list.c
	gcc -g -c -Wall my402list.c

clean:
	rm -f *.o warmup2
