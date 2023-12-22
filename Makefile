main: ./src/main.c
	gcc -o ./app/image_editor ./src/main.c -I./src -lm
