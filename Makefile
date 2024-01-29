Q3: Q3.c
	gcc -pthread Q3.c -o Q3
	./Q3

clean:
	rm -rf Q3
	rm -rf Q2
	rm -rf Q1
	rm -rf Q2_other
	

Q2: Q2.c
	gcc -o Q2 Q2.c -pthread -lrt -w
	./Q2


Q1:
	gcc -pthread Q1.c -o Q1
	./Q1

kot: Q2_other.c
	gcc -o Q2_other Q2_other.c -pthread -lrt -w
	./Q2_other