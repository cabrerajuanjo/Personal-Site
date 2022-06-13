#include "imagen.h"

void copia(const Imagen img_in, Imagen img_out)
{
	int i;
	for(i = 0; i < ((img_in.ancho)*(img_in.alto)); i++)
		*(img_out.pixels + i) = *(img_in.pixels + i);
}
