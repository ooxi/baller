/*
    screen.h - prototypes and definitions for screen.c

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

#include <SDL.h>

extern SDL_Surface *surf;

void v_pline(short handle, short num, short xy[]);
void v_fillarea(short handle, short num, short xy[]);
void v_circle(int handle, int x, int y, int w);
void v_bar(short handle, short *xy);
void clr(short x, short y, short w, short h);
void clr_bg(short x, short y, short w, short h);
void vsf_interior(short handle, short val);
void v_gtext(int handle, int x, int y, const char *text);
void vsf_style(short handle, short val);
void vswr_mode(short handle, short val);
void vst_color(short handle, short val);


void scr_init(void);
void scr_clear(void);
void scr_line(int x1, int y1, int x2, int y2, int rgba);
int scr_getpixel(int x, int y);
void scr_color(int c);
void scr_fillcolor(int c);
void color(int a);
void scr_init_done_button(int *bx, int *by, int *bw, int *bh);
void scr_draw_done_button(int selected);
void scr_ctr_text(int cx, int y, const char *text);
void scr_cannonball(int x, int y);
void *scr_save_bg(int x, int y, int w, int h);
void scr_restore_bg(void *ps);

int DlgAlert_Notice(const char *text, const char *button);
int DlgAlert_Query(const char *text, const char *button1, const char *button2);
