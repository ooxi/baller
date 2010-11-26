/*
    screen.c - Screen functions for Ballerburg

    Copyright (C) 2010  Thomas Huth

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <SDL.h>
#include <SDL_gfxPrimitives.h>

#include "screen.h"
#include "sdlgui.h"


SDL_Surface *surf;
Uint32 the_color;

void scr_init(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Could not initialize the SDL library:\n %s\n",
			SDL_GetError() );
		exit(-1);
	}

	surf = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE|SDL_HWPALETTE);
	if (!surf)
	{
		fprintf(stderr, "Could not initialize the SDL library:\n %s\n",
			SDL_GetError() );
		exit(-1);
	}

	SDLGui_Init();
	SDLGui_SetScreen(surf);
}

void scr_clear(void)
{
	SDL_Rect rect;

	puts("scr_clear");

	rect.x = 0;
	rect.y = 0;
	rect.w = 640;
	rect.h = 480;
	SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format,255,255,255));
}

int form_alert(int type, char *text)
{
	printf("Alert: %s\n", text);
	return 0;
}

void v_gtext(int handle, int x, int y, char *text)
{
	printf("v_gtext: %s\n", text);
}

void v_circle(int handle, int x, int y, int w)
{
	//printf("v_circle: %i %i %i\n", x,y,w);

	filledCircleColor(surf, x, y, w, the_color);

	SDL_UpdateRect(surf, 0,0, 640,480);
}

void movmem(void *src, void *dst, int size)
{
	// printf("movmem: %p -> %p\n", src, dst);
	memmove(dst, src, size);
}

void Giaccess(int a, int b)
{
	// printf("Giaccess %i %i\n", a, b);
}

void vswr_mode(short handle, short val)
{
	//printf("vswr_mode %i\n", val);
}

void vsf_style(short handle, short val)
{
	//printf("vsf_style %i\n", val);
}

void vsf_interior(short handle, short val)
{
	//printf("vsf_interior %i\n", val);
}

void vst_height(short handle, short height, short *val1, short *val2, short *val3, short *val4)
{
	//printf("vst_height %i\n", height);
}

void vst_color(short handle, short val)
{
	//printf("vst_color %i\n", val);
}

void v_bar(short handle, short *xy)
{
	SDL_Rect rect;
	Uint8 r, g, b;

	//printf("v_bar %i,%i -> %i,%i\n", xy[0], xy[1], xy[2], xy[3]);

	r = the_color >> 24;
	g = the_color >> 16;
	b = the_color >> 8;

	rect.x = xy[0];
	rect.y = xy[1];
	rect.w = xy[2]-xy[0]+1;
	rect.h = xy[3]-xy[1]+1;
	SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format,r,g,b));
	rectangleColor(surf, xy[0], xy[1], xy[2], xy[3], the_color);

	SDL_UpdateRect(surf, 0,0, 640,480);
}

void v_fillarea(short handle, short num, short xy[])
{
	int i;
	Sint16 vx[512], vy[512];

	//printf("v_fillarea %i\n", num);

	if (num > 512) {
		puts("v_fillarea overlow");
		exit(-2);
	}

	for (i = 0; i < num; i++) {
		vx[i] = xy[i*2];
		vy[i] = xy[i*2+1];
	}

	filledPolygonRGBA(surf, vx, vy, num, 60, 30, 30, 255);

	SDL_UpdateRect(surf, 0,0, 640,480);
}

void v_pline(short handle, short num, short xy[])
{
	int i;
	int maxx, maxy;
	int minx, miny;

	minx = 639; miny = 399;
	maxx = 0; maxy = 0;

	// printf("v_pline %i\n", num);

	for (i = 0; i < num-1; i++)
	{
		if (xy[i*2] < 0 || xy[i*2] > 639
		    || xy[i*2+1] < 0 || xy[i*2+1] > 479) {
			printf("bad coordinates!\n");
			return;
		}

		if (xy[i*2] > maxx)  maxx = xy[i*2];
		if (xy[i*2+1] > maxy)  maxy = xy[i*2+1];
		if (xy[i*2] < minx)  minx = xy[i*2];
		if (xy[i*2+1] < miny)  miny = xy[i*2+1];
		lineColor(surf, xy[i*2], xy[i*2+1], xy[i*2+2], xy[i*2+3], the_color|0x1000);
	}

	if (xy[i*2] > maxx)  maxx = xy[i*2];
	if (xy[i*2+1] > maxy)  maxy = xy[i*2+1];
	if (xy[i*2] < minx)  minx = xy[i*2];
	if (xy[i*2+1] < miny)  miny = xy[i*2+1];

	//printf("blit %i %i %i %i\n", minx,miny, maxx-minx+1,maxy-miny+1);
	SDL_UpdateRect(surf, minx,miny, maxx-minx+1,maxy-miny+1);
}

int scr_getpixel(int x, int y)
{
	Uint32 *p = surf->pixels;
	Uint32 c;
	SDL_PixelFormat *fmt = surf->format;

	c = p[y*640+x];
	c = ((((c & fmt->Rmask) >> fmt->Rshift) << fmt->Rloss) << 16)
	    | ((((c & fmt->Gmask) >> fmt->Gshift) << fmt->Gloss) << 8)
	    | (((c & fmt->Bmask) >> fmt->Bshift) << fmt->Bloss);

	return c;
}

void scr_color(int c)
{
	if (c)
		the_color = 0x203020ff;
	else
		the_color = 0xffffffff;
}
