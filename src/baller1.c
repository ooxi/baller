/*
    baller1.c

    Copyright (C) 1987, 1989  Eckhard Kruse
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

/*****************************************************************************
 *                           B a l l e r b u r g          Modul 1            *
 * Dies ist der Hauptteil von Ballerburg. Die Routinen dieses Teils dienen   *
 * im großen und ganzen der Steuerung des Programmes, dem Aufruf der Objekt- *
 * bäume, sowie der Ausführung elementarer Grafikoperationen.                *
 *****************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "baller.h"
#include "baller1.h"
#include "baller2.h"
#include "ballergui.h"
#include "screen.h"
#include "psg.h"
#include "market.h"

#define Min(a,b)  ((a)<(b)?(a):(b))
#define Max(a,b)  ((a)>(b)?(a):(b))
#define maus()    /* graf_mkstate(&mx,&my,&bt,&dum) */
#define menu(a)   /* wind_update(3-a) */
#define hide()    /* graf_mouse(256,0) */
#define show()    /* graf_mouse(257,0) */
#define sav_scr() /* hide();movmem(scr,buf,32000);show() */
#define lod_scr() /* hide();movmem(buf,scr,32000);show() */

#define bing()    printf("\007");


double vvx,vvy;
short handle, mx,my,bt,dum,m_buf[8], xy[100],
	bur[2],bx[2],by[2], ge[2],pu[2],ku[2],vo[2],st[2],kn[2],
	wx[2],wy[2],ws,wc,
	*bg, zug,n,oldn, p[6],  t_gew[6][10], max_rund,
	*burgen[20],b_anz;
int ftx, fty, ftw, fth;     /* Koordinaten von "Fertig" */
void *bur_ad;
int   scr, a_opt, a_ein;
#if GEMSTUFF
int a_men,a_inf,a_sch,a_brg,a_nam,a_dra;
int a_sta,a_sie,a_com,a_re1,a_re2,a_re3;
#endif
char *l_nam, *r_nam;
int f;
char  mod, wnd, end, txt[4], an_erl, au_kap,
	cw[2]={2,2}, cx[2]={1,1};

//char cn[7][8]={ "Tölpel","Dummel","Brubbel","Wusel","Brösel","Toffel","Rüpel" };
char t_na[6][8]={ "Tölpel","Dummel","Brubbel","Wusel","Brösel","Toffel" };

char nsp1[22]="Hugo",nsp2[22]="Emil";

ka_t ka[2][10];
ft_t ft[2][5];


/*****************************************************************************/
int main(int argc, char **argv)
{
	// short i;

#if GEMSTUFF
	gem_init();
	scr=Logbase();
#else
	scr_init();
	scr_init_done_button(&ftx, &fty, &ftw, &fth);
#endif

//	m_laden("BALLER.MUS"); /* Laden der Musikdatei mit Funktion aus MUSIK.C */

#if GEMSTUFF
	if ( !rsrc_load( "BALLER.RSC" ) ) exit( form_alert(1,"[1][BALLER.RSC läßt sich nicht|laden.][Abbruch]") );
	rsrc_gaddr( 0,MENUE,&a_men  );
	rsrc_gaddr( 0,INFOTREE,&a_inf );
	rsrc_gaddr( 0,SCHUSS,&a_sch );
	rsrc_gaddr( 0,BURG,&a_brg );
	rsrc_gaddr( 0,NAMEN,&a_nam  );
	rsrc_gaddr( 0,DRAN,&a_dra );
	rsrc_gaddr( 0,STATUS,&a_sta );
	rsrc_gaddr( 0,SIEGER,&a_sie );
	rsrc_gaddr( 0,COMPUTER,&a_com);
	rsrc_gaddr( 0,REGEL1,&a_re1 );
	rsrc_gaddr( 0,REGEL2,&a_re2 );
	rsrc_gaddr( 0,REGEL3,&a_re3 );
	rsrc_gaddr( 0,OPTION,&a_opt );
	rsrc_gaddr( 0,EINTRAG,&a_ein );

	for ( i=0;i<6;i++ ) *(short *)(a_men+(ACC1+i)*24+10)=8;
	*(short *)(a_com+CN1*24+58)=1;
	*(short *)(a_com+CN2*24+58)=1;
	*(short *)(a_com+CTS1*24+34)=1;
	*(short *)(a_com+CTS2*24+34)=1;
#endif

	bur_ad = malloc(16000); /* Speicher für Burgdaten */
	if (!bur_ad) {
		printf("Zu wenig Speicher!\n");
		exit(-1);
	}
	an_erl=1;
	// mxin=3;
	max_rund=32767;
	au_kap=1;
	t_load();
	burgen_laden();

//	menu_bar(a_men,1);
	menu(0);

//	v_show_c( handle,0 );
//	graf_mouse( 0,0 );

	psg_audio_init();

	l_nam = nsp1;
	r_nam = nsp2;

	mod=0;                        /* Spielmodus ( wer gegen wen ) */

	neues();
	while ( ein_zug() );

	t_save();
//	menu_bar(a_men,0);
	menu(1);
//	v_clsvwk( handle );
//	appl_exit();

	return 0;
}


