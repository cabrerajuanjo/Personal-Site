#ifndef libio
#define libio
#include <stdio.h>
#endif

#ifndef imagen
#define imagen

typedef int Pixel;

typedef enum {PNM_OK, PNM_ARCHIVO_INEXISTENTE, PNM_ERROR_LECTURA, PNM_ENCABEZADO_INVALIDO, PNM_DATOS_INVALIDOS, PNM_ERROR_ESCRITURA} CodigoError;

typedef enum tipo_imagen {GRISES, COLOR} TipoImagen;

typedef enum canal {ROJO, VERDE, AZUL} Canal;

typedef struct imagen
{
	TipoImagen tipo;
	int ancho;
	int alto;
	int valor_maximo;
	Pixel *pixels;
} Imagen;

typedef unsigned char byte;

typedef enum {FALSE, TRUE} bool;

int inicializar_imagen(int ancho, int alto, TipoImagen tipo, Imagen* pnew);

void destruir_imagen(Imagen* pimg);

void duplicar_imagen(const Imagen* pin, Imagen* pout);

int leer_imagen(const char* ruta, Imagen* pimg);

int escribir_imagen(const Imagen* pimg, const char* ruta);

/*Verifica si una ruta tiene un extención valida(pgm o ppm) devuelve TRUE si lo es y FALSE si no.*/

bool tiene_ext_valida(char *ruta);

/*Lee el encablezado(los metadatos) de una imagen
*1 param: puntero al archivo imagen(ENTRADA)
*2 param: puntero a char(string) para numero mágico(SALIDA)
*3 param: puntero a entero para el ancho(SALIDA)
*4 param: puntero a entero para el alto(SALIDA)
*5 param: puntero a entero para el maximo valor por canal*/
CodigoError leer_encabezado(FILE *f, char *Mn, int *n, int *m, int *M);
#endif

/**
*Convierte cooredenadas para arreglos bidimencionales a coordenadas para arreglos unidimencionales.
*El arreglo unidimencional debe representar al bidimencional por filas.
*
*@param fila coordenada vertical
*@param col coordenada horizontal
*@param ancho ancho del arreglo bidimencional
*@return arreglo coordenada del arreglo unidimencional
*/
#ifndef convind
#define convind

int conv_ind(int fila, int col, int ancho);

#endif
/**
*Redondea un numero flotante
*
*@param x numero flotante a redondear
*@return numero x redondeado
*/ 
#ifndef redon
#define redon

int redondear(double x);

#endif
/**
*Realiza clipping a una coordenada dados limite inferior y superior, o sea acota la coordenada a el intervalo[limite inferior, limite superior]
*
*@param coordenada coordenada a clipear
*@param limite_inf limite inferior
*@param limite_sup limite superior
*@return coordenada clipeada
*/
#ifndef clip
#define clip

int clipear(int coordenada, int limite_inf, int limite_sup);

#endif
/**
*Realiza la convercion entre lo diferentes canales de los pixeles y un entero bidireccionalmente segun la opcion
*
*@param r canal rojo
*@param g canal verde
*@param b canal azul
*@param pix pixel en forma de entero(los canales acomodados en los bites del entero de los menos significatibos a los mas)
*@param op cadena de caracteres que indica en que direccion sucedera la conversion("pai" para convertir de pixel a entero y "iap" viceversa)
*@return --
*/
#ifndef conv_pix_int
#define conv_pix_int
void convertir_pixel_int(byte *r, byte *g, byte *b, Pixel *pix, char *op);
#endif

#ifndef conv_grises
#define conv_grises
/*Convierte imagen in a escala de grises y la devuelve por imagen out*/
void convertir_a_grises(const Imagen in, Imagen out);
#endif
