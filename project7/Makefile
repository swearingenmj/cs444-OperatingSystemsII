image.o: image.c
	gcc -Wall -Wextra -c $<

block.o: block.c
	gcc -Wall -Wextra -c $<

free.o: free.c
	gcc -Wall -Wextra -c $<

mkfs.o: mkfs.c
	gcc -Wall -Wextra -c $<

inode.o: inode.c
	gcc -Wall -Wextra -c $<

pack.o: pack.c
	gcc -Wall -Wextra -c $<

dir.o: dir.c
	gcc -Wall -Wextra -c $<

ls.o: ls.c
	gcc -Wall -Wextra -c $<

simfs_test.o: simfs_test.c
	gcc -Wall -Wextra -c $<

simfs.a: image.o block.o free.o mkfs.o inode.o pack.o dir.o ls.o
	ar rcs $@ $^

simfs_test: simfs_test.c simfs.a
	gcc -Wall -Wextra -DCTEST_ENABLE -o $@ $^

.PHONY: test

test: simfs_test
	./simfs_test
