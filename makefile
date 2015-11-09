exe:	main.cpp
	g++ *.cpp -Iincludes

run:
	./a.out

client:
	./a.out localhost

r:
	./a.out

clean:
	rm a.out

c:
	rm a.out