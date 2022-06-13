#FLAGS= -Wall -ggdb -O0
FLAGS= -Wall -O3
#FLAGS= -s WASM=0
#FLAG2= --preload-file auto2.ppm --preload-file  auto2out.ppm

obligatorio: main imagen copia negativo reflejo bordes caricatura
		cc $(FLAGS) -o obligatorio main.o imagen.o copia.o negativo.o reflejo.o bordes.o caricatura.o -lm

main: main.c
		cc $(FLAGS) -c main.c

imagen: imagen.c
		cc $(FLAGS) -c imagen.c

copia: copia.c
		cc $(FLAGS) -c copia.c

negativo: negativo.c
		cc $(FLAGS) -c negativo.c

reflejo: reflejo.c
		cc $(FLAGS) -c reflejo.c

bordes: bordes.c
		cc $(FLAGS) -c bordes.c

caricatura: caricatura.c
		cc $(FLAGS) -c caricatura.c
