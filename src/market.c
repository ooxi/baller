/*
    market.c

    Copyright (C) 1987, 1989  Eckhard Kruse
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


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "baller1.h"
#include "baller2.h"
#include "ballergui.h"
#include "screen.h"
#include "sdlgui.h"
#include "market.h"


static char dlg_geld[6];
static char dlg_geschuetze[6];
static char dlg_pulver[6];
static char dlg_volk[6];
static char dlg_ftuerme[6];
static char dlg_fahne[6];
static char dlg_kugeln[6];
static char dlg_steuern[6];

static char dlg_anbauenpreis[6];
static char dlg_fturmpreis[6];
static char dlg_geschuetzpreis[6];
static char dlg_fahnepreis[6];
static char dlg_pulverpreis[6];
static char dlg_kugelpreis[6];

#define SH1	11	/* Geld */
#define SH2	12	/* F�rdert�rme */
#define SH3	13	/* Gesch�tze */
#define SH4	14	/* Windfahne */
#define SH5	15	/* Pulver */
#define SH6	16	/* Kugeln */
#define SH7	17	/* Volk */
#define SH8	18	/* Steuern */
#define SHK	19	/* Steuern runter */
#define SHG	20	/* Steuern rauf */

#define SM1	29	/* Preis f�r Anbauen */
#define SM2	30	/* Preis f�r F�rderturm */
#define SM3	31	/* Preis f�r Gesch�tz */
#define SM4	32	/* Preis f�r Windfahne */
#define SM5	33	/* Preis f�r Pulver */
#define SM6	34	/* Preis f�r Kugeln */

#define FERTIG	35

static SGOBJ marktdlg[] =
{
	{ SGBOX, 0, 0, 0,0, 44,20, NULL },

	{ SGBOX, 0, 0, 1,1, 42,7, NULL },
	{ SGTEXT, 0, 0, 18,1, 8,1, "Du hast:" },

	{ SGTEXT, 0, 0, 2,3, 10,1, "Geld:" },
	{ SGTEXT, 0, 0, 23,3, 12,1, "F�rdert�rme:" },
	{ SGTEXT, 0, 0, 2,4, 10,1, "Gesch�tze:" },
	{ SGTEXT, 0, 0, 23,4, 12,1, "Windfahne:" },
	{ SGTEXT, 0, 0, 2,5, 10,1, "Pulver:" },
	{ SGTEXT, 0, 0, 23,5, 12,1, "Kugeln:" },
	{ SGTEXT, 0, 0, 2,6, 10,1, "Volk:" },
	{ SGTEXT, 0, 0, 23,6, 12,1, "Steuern in %" },
	{ SGTEXT, 0, 0, 14,3, 5,1, dlg_geld },
	{ SGTEXT, 0, 0, 37,3, 5,1, dlg_ftuerme },
	{ SGTEXT, 0, 0, 14,4, 5,1, dlg_geschuetze },
	{ SGTEXT, 0, 0, 37,4, 5,1, dlg_fahne },
	{ SGTEXT, 0, 0, 14,5, 5,1, dlg_pulver },
	{ SGTEXT, 0, 0, 37,5, 5,1, dlg_kugeln },
	{ SGTEXT, 0, 0, 14,6, 5,1, dlg_volk },
	{ SGTEXT, 0, 0, 35,6, 5,1, dlg_steuern },
	{ SGBUTTON, SG_EXIT, 0, 36,6, 1,1, "\x04" },    // Arrow left
	{ SGBUTTON, SG_EXIT, 0, 41,6, 1,1, "\x03" },    // Arrow right

	{ SGBOX, 0, 0, 1,9, 42,7, NULL },
	{ SGTEXT, 0, 0, 18,9, 8,1, "Markt:" },

	{ SGTEXT, 0, 0, 2,11, 10,1, "Anbauen:" },
	{ SGTEXT, 0, 0, 23,11, 12,1, "F�rderturm:" },
	{ SGTEXT, 0, 0, 2,12, 10,1, "Gesch�tz:" },
	{ SGTEXT, 0, 0, 23,12, 12,1, "Windfahne:" },
	{ SGTEXT, 0, 0, 2,13, 10,1, "30 Pulver:" },
	{ SGTEXT, 0, 0, 23,13, 12,1, "2 Kugeln:" },
	{ SGTEXT, 0, 0, 14,11, 5,1, dlg_anbauenpreis },
	{ SGTEXT, 0, 0, 37,11, 5,1, dlg_fturmpreis },
	{ SGTEXT, 0, 0, 14,12, 5,1, dlg_geschuetzpreis },
	{ SGTEXT, 0, 0, 37,12, 5,1, dlg_fahnepreis },
	{ SGTEXT, 0, 0, 14,13, 5,1, dlg_pulverpreis },
	{ SGTEXT, 0, 0, 37,13, 5,1, dlg_kugelpreis },

	{ SGBUTTON, SG_DEFAULT, 0, 16,18, 12,1, "Fertig" },

	{ -1, 0, 0, 0,0, 0,0, NULL }
};


