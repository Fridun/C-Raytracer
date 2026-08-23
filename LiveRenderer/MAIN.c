//gcc MAIN.c liverenderer.c Fenster.c -o run -lm -lX11

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#include "liverenderer.h"
#include "Fenster.h"


void main(){

	int res[] = {100, 100};

	uint32_t *renderimage = malloc(res[0] * res[1] * sizeof(uint32_t));

	render(renderimage, res);
	
//++++++++++++++++++++++
	int scale = 10;
	
	int displayres[] = {res[0]*scale, res[1]*scale};

	uint32_t *framebuffer = malloc(displayres[0] * displayres[1] * sizeof(uint32_t));

	for (int p = 0; p < res[0] * res[1] ; p++){
		
		for (int r = 0 ; r < scale ; r++){
			
			for (int i = 0; i < scale; i++){
				//printf("%d\n", (i + (displayres[0]*r) + ((p/res[0]) * scale * displayres[0]) + (p*scale)));
				framebuffer[i + (displayres[0]*r) + ((p/res[0] * scale * displayres[0]) + ((p % res[0]) *scale))] = renderimage[p];
			}
		}
	}
	
	DispContext window1context = ContextInit(displayres, framebuffer);
	
	CreateWindow(window1context);

	while(1){ UpdateWindow(window1context); }
	
	
}
