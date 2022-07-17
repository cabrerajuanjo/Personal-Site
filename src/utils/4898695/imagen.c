#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagen.h"
#include <ctype.h>

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*						CARGAR IMAGEN EN MEMORIA                       */
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

int leer_imagen(const char* ruta, Imagen* pimg)
{
	FILE *fimg;
	fimg = fopen(ruta, "rb");
	if (fimg != NULL)
	{
		char magic_num[3];
		int n, m, M;
		TipoImagen t_in;

		/*Leo encabezado de la imagen*/
		CodigoError err_leer_enc = leer_encabezado(fimg, magic_num, &n, &m, &M);
		if(err_leer_enc != PNM_OK) return err_leer_enc;


		printf("magic number : %s; n : %d ; m : %d ; M : %d\n", magic_num, n, m, M);

		if (strcmp(magic_num, "P2") == 0)
			t_in = GRISES;
		else if (strcmp(magic_num, "P6") == 0)
			t_in = COLOR;
		else
			return PNM_ENCABEZADO_INVALIDO;

		/*Asigo las propiedads de la imagen y reservo el espacio necesario para cargar la imagen en memoria*/
		if (inicializar_imagen(n, m, t_in, pimg) != PNM_OK) return PNM_ENCABEZADO_INVALIDO;

		fgetc(fimg);/*Posiciono el puntero en los datos de la imagen*/

		unsigned int i = 0;
		if((*pimg).tipo == COLOR)
		{
			while ((!feof(fimg)) && (i < (pimg->ancho) * (pimg->alto)))
			{
				#define CATCH_ERR_EOF_ENTRE_CAN(C) if((C) == EOF) return PNM_DATOS_INVALIDOS
				#define CATCH_ERR_EOF_PREMATURO(C) if(((C) == EOF) && ((i + 1) < (pimg->ancho) * (pimg->alto))) \
													return PNM_DATOS_INVALIDOS

				byte c, r, g, b;
				r = fgetc(fimg); CATCH_ERR_EOF_ENTRE_CAN(r);
				g = fgetc(fimg); CATCH_ERR_EOF_ENTRE_CAN(g);
				b = fgetc(fimg); CATCH_ERR_EOF_ENTRE_CAN(b);


				c = fgetc(fimg);
				CATCH_ERR_EOF_PREMATURO(c);
				#undef CATCH_ERR_EOF_ENTRE_CAN
				#undef CATCH_ERR_EOF_PREMATURO
				ungetc(c, fimg);

				Pixel pix;
				convertir_pixel_int(&r, &g, &b, &pix, "CanalesAInt");

				*((pimg->pixels) + i) = pix;
				i++;
			}
		}else
		{
			while ((!feof(fimg)) && (i < (((*pimg).ancho) * ((*pimg).alto))))
			{
				#define CATCH_ERR_LECTURA(C) if(C == EOF) return PNM_ERROR_LECTURA
				CATCH_ERR_LECTURA(fscanf(fimg, " %d ", ((pimg->pixels) + i)));
				i++;
				#undef CATCH_ERR_LECTURA
			}
		}
		fclose(fimg);
		return PNM_OK;
	}else
		return PNM_ARCHIVO_INEXISTENTE;
}

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*					GUARDAR IMAGEN EN DISCO 	                       */
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

int escribir_imagen(const Imagen* pimg, const char* ruta)
{
	FILE *fimg = fopen(ruta, "wb");
	if (fimg != NULL)
	{
		fprintf(fimg, "%s\n%d %d\n%d\n", "P6",
										 (*pimg).ancho,
										 (*pimg).alto,
										 (*pimg).valor_maximo);
		unsigned int i = 0;
		if((*pimg).tipo == COLOR)
		{
			Pixel pix;
			byte r, g, b;
			while(i < (((*pimg).ancho) * ((*pimg).alto)))
			{
				pix = *((pimg->pixels) + i);
				convertir_pixel_int(&r, &g, &b, &pix, "IntACanales");

				fputc(r, fimg); /*R*/fputc(g, fimg); /*G*/fputc(b, fimg);/*B*/

				i++;
			}
		}else
		{
			while(i < (((*pimg).ancho) * ((*pimg).alto)))
			{
				byte by = *((pimg->pixels) + i);
				fputc(by, fimg); /*R*/fputc(by, fimg); /*G*/fputc(by, fimg);/*B*/
				i++;
			}

		}
		fclose(fimg);
		return PNM_OK;
	}else
		return PNM_ARCHIVO_INEXISTENTE;
}

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*		  	    		 LECTURA DE ENCABEZADO			               */
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

#define CATCH_EOF_EN_ENCABEZADO if(c == EOF) return PNM_DATOS_INVALIDOS
#define CATCH_ENCABEZADO_INVALIDO if((c < '0') || (c > '9')) return PNM_ENCABEZADO_INVALIDO

CodigoError ignorar_coment_esp(FILE *f)
{
	char c;
	do
	{
		c = fgetc(f); CATCH_EOF_EN_ENCABEZADO;
	}
	while((c != '\n') && isspace(c));

	if (c != '\n')
		ungetc(c, f);
	else
	{
		c = fgetc(f);
		if(c == '#')
		{
			do /*Encuentro comentario*/
			{
				while((c = fgetc(f)) != '\n')
					CATCH_EOF_EN_ENCABEZADO;
				CATCH_EOF_EN_ENCABEZADO;
			}while((c = fgetc(f)) == '#');

			ungetc(c, f);
			ignorar_coment_esp(f);
		}else if (isspace(c))
		{
			ungetc(c, f);
			ignorar_coment_esp(f);
		}
		else
			ungetc(c, f);
	}
	return PNM_OK;
}

