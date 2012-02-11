/*
    screen.c - Screen functions for Ballerburg

    Copyright (C) 2010, 2011  Thomas Huth

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

#include "i18n.h"
#include "screen.h"
#include "sdlgui.h"


SDL_Surface *surf;
Uint32 the_color, fill_color;
Uint32 bg_color;

int fill_style, fill_interior;

static SGOBJ donebuttondlg[] =
{
	{ SGBOX, 0, 0, 36,29, 8,1, NULL },
	{ SGBUTTON, SG_EXIT, 0, 0,0, 8,1, N_("Done") }
};


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

	SDL_WM_SetCaption("Ballerburg SDL", "Ballerburg");

	bg_color = SDL_MapRGB(surf->format,0xe0,0xf0,0xff);

	SDLGui_Init();
	SDLGui_SetScreen(surf);
}

void scr_clear(void)
{
	SDL_Rect rect;
	Uint32 white;
	int i;

	white = SDL_MapRGB(surf->format,0xff,0xff,0xff);

	rect.x = 0;
	rect.y = 0;
	rect.w = 640;
	rect.h = 400;
	SDL_FillRect(surf, &rect, bg_color);

	for (i = 0; i < 80; i += 1)
	{
		rect.x = 0;
		rect.y = 479-i;
		rect.w = 640;
		rect.h = 1;
		SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format,8+i/2,32+i,8+i/2));
	}

	/* Left vane box: */
	rect.x = 5; rect.y = 410;
	rect.w = 104; rect.h = 48+16;
	for (i = 1; i < 5; i++)
	{
		rectangleRGBA(surf, rect.x-i, rect.y-i,
			rect.x+rect.w-1+i, rect.y+rect.h-1+i,
			0xf0, 0xff, 0xf0, 0xff-i*0x3c);
	}
	SDL_FillRect(surf, &rect, white);

	/* Right vane box: */
	rect.x = 5+(629-104); rect.y = 410;
	rect.w = 104; rect.h = 48+16;
	for (i = 1; i < 5; i++)
	{
		rectangleRGBA(surf, rect.x-i, rect.y-i,
			rect.x+rect.w-1+i, rect.y+rect.h-1+i,
			0xf0, 0xff, 0xf0, 0xff-i*0x3c);
	}
	SDL_FillRect(surf, &rect, white);
}

int form_alert(int type, char *text)
{
	printf("Alert: %s\n", text);
	return 0;
}

void v_gtext(int handle, int x, int y, char *text)
{
	SDL_Rect rect;

	// printf("v_gtext: %s\n", text);

	y -= 12;

	rect.x = x;
	rect.y = y;
	rect.w = strlen(text)*8;
	rect.h = 16;
	SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format,0xff,0xff,0xff));

	SDLGui_Text(x, y, text);

	SDL_UpdateRects(surf, 1, &rect);
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

void vswr_mode(short handle, short val)
{
	//printf("vswr_mode %i\n", val);
}

static void update_fill_color(void)
{
	if (fill_interior == 1)
	{
		fill_color = 0x000000ff;
	}
	else if (fill_interior == 2)
	{
		switch (fill_style)
		{
		case 2:  fill_color = 0x602060ff; break;   // King color
		case 9:  fill_color = 0x909080ff; break;   // Wall color
		case 11:  fill_color = 0xc04020ff; break;  // Roof color
		}
	}
	else
	{
		puts("unknown fill interior");
	}
}

void vsf_style(short handle, short val)
{
	fill_style = val;
	update_fill_color();
}

void vsf_interior(short handle, short val)
{
	fill_interior = val;
	update_fill_color();
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


void clr(short x, short y, short w, short h)
{
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format,0xff,0xff,0xff));

	SDL_UpdateRect(surf, x,y, w,h);
}


void clr_bg(short x, short y, short w, short h)
{
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_FillRect(surf, &rect, bg_color);

	SDL_UpdateRect(surf, x,y, w,h);
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

	filledPolygonColor(surf, vx, vy, num, fill_color);

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


void scr_line(int x1, int y1, int x2, int y2, int rgb)
{
	int minx, miny, maxx, maxy;

	if (x1 < x2)
	{
		minx = x1;
		maxx = x2;
	}
	else
	{
		minx = x2;
		maxx = x1;
	}

	if (y1 < y2)
	{
		miny = y1;
		maxy = y2;
	}
	else
	{
		miny = y2;
		maxy = y1;
	}

	lineColor(surf, x1, y1, x2, y2, (rgb<<8)|0xff);
	SDL_UpdateRect(surf, minx, miny, maxx-minx+1, maxy-miny+1);
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
	the_color = (c << 8) | 0xff;
}

void scr_fillcolor(int c)
{
	fill_color = (c << 8) | 0xff;
}


/**
 * Select foreground (1) or background (0) color
 */
void color(int c)
{
	if (c)
		the_color = 0x000000ff;
	else
		the_color = (bg_color<<8) | 0x0ff;
}


void scr_init_done_button(int *bx, int *by, int *bw, int *bh)
{
	int fontw, fonth;

	/* Calculate the "Done" button coordinates */
	SDLGui_GetFontSize(&fontw, &fonth);
	*bx = donebuttondlg[0].x * fontw;
	*by = donebuttondlg[0].y * fonth;
	*bw = donebuttondlg[0].w * fontw;
	*bh = donebuttondlg[0].h * fonth;

}

void scr_draw_done_button(int selected)
{
	if (selected)
		donebuttondlg[1].state |= SG_SELECTED;
	else
		donebuttondlg[1].state &= ~SG_SELECTED;

	SDLGui_DrawButton(donebuttondlg, 1);

	SDL_UpdateRect(surf, 0,0, 0,0);
}
