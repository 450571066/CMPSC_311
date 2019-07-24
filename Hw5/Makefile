CFLAGS := -Wall -fstack-protector

all: good_echo good_echo.s good_echo.o 

good_echo: good_echo.c 
	gcc $(CFLAGS) -o good_echo good_echo.c
good_echo.s: good_echo.c 
	gcc $(CFLAGS)  -S good_echo.c
good_echo.o: good_echo.c
	gcc $(CFLAGS)  -c good_echo.c 

.PHONY: clean
clean:
	rm -f good_echo.o good_echo.s good_echo
