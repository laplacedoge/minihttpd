.PHONY: all clean

all: httpd.c
	@gcc httpd.c -o httpd.elf

clean:
	@rm -rf httpd.elf
