CFLAGS=-Wextra -Wall -pedantic -Werror -std=c11
cat: cat.c

cat.c:
	cc $(CFLAGS) cat.c -o cat
