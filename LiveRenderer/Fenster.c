//custom display library
//gcc Fenster.c -o fenster -lX11

#include <X11/Xlib.h>
#include <stdint.h>
#include "Fenster.h"

DispContext ContextInit(int res[], uint32_t *framebuffer){
	DispContext contextreturn = {};
	contextreturn.res[0] = res[0];
	contextreturn.res[1] = res[1];
	contextreturn.display = XOpenDisplay(NULL);
	Window root = DefaultRootWindow(contextreturn.display);
	contextreturn.screen = DefaultScreen(contextreturn.display);
	contextreturn.visual = DefaultVisual(contextreturn.display, contextreturn.screen);
	contextreturn.MainWindow = XCreateSimpleWindow(contextreturn.display, root, 100, 100, res[0], res[1], 0,0,0);
	
	contextreturn.image = XCreateImage(contextreturn.display, contextreturn.visual, DefaultDepth(contextreturn.display, contextreturn.screen), ZPixmap, 0, (char *)framebuffer, res[0], res[1], 32, 0);
	
	return contextreturn;
}

void CreateWindow(DispContext context){
	XMapWindow(context.display, context.MainWindow);
	XFlush(context.display);
}

void UpdateWindow(DispContext context){
	XPutImage(
		context.display,
		context.MainWindow,
		DefaultGC(context.display, context.screen),
		context.image,
		0, 0,
		0, 0,
		context.res[0], context.res[1]
	);
	XFlush(context.display);
}
