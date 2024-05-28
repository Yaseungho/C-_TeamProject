main : main.o InputManager.o ItemSpawner.o Snake.o WindowManager.o
	g++ -o main main.o InputManager.o ItemSpawner.o Snake.o WindowManager.o -lncurses

main.o : main.cpp
	g++ -c -o main.o main.cpp -lncurses

InputManager.o : InputManager.cpp
	g++ -c -o InputManager.o InputManager.cpp -lncurses

ItemSpawner.o : ItemSpawner.cpp
	g++ -c -o ItemSpawner.o ItemSpawner.cpp -lncurses

Snake.o : Snake.cpp
	g++ -c -o Snake.o Snake.cpp -lncurses

WindowManager.o : WindowManager.cpp
	g++ -c -o WindowManager.o WindowManager.cpp -lncurses