/**************************** Tabelle ****************************************/
void tabelle(void)
{
	short i,j;
	hide();
	vsf_interior(handle,0);
	box(53,56,587,343,1);
	box(55,58,585,341,1);
	box(56,59,584,340,1);
	line(144,59,144,340);
	for (i=152;i<584;i+=72) line(i,59,i,340);
	line(56,84,584,84);
	for (i=92;i<240;i+=24) line(56,i,584,i);
	for (i=244;i<340;i+=24) line(56,i,584,i);
	for (i=0;i<6;i++) v_gtext(handle,160+i*72,78,t_na[i]);
	for (i=0;i<6;i++) v_gtext(handle,80,110+i*24,t_na[i]);
	vsf_interior(handle,2);
	vsf_style(handle,2);
	for (i=0;i<6;i++)
	{
		for (j=0;j<10;j++)
		{
			z_txt(t_gew[i][j]);
			if (j==9 && !t_gew[i][6] )
			{
				txt[0]=32;
				txt[1]='-';
				txt[2]=0;
			}
			v_gtext(handle,176+i*72,110+j*24+8*(j>5),txt);
			if (i==j) box(152+i*72,92+j*24,224+i*72,116+j*24, 1 /*FIXME*/);
		}
	}
	v_gtext(handle,64,262,"\344  Spiele");
	v_gtext(handle,64,286,"\344gewonnen");
	v_gtext(handle,64,310,"\344verloren");
	v_gtext(handle,64,334,"Siege in%");
	line(56,59,144,84);
	vst_height(handle,4,&i,&i,&i,&i);
	v_gtext(handle,60,81,"VERLOREN");
	v_gtext(handle,92,66,"GEWONNEN");
	vst_height(handle,13,&i,&i,&i,&i);
	show();
}


void z_txt(short a)
{
	txt[0]=a/100+48;
	txt[1]=a%100/10+48;
	txt[2]=a%10+48;
	if (a<100)
	{
		txt[0]=32;
		if (a<10) txt[1]=32;
	}
	//for (a=0;a<3;a++) txt[a]=txt[a]==48? 79: txt[a]==49? 108: txt[a];
}


/******************** Initialisierung vor neuem Spiel ************************/
void neues(void)
{
	static short pr[6]={ 200,500,400,150,50,50 };   /* Preise zu Beginn */
	short j;

	wnd=rand()%60-30;
	st[0]=st[1]=20;
	kn[0]=kn[1]=0;
	for ( j=0;j<6;j++ )
		p[j]=pr[j]*(95+rand()%11)/100;
	bild();
	for ( n=0;n<2;n++ )
	{
		bg=burgen[bur[n]];
		wx[n]=n? 639-bg[23]:bg[23];
		wy[n]=by[n]-bg[24];
		for ( f=0;f<5; ) ft[n][f++].x=-1;
	}
	zug=n=end=0;
	f=1;
}


