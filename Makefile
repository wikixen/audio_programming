# CFLAGS = -lm
CFLAGS = -lm -lportsf
FILE = portsf


default:
	gcc $(CFLAGS) -o build/$(FILE) src/$(FILE).c