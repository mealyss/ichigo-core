all: ichigo

ichigo: clean
	gcc  -Wall -O3 -pthread -msse4 src/*.c src/format/*.c src/platform/*.c src/platform/unix/*.c src/render/*.c  src/parallel/*.c -o ichigo -lm
	./ichigo 
	eog result.bmp

clean:
	rm -f ichigo