/**
 * Anbauen
 */
static void anbau(void)
{
	short s;
	char a[3];
	SDL_Surface *pBgSurface;
	SDL_Rect rect, bgrect;

	/* Save background */
	rect.x = 220;
	rect.y = 375-12;
	rect.w = 25*8;
	rect.h = 38;
	bgrect.x = bgrect.y = 0;
	bgrect.w = rect.w;
	bgrect.h = rect.h;
	pBgSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, 25*8, 38, surf->format->BitsPerPixel,
	                                  surf->format->Rmask, surf->format->Gmask,
	                                  surf->format->Bmask, surf->format->Amask);
	if (pBgSurface != NULL)
	{
		SDL_BlitSurface(surf, &rect, pBgSurface, &bgrect);
	}
	else
	{
		fprintf(stderr, "anbau: CreateRGBSurface failed: %s\n", SDL_GetError());
	}

	color(1);
	vsf_interior( handle,2 );
	vsf_style( handle,9 );
	v_gtext( handle,280,375," Anbauen: " );
	v_gtext( handle,220,395," Verbleibende Steine: 20 ");
	s=20;
	// graf_mouse(6,0);
	scr_color(0x909080);

	do
	{
		if (event())
			exit(0);

		if ( bt && (n? mx>624-bg[0] : mx<bg[0]+15 ) && my>155 )
		{
			// hide();
			if ( !( loc(mx,my) || loc(mx+1,my+1) || loc(mx-1,my-1) ) &&
			                ( loc(mx+3,my-1)||loc(mx+3,my+1)||loc(mx-3,my-1)||loc(mx-3,my+1)||
			                  loc(mx+1,my+2)||loc(mx-1,my+2)||loc(mx+1,my-2)||loc(mx-1,my-2) ))
			{
				vswr_mode( handle,4 );
				xy[0]=mx-2;
				xy[1]=my-1;
				xy[2]=mx+2;
				xy[3]=my+1;
				v_bar( handle,xy );
				vswr_mode( handle,1 );
				s--;
				a[0]=48+s/10;
				a[1]=48+s%10;
				a[2]=0;
				v_gtext( handle,396,395,a );
			}
			// show();
		}
	}
	while ( s>0 && bt<2 );

	/* Restore background */
	if (pBgSurface)
	{
		SDL_BlitSurface(pBgSurface, &bgrect, surf,  &rect);
		SDL_FreeSurface(pBgSurface);
	}

	// graf_mouse(0,0);
}


/**
 * 5-stellige Zahl, rechtsb�ndig, ohne f�hrende Nullen
 */
