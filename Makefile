all: rainy

rainy: clean
	gcc -Wall -pthread src/*.c src/format/*.c src/platform/*.c src/platform/unix/*.c src/render/*.c -o rainy

clean:
	rm -f rainy
