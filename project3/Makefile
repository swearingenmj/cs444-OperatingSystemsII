reservations: pcseml.c
	gcc -Wall -Wextra -o $@ $^ -lpthread

reservations.zip:
	rm -f $@
	zip $@ Makefile pcseml.c