static void zahl(short nr, short wert)
{
	short i,a,b;
	char *adr;

	adr = marktdlg[nr].txt;

	for (b = i = 0, a = 10000; i < 5; i++, a /= 10)
	{
		*adr++ = 48 + wert/a - 16*(wert<a && i<4 && !b);
		b |= wert/a;
		wert %= a;
	}
}


/**
 * Markt dialog
 */
void markt(void)
{
	short a,k,ko,t;
#if GEMSTUFF
	form_center( a_sta,&fx,&fy,&fw,&fh );
	sav_scr();
	form_dial( 1,mx,my,30,20,fx,fy,fw,fh );
#else
	SDLGui_CenterDlg(marktdlg);

#endif

	for (a = 0; a < 6; a++)
	{
		zahl(SM1 + a, p[a]);
	}

	do
	{
		for (t = k = 0; k < 5; )
		{
			/* F�rdert�rme z�hlen */
			t += (ft[n][k++].x > -1);
		}
		for (ko = k = 0; k < 10; )
		{
			/* Kanonen z�hlen */
			ko += (ka[n][k++].x > -1);
		}
		for (k = 0; k < 10; k++)
		{
			if (bg[1+k*2] > -1 && ka[n][k].x == -1)
			{
				/* Platz f�r neue Kanone? */
				for (a = 1; a < 10; a++)
				{
					if (loc(639*n+(bg[1+k*2]+5+a)*f, by[n]-bg[2+k*2]-a))
						break;
				}
				if (a > 9)
					break;
			}
		}

		zahl(SH1, ge[n]);
		zahl(SH2, t);
		zahl(SH3, ko);
		zahl(SH4, wx[n]>-1);
		zahl(SH5, pu[n]);
		zahl(SH6, ku[n]);
		zahl(SH7, vo[n]);
		zahl(SH8, st[n]);

		for (a = 0; a < 6; a++)
		{
			// fprintf(stderr,"a=%i: ge=%i p=%i an_erl=%i bg=%i t=%i k=%i wx=%i\n",
			//	   a, ge[n], p[a], an_erl, bg[0], t, k, wx[n]);
			if (ge[n] < p[a]
			    || (!an_erl && a == 0)
			    || (a == 1 && (bg[0]+t*30 > 265 || t > 4))
			    || (a == 2 && k > 9)
			    || (a == 3 && wx[n] > -1))
			{
				marktdlg[SM1 + a].type = SGTEXT;
			}
			else
			{
				marktdlg[SM1 + a].type = SGBUTTON;
			}
		}
#if GEMSTUFF
		objc_draw( a_sta,0,5,0,0,640,400 );

		a=form_do( a_sta,0 );
		if ( a>=0 ) *(short *)(a_sta+24*a+10)=0;
#else
fprintf(stderr,"pulver = %i kugeln = %i\n", pu[n], ku[n]);
		a = SDLGui_DoDialog(marktdlg, NULL);
fprintf(stderr,"... a = %i\n", a);
#endif

		if (a == SHK)
		{
			st[n]-=2*(st[n]>0);
		}
		else if (a == SHG)
		{
			 st[n]+=2*(st[n]<100);
		}
		else if (a != FERTIG && a >= SM1 && a <= SM6)
		{
			ge[n] -= p[a-SM1];
			if (a < SM5)
			{
				drw_all();
				if (a == SM1) anbau();
				else if (a == SM2) fturm();
				else if (a == SM3) init_ka( k,639*n );
				else if (a == SM4)
				{
					wx[n]=639*n+f*bg[23];
					wy[n]=by[n]-bg[24];
					werdran(1);
				}
			}
			else
			{
				pu[n]+=30*(a==SM5);
				ku[n]+=2*(a==SM6);
				drw_gpk(a-SM4);
				drw_gpk(0);
			}
		}
	}
	while (a != FERTIG);

#if GEMSTUFF
	lod_scr();
	drw_all();
	form_dial( 2,mx,my,30,20,fx,fy,fw,fh );
#else
	SDL_UpdateRect(surf, 0,0, 0,0);
#endif
}