/************************* Durchführen eines Zuges ***************************/
int ein_zug(void)
{
	short i = 0, fl, a;

	// puts("ein zug ...");

	n=zug&1;
	fn();
	kn[n]&=~16;
	wnd=wnd*9/10+rand()%12-6;

	werdran(1);

	do
	{
		fl=0;
		menu(1);
		do
		{
			if (event(!(mod&(2-n))) != 0)
				return(0);
		}
		while (!bt && !(mod&(2-n)));
		//printf("ein zug %i %i %i\n", bt, mod, a);
		menu(0);
		bg=burgen[bur[n]];

		if ( mod&(2-n) )
		{
			hide();
			i=comp();
			show();
			fl=1+(i<0);
		}
		else if (mx > ftx && mx < ftx+ftw && my > fty && my < fty+fth)
		{
			/* "Fertig"-Knopf wurde gedrueckt */
			scr_draw_done_button(1);
			do
			{
				event(1);
			} while (bt);
			scr_draw_done_button(0);
			fl=2;
		}
		else
		{
			for ( i=0;i<10;i++ )
				if ( ka[n][i].x<=mx && ka[n][i].x+20>=mx && ka[n][i].x!=-1 &&
				                ka[n][i].y>=my && ka[n][i].y-14<=my ) break;
			if ( i>9 )
			{
				if ( drin( bg[25],bg[26],bg[31],bg[32],0,mx,my ) ||
				                drin( bg[27],bg[28],bg[33],bg[34],0,mx,my ) ||
				                drin( bg[29],bg[30],bg[35],bg[36],0,mx,my ) ) markt();
				else if ( drin( bg[21],bg[22],30,25,0,mx,my ) ) koenig();
				else bing();
			}
			else if ( pu[n]<5 )
			{
				bing();
				DlgAlert_Notice("Dein Pulver reicht nicht!", "Abbruch");
			}
			else if ( !ku[n] )
			{
				bing();
				DlgAlert_Notice("Du hast keine Kugeln mehr!", "Abbruch");
			}
			else fl=sch_obj(i);
		}
	}
	while ( !fl );

	werdran(0);
	if ( fl<2 ) schuss(i);

	if ( ~kn[n]&16 ) kn[n]&=~15;
	rechnen();
	zug++;

	for ( i=0;i<10;i++ ) if ( ka[n][i].x>-1 ) break;
	n=zug&1;
	bg=burgen[bur[n]];
	for ( a=0;a<10;a++ ) if ( ka[n][a].x>-1 ) break;
	if ( a==10 && i<10 && bg[40]>vo[n] && ge[n]<p[2]/3 && au_kap &&
	                ft[n][0].x+ft[n][1].x+ft[n][2].x+ft[n][3].x+ft[n][4].x==-5 )
		end=n+33;

	if ( !end && zug/2>=max_rund )
	{
		static int h[2];
		for (n=0;n<2;n++)
		{
			h[n]=ge[n]+pu[n]*p[4]/30+ku[n]*p[5]/2+(wx[n]>-1)*p[3]+vo[n]*4;
			for (i=0;i<5;i++) if ( ft[n][i].x>-1 ) h[n]+=p[1];
			for (i=0;i<10;i++) if ( ka[n][i].x>-1 ) h[n]+=p[2];
		}
		end=65+(h[1]<h[0]);
	}
	if ( end )
	{
		ende();
		menu(1);
		do
			if (event(1) != 0)
				return 0;
		while ( !bt /*|| end*/ );
		neues();
		menu(0);
	}
	return(1);
}


/********** Berechnen von Bevölkerungszuwachs usw. nach jedem Zug ************/
void rechnen(void)
{
	short j;
	static short pmi[6]={ 98,347,302,102,30,29 },   /* Preisgrenzen */
	             pma[6]={ 302,707,498,200,89,91 },
	             psp[6]={ 10,50,50,20,10,10 };     /* max. Preisschwankung */

	j=st[n];
	ge[n]+=vo[n]*(j>65? 130-j:j)/(150-rand()%50);
	vo[n]=vo[n]*(95+rand()%11)/100+(21-j+rand()%9)*(8+rand()%5)/20;
	if ( vo[n]<0 )
	{
		vo[n]=0;
		end=n+49;
	}

	for ( j=0;j<5;j++ ) ge[n]+=(40+rand()%31)*(ft[n][j].x>-1);
	for ( j=0;j<6;j++ )
	{
		p[j]+=psp[j]*(rand()%99)/98-psp[j]/2;
		p[j]=Max(p[j],pmi[j]);
		p[j]=Min(p[j],pma[j]);
	}
	drw_gpk(0);
}