CodigoError leer_num(int *num, FILE *f)
{
	char *temp = NULL;
	char c;
	int i = 0;
	while(!isspace(c = fgetc(f)))
	{
		CATCH_EOF_EN_ENCABEZADO;
		CATCH_ENCABEZADO_INVALIDO;
		temp = (char*)realloc(temp, ((++i) + 1)*sizeof(char));
		*(temp + i - 1) = c;
	}
	*(temp + i) = '\0';
	ungetc(c, f);
	*num = atoi(temp);
	free(temp);
	return PNM_OK;
}

CodigoError leer_encabezado(FILE *f, char *Mn, int *n, int *m, int *M)
{
	#define CATCH_ERR_LEERNUM if(cod_temp == PNM_DATOS_INVALIDOS)\
						return PNM_DATOS_INVALIDOS;\
				  else if(cod_temp == PNM_ENCABEZADO_INVALIDO)\
						return PNM_ENCABEZADO_INVALIDO

	#define	CATCH_ERR_IGNOR_COM_ESP if(cod_temp == PNM_DATOS_INVALIDOS) return PNM_DATOS_INVALIDOS


	CodigoError cod_temp;

	Mn[0] = fgetc(f); Mn[1] = fgetc(f); Mn[2] = '\0';
	cod_temp = ignorar_coment_esp(f); CATCH_ERR_IGNOR_COM_ESP;

	cod_temp = leer_num(n, f); CATCH_ERR_LEERNUM;
	cod_temp = ignorar_coment_esp(f); CATCH_ERR_IGNOR_COM_ESP;


	cod_temp = leer_num(m, f); CATCH_ERR_LEERNUM;
	cod_temp = ignorar_coment_esp(f); CATCH_ERR_IGNOR_COM_ESP;

	cod_temp = leer_num(M, f); CATCH_ERR_LEERNUM;


	#undef CATCH_ERR_LEERNUM
	#undef	CATCH_ERR_IGNOR_COM_ESP
	return PNM_OK;
}

#undef CATCH_EOF_EN_ENCABEZADO
#undef CATCH_ENCABEZADO_INVALIDO

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*		  				      	 OTRAS			                       */
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
int inicializar_imagen(int ancho, int alto, TipoImagen tipo, Imagen *pnew)
{
	(*pnew).tipo = tipo;
	(*pnew).ancho = ancho;
	(*pnew).alto = alto;
	(*pnew).valor_maximo = 255;
	return ((*pnew).pixels = (Pixel*)(malloc(sizeof(Pixel)*(ancho*alto))))== NULL ? PNM_ENCABEZADO_INVALIDO : PNM_OK;
}

void duplicar_imagen(const Imagen* pin, Imagen* pout)
{
	inicializar_imagen((*pin).ancho, (*pin).alto, (*pin).tipo, pout);
}

void destruir_imagen(Imagen* pimg)
{
	(*pimg).tipo = 0;
	(*pimg).ancho = 0;
	(*pimg).alto = 0;
	(*pimg).valor_maximo = 0;
	free((*pimg).pixels);
	pimg->pixels = NULL;
}

bool tiene_ext_valida(char *ruta)
{
	char *ext = strrchr(ruta, '.') + 1;
	if ((ext == NULL) || (!(strcmp(ext, "pgm") == 0) || !(strcmp(ext, "ppm") == 0)))
	{
		printf("Aviso: No se reconoce la extencion del archivo o este no posee una.\n");
		return FALSE;
	}else
		return TRUE;
}

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*		  				       OTRAS AUXILIARES       	               */
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

int conv_ind(int fila, int col, int ancho)
{
    return ((fila*ancho) + col);
}

int redondear(double x)
{
    return ((int)(x + 0.5));
}

int clipear(int coordenada, int limite_inf, int limite_sup)
{
    if (coordenada < limite_inf)
	return 0;
    else if (coordenada >= limite_sup)
	return limite_sup;
    else
	return coordenada;
}

void convertir_pixel_int(byte *r, byte *g, byte *b, Pixel *pix, char *op)
{
	if (strcmp(op, "CanalesAInt") == 0)
	{
		unsigned int masc;
		*pix = 0;
		masc = *r;
		masc <<= 16;
		*pix |= masc;
		masc = *g;
		masc <<= 8;
		*pix |= masc;
		masc = *b;
		*pix |= masc;
	}else if (strcmp(op, "IntACanales") == 0)
	{
		Pixel ptemp = *pix;

		*b = ptemp;
		ptemp >>= 8;
		*g = ptemp;
		ptemp >>= 8;
		*r = ptemp;
	}
}

void convertir_a_grises(const Imagen in, Imagen out)
{
	int i;
	byte r, g, b;
	for(i = 0; i < ((in.ancho)*(in.alto)); i++)
	{
		convertir_pixel_int(&r, &g, &b, in.pixels + i, "IntACanales");
		*(out.pixels + i) = redondear(0.2*r + 0.7*g + 0.1*b);
	}
}
