#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "filtros.h"
#include "imagen.h"

#define COMANDO argv[1]
#define PARAMETRO atoi(argv[2])
#define ENTRADA argv[3]
#define SALIDA argv[4]

// #define COMANDO "negativo"
// #define PARAMETRO 0
// #define ENTRADA "auto2.ppm"
// #define SALIDA "auto2out.ppm"

int impr_msj_err(CodigoError Cod, char *accion, char *ruta);

int main(int argc, char *argv[])
{
	Imagen imagen_entrada;
	Imagen imagen_salida;

	printf(argv[0]);
	printf(argv[1]);
	printf(argv[2]);
	printf(argv[3]);
	printf(argv[4]);
	printf(argv[5]);

	if (argc != 5)
	{
		printf("Cantidad de comandos introducidos inválida\nUso: <comando> <parametro> <ruta_entrada> <ruta_salida>\n\n");
		return 1;
	}

	/*--------------------------------------------------------------------------------------*/
	/*Carga la imagen en imagen_abierta a la vez que verifica si ha ocurrido bien el proceso*/
	/*--------------------------------------------------------------------------------------*/

	if(impr_msj_err(leer_imagen(ENTRADA, &imagen_entrada), "Leer Imagen", ENTRADA) != 0)
		return 1;

	/*--------------------------*/
	/*Interpretacion de comandos*/
	/*--------------------------*/

	if(strcmp(COMANDO, "copy") == 0)
	{
		inicializar_imagen(imagen_entrada.ancho, imagen_entrada.alto, imagen_entrada.tipo, &imagen_salida);
		copia(imagen_entrada, imagen_salida);
	}

	else if(strcmp(COMANDO, "negative") == 0)
	{
		inicializar_imagen(imagen_entrada.ancho, imagen_entrada.alto, imagen_entrada.tipo, &imagen_salida);
		negativo(imagen_entrada, imagen_salida);
	}

	else if(strcmp(COMANDO, "mirror") == 0)
	{
		inicializar_imagen(imagen_entrada.ancho, imagen_entrada.alto, imagen_entrada.tipo, &imagen_salida);
		reflejo(imagen_entrada, PARAMETRO, imagen_salida);
	}

	else if(strcmp(COMANDO, "edges") == 0)
	{
		inicializar_imagen(imagen_entrada.ancho, imagen_entrada.alto, GRISES, &imagen_salida);
		bordes(imagen_entrada, PARAMETRO, imagen_salida);
	}

	else if(strcmp(COMANDO, "caricature") == 0)
	{
		inicializar_imagen(imagen_entrada.ancho, imagen_entrada.alto, imagen_entrada.tipo, &imagen_salida);
		caricatura(imagen_entrada, PARAMETRO, imagen_salida);
	}

	else
	{
		printf("Error: Ha introducido un comando inválido\n\
				Uso: <comando> <parametro> <ruta_entrada> <ruta_salida>\n\n");
		return 1;
	}

	/*----------------------------------*/
	/*Escribe la imagenen un archivo PNM*/
	/*----------------------------------*/

	if(impr_msj_err(escribir_imagen(&imagen_salida, SALIDA), "Guardar Imagen", SALIDA) != 0)
		return 1;


	destruir_imagen(&imagen_entrada);
	destruir_imagen(&imagen_salida);
	return 0;
}


int impr_msj_err(CodigoError cod, char *accion, char *ruta)
{
	switch (cod)
	{
		case PNM_OK:
			printf("%s ha ocurrido sastifacoriamente.\n", accion);
			return 0;
			break;
		case PNM_ARCHIVO_INEXISTENTE:
			printf("Error al %s: \nNo se ha podido abrir el archivo %s, este no existe o no se encuentra en tal directorio\n", accion, ruta);
			return 1;
			break;
		case PNM_ERROR_LECTURA:
			printf("Error al %s: \nNo se ha podido cargar la imaagen %s\n", accion, ruta);
			return 1;
			break;
		case PNM_ENCABEZADO_INVALIDO:
			printf("Error al %s: \nEl encabezado del archivo %s presenta un formato inválido(verificar la integridad del archivo)\n", accion, ruta);
			return 1;
			break;
		case PNM_DATOS_INVALIDOS:
			printf("Error al %s: \nLos datos de la imagen %s presentan un formato inválido(verificar la integridad del archivo)\n", accion, ruta);
			return 1;
			break;
		case PNM_ERROR_ESCRITURA:
			printf("Error al %s: \nNo se pudo guardar la imagen %s\n", accion, ruta);
			return 1;
			break;
		default:
			printf("Codigo de error desconocido\n");
			return 0;
	}
}