/******************************* Spielende ***********************************/
void ende(void)
{
	char s1[80], s2[80], s3[80];
	int a, b;
	int i;

	strcpy( s1,"!! ");
	strcat( s1, end&2? l_nam:r_nam );
	strcat( s1," hat gewonnen !!" );
	s2[0]=0;
	if (~end&64) strcpy(s2,"( ");
	strcat( s2, end&2? r_nam:l_nam );
	if ( (end&240)<48 )
	{
		a=s2[strlen(s2)-1];
		strcat( s2, a=='s' || a=='S'? "' ":"s " );
	}
	switch ( end&240 )
	{
	case 16:
		strcat( s2,"König wurde getroffen," );
		strcpy( s3,"  daraufhin ergab sich dessen Volk. )" );
		break;
	case 32:
		strcat( s2,"König hat aufgrund der" );
		strcpy( s3,"  aussichtslosen Lage kapituliert. )" );
		break;
	case 48:
		strcat( s2," hat kein Volk mehr. )" );
		s3[0]=0;
		break;
	case 64:
		strcpy( s3,s2 );
		strcpy( s2,"( Die maximale Rundenzahl ist erreicht.");
		strcat( s3," befindet sich in der schlechteren Lage. )" );
	}

	for (a=0;a<6 && strncmp(t_na[a],l_nam,7);a++);
	for (b=0;b<6 && strncmp(t_na[b],r_nam,7);b++);
	if (a<6 && b<6 && a!=b)
	{
		if (~end&2)
		{
			int c;
			c=a;
			a=b;
			b=c;
		}
		t_gew[a][b]++;
		t_gew[b][8]++;
		t_gew[a][9]=100*++t_gew[a][7]/++t_gew[a][6];
		t_gew[b][9]=100*t_gew[b][7]/++t_gew[b][6];
	}

	for (i = 0; i < 8; i++)
	{
		short xy[4];
		if (i!=7)
			scr_color((i*32)<<16);
		else
			scr_color(0xffffff);
		xy[0] = 40 + i*8;
		xy[1] = 80 + i*8;
		xy[2] = 600 - i*8;
		xy[3] = 320 - i*8;
		v_bar(handle, xy);
	}

	printf("Ende : %s\n", s1);
	printf("Ende : %s\n", s2);
	printf("Ende : %s\n", s3);

	v_gtext(handle, 140, 170, s1);
	v_gtext(handle, 140, 210, s2);
	v_gtext(handle, 140, 230, s3);

	//m_musik();

	/*
	Giaccess( 0,138 );
	Giaccess( 0,139 );
	Giaccess( 0,140 );
	*/
}


/* Die Routine m_wait() wird von m_musik() nach jedem 1/96 Takt aufgerufen.  */
/* In diesem Fall macht sie nichts anderes als die eigentliche Warteschleife */
/* aufzurufen. In eigenen Programmen könnten Sie hier während der Musik zu-  */
/* sätzliche Aktionen ablaufen lassen. */
void m_wait(void)
{
	// m_wloop();
	SDL_Delay(10);
}


