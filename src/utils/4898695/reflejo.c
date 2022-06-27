#include "imagen.h"

void reflejo(const Imagen img_in, int param, Imagen img_out)
{
    int i, j, i_mod, j_mod, horizontal, vertical;
    /*Si vertical u horizontal es 1, se invierte vertical u horizontalmente respectivamente.
    Particularmente las inversiones en ambos sentidos es igual al reflejo central*/
    vertical = (param == 1) || (param == 2)? 1 : 0;
    horizontal = (param == 0) || (param == 2)? 1 : 0;
    for(i = 0; i <= ((img_in.alto) - 1); i++)
        for(j = 0; j <= ((img_in.ancho) - 1); j++)
    	{
    	    i_mod = vertical? ((img_in.alto) - i - 1) : i;
    	    j_mod = horizontal? ((img_in.ancho) - j - 1) : j;
    	    *(img_out.pixels + conv_ind(i, j, (img_in.ancho))) = *(img_in.pixels + conv_ind(i_mod, j_mod, (img_in.ancho)));
    	}
}
