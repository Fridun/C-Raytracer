//gcc MAIN.c liverenderer.c Fenster.c -o run -lm -lX11

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#include "liverenderer.h"
#include "Fenster.h"


void main(){

	int res[] = {720, 450};

	uint32_t *renderimage = malloc(res[0] * res[1] * sizeof(uint32_t));

	
	
//++++++++++++++++++++++
	int scale = 4;
	
	int displayres[] = {res[0]*scale, res[1]*scale};

	uint32_t *framebuffer = malloc(displayres[0] * displayres[1] * sizeof(uint32_t));


	
	DispContext window1context = ContextInit(displayres, framebuffer);
	
	CreateWindow(window1context);

	while(1){ 
		struct timespec starttime, endtime;
		clock_gettime(CLOCK_MONOTONIC, &starttime);
		
		render(renderimage, res);
		
		clock_gettime(CLOCK_MONOTONIC, &endtime);
		double rendertime = ((endtime.tv_sec - starttime.tv_sec) + ((endtime.tv_nsec - starttime.tv_nsec)/1000000000.0));
		
		printf("Render took: %lf seconds\n", rendertime); //comment this if you dont want it to spam your terminal
	
		for (int p = 0; p < res[0] * res[1] ; p++){
		
			for (int r = 0 ; r < scale ; r++){
			
				for (int i = 0; i < scale; i++){
					//printf("%d\n", (i + (displayres[0]*r) + ((p/res[0]) * scale * displayres[0]) + (p*scale)));
					framebuffer[i + (displayres[0]*r) + ((p/res[0] * scale * displayres[0]) + ((p % res[0]) *scale))] = renderimage[p];
				}
			}
		}
		UpdateWindow(window1context); }
	
	
}
