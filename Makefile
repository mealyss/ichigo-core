all: rainy

rainy: clean
	gcc src/*.c src/format/*.c src/render/*.c -o rainy
	clear
	./rainy 

clean:
	rm -f rainy