/** Anzeige des Spielers, der am Zug ist, sowie Darstellung der Windfahnen ***/
void werdran(char c)
{
#if GEMSTUFF
	short *a;
	char *ad;
#else
	char ad[8];
#endif
	int i;
	short x, y, w, h, c1, s1, c2, s2;
	double wk,wl;

	/* Anzahl der Spielrunden ausgeben */
	z_txt(zug/2+1);
	v_gtext(handle, 332, 395+16, txt);

#if GEMSTUFF
	a=(short *)(a_dra+16);
#endif
	if ( c )
	{
#if GEMSTUFF
		a[0]=5+(629-a[2])*n;
		a[1]=25;
		*(int *)(a_dra+DNAM*24+12)=n? r_nam:l_nam;
		ad=*(char **)(a_dra+DWIN*24+12);
		ad[0]=ad[5]=4+28*!wnd-(wnd>0);
		i=wnd<0? -wnd:wnd;
		ad[2]=48+i/10;
		ad[3]=48+i%10;
		if ( wx[n]<0 )
		{
			ad[0]=ad[5]=32;
			ad[2]=ad[3]='?';
		}

		objc_offset( a_dra,DOK,&ftx,&fty );
		ftw=*(short *)(a_dra+DOK*24+20);
		fth=*(short *)(a_dra+DOK*24+22);
		objc_offset( a_dra,DWBX,&x,&y );
		w=*(short *)(a_dra+DWBX*24+20);
		h=*(short *)(a_dra+DWBX*24+22);

		objc_draw( a_dra,0,4,0,0,640,400 );
#else
		x = 5+(629-104)*n; y = 410;
		w = 104; h = 48;

		/* Wind ausgeben: */
		ad[0] = ad[5] = 4+28*!wnd-(wnd>0);
		i = wnd<0? -wnd:wnd;
		ad[1] = ad[4] = ' ';
		ad[2] = 48+i/10;
		ad[3] = 48+i%10;
		ad[6] = 0;
		if ( wx[n]<0 )
		{
			ad[0]=ad[5]=32;
			ad[2]=ad[3]='?';
		}
		v_gtext(handle, x+4, y+h+12, "Wind:");
		v_gtext(handle, x+52, y+h+12, ad);
#endif
		c=wnd>0? 1:-1;
		wk=c*wnd/15.0;
		wl=wk*.82;
		if ( wk>1.57 ) wk=1.57;
		if ( wl>1.57 ) wl=1.57;
		s1=c*20*sin(wk);
		c1=20*cos(wk);
		s2=c*20*sin(wl);
		c2=20*cos(wl);
		ws=s1/2.0;
		ws+=!ws;
		wc=c1/2.0;

		hide();
		if ( wx[n]>-1 )
		{
			color(1);
			x+=w/2;
			line( x,y+h,x,y+5 );
			line( x+1,y+h,x+1,y+5 );
			xy[0]=xy[2]=x+1;
			xy[1]=y+5;
			xy[3]=y+11;
			if ( wk<.2 )
			{
				xy[0]=x-1;
				xy[1]=xy[3]=y+5;
				xy[2]=x+2;
			}
			xy[4]=xy[2]+s1;
			xy[5]=xy[3]+c1;
			xy[8]=xy[0]+s1;
			xy[9]=xy[1]+c1;
			xy[10]=xy[0];
			xy[11]=xy[1];
			xy[6] = ((xy[4] + xy[8]) >> 1) + s2;
			xy[7] = ((xy[5] + xy[9]) >> 1) + c2;
			v_pline( handle,6,xy );
		}
		fahne();
		show();
	}
	else
	{
		hide();
#if GEMSTUFF
		clr( a[0],a[1],a[2],a[3] );
#else
		clr(5+(629-104)*n,410, 104,48+16);
#endif
		show();
	}
}


/******************* Darstellung der beiden Windfahnen ***********************/
void fahne(void)
{
	int m=-1;

	while ( ++m<2 ) if ( wx[m]>-1 )
		{
			clr_bg( wx[m]-10,wy[m]-15,20,15 );
			color(1);
			line( wx[m],wy[m],wx[m],wy[m]-15 );
			if ( m==n )
			{
				line( wx[m],wy[m]-15,wx[m]+ws,wy[m]-13+wc );
				line( wx[m],wy[m]-11,wx[m]+ws,wy[m]-13+wc );
			}
		}
}


/********************** BALLER.TAB laden/speichern ***************************/
int t_load(void)
{
#if 1
	//printf("t_load does not work yet\n");
#else
	FILE *f_h;

	f_h = fopen("baller.tab", "rb");

	if (!f_h) {
		perror("t_load");
		return(1);
	}
	fread(&an_erl, 1, 1, f_h);
	fread(&au_kap, 1, 1, f_h);
	fread(&mxin, 1, 1, f_h);
	fread(**(int **)(a_opt+MAX_XX*24+12), 1, 3, f_h);
	fread(&max_rund, 2, 1, f_h);
	fread(t_na, 1, 48, f_h);
	fread(t_gew, 1, 120, f_h);

	fclose(f_h);
#endif
	return(0);
}


