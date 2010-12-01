/*
    baller1.h - prototypes and definitions for baller1.c

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

#define P57       57.296
#define G         0.02     /* Fallbeschleunigung ( g/500 ) */

extern double vvx,vvy;
extern short handle, mx,my,bt,dum,m_buf[8], xy[100],
	bur[2],bx[2],by[2], ge[2],pu[2],ku[2],vo[2],st[2],kn[2],
	wx[2],wy[2],ws,wc,
	*bg, zug,n,oldn, p[6],
	fx,fy,fw,fh,
	ftx,fty,ftw,fth,
	f_h, *burgen[20],b_anz;
//extern int scr, a_men,a_inf,a_sch,a_brg,a_nam,a_dra,a_sta,a_sie;
extern char f, mod, wnd, end, an_erl,
	cw[2],cx[2],
	nsp1[],nsp2[];

typedef struct
{
	short x,y,w,p;
} ka_t;
extern ka_t ka[2][10];

typedef struct
{
	short x,y;
} ft_t;
extern ft_t ft[2][5];



int main(int argc, char **argv);
void tabelle(void);
void z_txt(short a);
void neues(void);
int ein_zug(void);
void rechnen(void);
void ende(void);
void m_wait(void);
void werdran(char c);
void fahne(void);
int t_load(void);
int t_save(void);
void burgen_laden(void);
int bur_obj(void);
int obj_do(int adr);
void gem_init(void);
void color(int a);
int loc(int x, int y);
void line(short x1, short y1, short x2, short y2);
void box(short x, short y, short x2, short y2, short c);
void clr(short x, short y, short w, short h);
void cls(void);
