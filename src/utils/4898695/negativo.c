#include "imagen.h"

void negativo(const Imagen img_in, Imagen img_out)
{
	int i;
	for(i = 0; (i < ((img_in.ancho)*(img_in.alto))); i++)
	{
		if( img_in.tipo == COLOR)
		{
			Pixel pix = *((img_in.pixels) + i);
			byte r, g, b;
			convertir_pixel_int(&r, &g, &b, &pix, "IntACanales");
			r = ((img_in.valor_maximo) - r); g = ((img_in.valor_maximo) - g); b = ((img_in.valor_maximo) - b);
			convertir_pixel_int(&r, &g, &b, &pix, "CanalesAInt");
			*((img_out.pixels) + i) = pix;
		}else
			*((img_out.pixels) + i) = (img_in.valor_maximo) - *((img_in.pixels) + i);
	}
}