int t_save(void)
{
#if 1
	//printf("t_save not implemented yet\n");
#else
	if ( (f_h=Fcreate( "baller.tab",0 ))<0 ) return(1);
	Fwrite(f_h,1,&an_erl);
	Fwrite(f_h,1,&au_kap);
	Fwrite(f_h,1,&mxin);
	Fwrite(f_h,3,**(int **)(a_opt+MAX_XX*24+12) );
	Fwrite(f_h,2,&max_rund);
	Fwrite(f_h,48,t_na);
	Fwrite(f_h,120,t_gew);
	Fclose(f_h);
#endif
	return(0);
}


/************************* BALLER.DAT laden **********************************/

/* liest ein char von der Datei */
static char zeichen(FILE *f_h)
{
	char a;

	if (fread(&a, 1, 1, f_h) != 1)
	{
		perror("zeichen");
	}

	return a;
}

/* liest eine Dezimalzahl von der Datei, Remarks werden überlesen */
static int rdzahl(FILE *f_h)
{
	char a,sign=1,rem=0;    /* wird durch * getoggled, und zeigt damit an, */
	/* ob man sich in einer Bemerkung befindet */
	int val=0;

	do
		if ( (a=zeichen(f_h))=='*' ) rem=!rem;
	while ((a != '-' && a < '0') || a > '9' || rem);

	if ( a=='-' )
	{
		sign=-1;
		a=zeichen(f_h);
	}
	while ( a>='0' && a<='9' )
	{
		val*=10;
		val+=a-'0';
		a=zeichen(f_h);
	}

	return( sign*val );
}


void burgen_laden(void)
{
	short *a,j;
	FILE *f_h;

	// printf("burgen_laden\n");

	a=(short *)bur_ad;

	f_h = fopen( "baller.dat", "rb");
	if (!f_h) {
		perror("Kann 'baller.dat' nicht finden: ");
		exit(-1);
	}

	b_anz=0;
	while ( (j=rdzahl(f_h))!=-999 )
	{
		burgen[b_anz++]=a;
		*a++=j;
		for ( j=0;j<40;j++ ) *a++=rdzahl(f_h);
		while ( (*a++=rdzahl(f_h))!=-1 );
	}

	fclose(f_h);
}


/************** Darstellung und Verwaltung eines Objektbaumes ****************/
int obj_do(int adr)
{
	int a = 0;
#if GEMSTUFF
	short x=20,fl=0;

	if (adr==a_nam)
	{
		fl=NSP1;
		x=80;
	}
	if (adr==a_opt)
	{
		fl=MAX_XX;
		x=80;
	}
	if (adr==a_com) x=150;
	if (adr==a_ein)
	{
		fl=EI_N1;
		x=260;
	}

	form_center( adr,&fx,&fy,&fw,&fh );
	sav_scr();
	form_dial( 1,x,20,30,20,fx,fy,fw,fh );
	objc_draw( adr,0,5,0,0,640,400 );
	a=form_do( adr,fl );
	*(short *)(adr+24*a+10)=0;
	lod_scr();
	form_dial( 2,x,20,30,20,fx,fy,fw,fh );
#else
	puts("obj_do!");
#endif
	return(a);
}

/*********************** Elementare  Grafikbefehle ***************************/
#if 0
gem_init() /* Öffnen der Workstation... */
{
	short i, work_in[11], work_out[57];

	appl_init();
	handle=graf_handle( &i, &i, &i, &i );
	for ( i=0; i<10; i++ )  work_in[i]=1;
	work_in[10]=2;
	v_opnvwk( work_in, &handle, work_out );
	v_clrwk( handle );
}
#endif


int loc(int x, int y)  /* Ermittelt, ob Punkt gesetzt ist */
{
	int a;
#if GEMSTUFF
	short b;
	v_get_pixel(handle,x,y,&a,&b);
	return((int)a);
#else
	a = scr_getpixel(x,y);
	// printf("loc %i %i = 0x%x\n", x,y, a);
	return  ((a&0xff) != 0xff);
#endif
}

void line(short x1, short y1, short x2, short y2) /* Zeichnet eine Linie */
{
	xy[0]=x1;
	xy[1]=y1;
	xy[2]=x2;
	xy[3]=y2;
	v_pline( handle, 2, xy );
}

void box(short x, short y, short x2, short y2, short c)
{
	color(c);
	xy[0]=x;
	xy[1]=y;
	xy[2]=x2;
	xy[3]=y2;
	v_bar( handle,xy );
}
