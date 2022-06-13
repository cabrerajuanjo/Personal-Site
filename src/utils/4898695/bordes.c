#include <string.h>
#include <math.h>
#include "imagen.h"
#include "filtros.h"

/*Para direc "h", se calcula absoluto de gradiente horizontal. Para direc "v", vertical.*/
void abs_grad(const Imagen img_in, char direc[], Imagen img_out)
{
    /*Con i_add y j_add se modifica el sentido del vector cuyo gradiente se quiere calcular*/
	int i_add = (strcmp(direc, "v") == 0)? 1 : 0;
	int j_add = (strcmp(direc, "h") == 0)? 1 : 0;
	int i, j;
    
	for(i = 0; i <= (img_in.alto - 1); i++)
		for(j = 0; j <= (img_in.ancho - 1); j++)
		{
			int coor_conv;
			/*Hallo el primer termino*/
			coor_conv = conv_ind(clipear(i - i_add, 0, img_in.alto - 1), clipear(j - j_add, 0, img_in.ancho - 1), img_in.ancho);
			int prim_pix = *(img_in.pixels + coor_conv);
			/*Hallo el segundo termino*/
			coor_conv = conv_ind(clipear(i + i_add, 0, img_in.alto - 1), clipear(j + j_add, 0, img_in.ancho - 1), img_in.ancho);
			int sig_pix = *(img_in.pixels + coor_conv);

			/*Calculo el valor absoluto del gradiente y lo asigno a la salida*/
			if ((sig_pix - prim_pix) >= 0)
				*(img_out.pixels + conv_ind(i, j, img_in.ancho)) = sig_pix - prim_pix;
			else
				*(img_out.pixels + conv_ind(i, j, img_in.ancho)) = (-1)*(sig_pix - prim_pix);
		}
}

void bordes(const Imagen img_in, int param, Imagen img_out)
{
	/*Convierto la imagen de entrada a imagen en grises*/
	Imagen grises;
	inicializar_imagen(img_in.ancho, img_in.alto, GRISES, &grises);
	if(img_in.tipo == COLOR)
		convertir_a_grises(img_in, grises);
	else
		copia(img_in, grises);


	switch(param)
	{
	case 0: 	
			abs_grad(grises, "h", img_out);
			break;
	case 1: 	
			abs_grad(grises, "v", img_out);
			break;
	case 2: {
			Imagen grad_x;
			Imagen grad_y;
			inicializar_imagen(grises.ancho, grises.alto, grises.tipo, &grad_x);
			inicializar_imagen(grises.ancho, grises.alto, grises.tipo, &grad_y);
			abs_grad(grises, "h", grad_x);/*Hallo gradiente horizontal*/
			abs_grad(grises, "v", grad_y);/*Hallo gradiente vertical*/

			int mod_grad;
			int i;
			for(i = 0; i < ((grises.ancho)*(grises.alto)); i++)
			{
				/*Hallo modulo de gradiente*/
				mod_grad = redondear(sqrt( (*(grad_x.pixels + i))*(*(grad_x.pixels + i)) + (*(grad_y.pixels + i))*(*(grad_y.pixels + i)) ));
				if (mod_grad <= (img_in.valor_maximo))
					*(img_out.pixels + i) = mod_grad;
				else
					*(img_out.pixels + i) = img_in.valor_maximo;
			}
			destruir_imagen(&grad_x);
			destruir_imagen(&grad_y);
			break;
		}
	}
	destruir_imagen(&grises);
}
