#ifndef FENSTER_H
#define FENSTER_H


#include <X11/Xlib.h>
#include <stdint.h>

typedef struct DispContext{
	int res[2];
	Display *display;
	Window MainWindow;
	int screen;
	Visual *visual;
	XImage *image;
} DispContext;

DispContext ContextInit(int res[], uint32_t *framebuffer);

void CreateWindow(DispContext context);

void UpdateWindow(DispContext context);

#endif
