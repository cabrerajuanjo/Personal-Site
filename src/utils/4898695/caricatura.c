#include <stdlib.h>
#include <stdio.h>
#include "filtros.h"
#include "imagen.h"

bool es_borde(const Imagen in, int i, int j, int umbral)
{
	if(((i < 0) || (j < 0)) || (*(in.pixels + conv_ind(i, j, in.ancho)) > umbral))
		return TRUE;
	else
		return FALSE;
}

void caricatura(const Imagen img_in, int umbral, Imagen img_out)
{
	Imagen img_bordes;
	inicializar_imagen(img_in.ancho, img_in.alto, GRISES, &img_bordes);
	bordes(img_in, 2, img_bordes);
	
	typedef struct
	{
		unsigned int inicio;
		unsigned int tamanio;
		union
		{
			struct
			{
				unsigned int sum_red;
				unsigned int sum_green;
				unsigned int sum_blue;
			}color;
			unsigned int sum_grey;
		}suma_pixel;
		Pixel promedio;
	} Region;

	Region *region = NULL;

	/*||||||||*/
	/*Etiqueto*/
	/*||||||||*/
	Imagen img_etiquetado;
	inicializar_imagen(img_bordes.ancho, img_bordes.alto, GRISES, &img_etiquetado);
	register int i, j, centro, arriba, izquierda;
	unsigned int etiq = 0;
	for(i = 0; i < img_bordes.alto; i++)
		for(j = 0; j < img_bordes.ancho; j++)
		{
			centro = conv_ind(i, j, img_bordes.ancho);
			arriba = conv_ind(i - 1, j, img_bordes.ancho);
			izquierda = conv_ind(i, j - 1, img_bordes.ancho);
			if(es_borde(img_bordes, i, j, umbral))/*Si es borde*/
				*(img_etiquetado.pixels + centro) = 0;
			else/*Para etiquetas no bordes*/
			{
				if(es_borde(img_bordes, i - 1, j, umbral) && es_borde(img_bordes, i, j - 1, umbral))
				{/*Encuentra una nueva region, cambia la etiqueta y marca el inicio*/
					(*(img_etiquetado.pixels + centro)) = ++etiq;
					region = realloc(region, etiq*sizeof(Region));
					(region + (etiq - 1))->inicio = i;/*Marco inicio de region(fila)*/
					(region + (etiq - 1))->tamanio = 0;
					(region + (etiq - 1))->suma_pixel.color.sum_red = 0;
					(region + (etiq - 1))->suma_pixel.color.sum_green = 0;
					(region + (etiq - 1))->suma_pixel.color.sum_blue = 0;
					(region + (etiq - 1))->suma_pixel.sum_grey = 0;
				}
				/*En el caso de que no sea una nueva region*/
				else if(es_borde(img_bordes, i, j - 1, umbral))
					*(img_etiquetado.pixels + centro) = *(img_etiquetado.pixels + arriba);
				else if(es_borde(img_bordes, i - 1, j, umbral))
					*(img_etiquetado.pixels + centro) = *(img_etiquetado.pixels + izquierda);
				else
				{
					*(img_etiquetado.pixels + centro) = *(img_etiquetado.pixels + arriba);
					if(*(img_etiquetado.pixels + izquierda) != *(img_etiquetado.pixels + arriba))
					{
						int etiq_vieja = *(img_etiquetado.pixels + izquierda);
						if((region + *(img_etiquetado.pixels + arriba) - 1)->inicio > (region + *(img_etiquetado.pixels + izquierda) - 1)->inicio)
							(region + *(img_etiquetado.pixels + arriba) - 1)->inicio = (region + *(img_etiquetado.pixels + izquierda) - 1)->inicio;
						int subi, subj;
						for(subi = (region + (etiq_vieja) - 1)->inicio; subi <= i; subi++)
							for(subj = 0; subj < img_bordes.ancho; subj++)
								if(*(img_etiquetado.pixels + conv_ind(subi, subj, img_bordes.ancho)) == etiq_vieja)
									*(img_etiquetado.pixels + conv_ind(subi, subj, img_bordes.ancho)) = *(img_etiquetado.pixels + centro);		
					}			
				}
			}
		}

	/*|||||||||||||||||||||||||||||||||||||||||||||||||*/
	/*Compilo informacion sobre las diferentes regiones*/
	/*|||||||||||||||||||||||||||||||||||||||||||||||||*/
	
	for(i = 0; i < img_etiquetado.alto; i++)
	{
		for(j = 0; j < img_etiquetado.ancho; j++)
		{
			centro = conv_ind(i, j, img_bordes.ancho);
			if(*(img_etiquetado.pixels + centro) > 0)
			{
				(region + *(img_etiquetado.pixels + centro) - 1)->tamanio++;
				if(img_in.tipo == COLOR)
				{
					byte r, g, b;
					convertir_pixel_int(&r, &g, &b, img_in.pixels + centro, "IntACanales");
					(region + *(img_etiquetado.pixels + centro) - 1)->suma_pixel.color.sum_red += r;
					(region + *(img_etiquetado.pixels + centro) - 1)->suma_pixel.color.sum_green += g;
					(region + *(img_etiquetado.pixels + centro) - 1)->suma_pixel.color.sum_blue += b;
				}else
					(region + *(img_etiquetado.pixels + centro) - 1)->suma_pixel.sum_grey += *(img_in.pixels + centro);
			}
		}
	}
	
	/*|||||||||||||||||||||||||||||||||||*/
	/*Calculo el promedio de las regiones*/
	/*|||||||||||||||||||||||||||||||||||*/
	unsigned int etiq_actual;
	for(etiq_actual = 1; etiq_actual <= etiq; etiq_actual++)
		if((region + (etiq_actual - 1))->tamanio != 0)
		{
			Pixel pixel_p;
			if(img_in.tipo == COLOR)
			{
				byte r_p, g_p, b_p;
				unsigned int tam = (region + (etiq_actual - 1))->tamanio;

				r_p = redondear( ((region + (etiq_actual - 1))->suma_pixel.color.sum_red) / tam );
				g_p = redondear( ((region + (etiq_actual - 1))->suma_pixel.color.sum_green) / tam );
				b_p = redondear( ((region + (etiq_actual - 1))->suma_pixel.color.sum_blue) / tam );
				convertir_pixel_int(&r_p, &g_p, &b_p, &pixel_p, "CanalesAInt");
				(region + (etiq_actual - 1))->promedio = pixel_p;
			}else
			{
				pixel_p = redondear(((region + (etiq_actual - 1))->suma_pixel.sum_grey) / (region + (etiq_actual - 1))->tamanio);
				(region + (etiq_actual - 1))->promedio = pixel_p;
			}
		}

	/*|||||||*/
	/*Relleno*/
	/*|||||||*/
	for(i = 0; i < img_etiquetado.alto; i++)
		for(j = 0; j < img_etiquetado.ancho; j++)
		{
			centro = conv_ind(i, j, img_bordes.ancho);
			etiq_actual = *(img_etiquetado.pixels + centro);
			*(img_out.pixels + centro) = etiq_actual > 0? (region + (etiq_actual - 1))->promedio : 0;
		}
	
	free(region);	
	destruir_imagen(&img_bordes);
	destruir_imagen(&img_etiquetado);
}
