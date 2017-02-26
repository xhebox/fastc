fastc: main.c
	cc $^ -o $@ -luv -Wall

clean:
	rm fastc
