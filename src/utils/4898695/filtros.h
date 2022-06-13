#ifndef filtros
#define filtros
#include "imagen.h"
/**
*Toma un arreglo de pixeles y le realiza una copia exacta
*
*/
void copia(const Imagen img_in, Imagen img_out);

/**
*Toma un arreglo de pixeles y le aplica un filtro de negativo que consiste en invertir los colores relativo a la escala de colores
*/
void negativo(const Imagen img_in, Imagen img_out);

/**
*Toma un arreglo de pixeles y le aplica un filtro de reflejo que consiste básicamente invertir una imagen dependiendo del parametro
*/
void reflejo(const Imagen img_in, int param, Imagen img_out);

/**
*Toma un arreglo de pixeles y le resalta los bordes
*
*Para param 0: valor absoluto de gradiente horizontal, 
*para param 1: valor absoluto de gradiente vertical,
*para param 2: modulo del gradiente.
*/
void bordes(const Imagen img_in, int param, Imagen img_out);

void caricatura(const Imagen img_in, int umbral, Imagen img_out);
#endif
