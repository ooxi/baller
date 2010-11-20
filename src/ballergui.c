
#include <stdio.h>
#include <SDL.h>
#include <math.h>

#include "baller1.h"
#include "baller.h"
#include "ballergui.h"
#include "sdlgui.h"
#include "screen.h"

#define hide()    /* graf_mouse(256,0) */
#define show()    /* graf_mouse(257,0) */
#define sav_scr() /* hide();movmem(scr,buf,32000);show() */
#define lod_scr() /* hide();movmem(buf,scr,32000);show() */

#if GEMSTUFF
/* Verwaltung von Ereignissen: Messages, Maus oder Timer */
event()
{
	short wh;

	wh=evnt_multi( 50, 1,1,1, 0,0,0,0,0, 0,0,0,0,0, m_buf, 300,0,
	               &mx,&my,&bt,&dum,&dum,&dum );
	if ( wh&16 && m_buf[0]==10 )
	{
		menu(0);
		if ( m_buf[4]==INFO ) obj_do( a_inf );
		else if ( m_buf[4]==SPI1 )    /* Neues Spiel */
		{
			if ( bur_obj() )
			{
				neues();
				werdran(1);
			}
		}
		else if ( m_buf[4]==SPI2 )    /* Namen eingeben */
		{
			movmem( nsp1, **(int **)(a_nam+NSP1*24+12), 20 );
			movmem( nsp2, **(int **)(a_nam+NSP2*24+12), 20 );
			if ( obj_do( a_nam )==NOK )
			{
				movmem( **(int **)(a_nam+NSP1*24+12), nsp1, 20 );
				movmem( **(int **)(a_nam+NSP2*24+12), nsp2, 20 );
				if ( !end ) werdran(1);
			}
		}
		else if ( m_buf[4]==SPI4 ) /* Optionen */
		{
			*(short*)(a_opt+A_ERL*24+10)=an_erl;
			*(short*)(a_opt+A_VER*24+10)=!an_erl;
			*(short*)(a_opt+MAX_1*24+10)=!mxin;
			*(short*)(a_opt+MAX_2*24+10)=(mxin==1);
			*(short*)(a_opt+MAX_XX*24+10)=(mxin==2);
			*(short*)(a_opt+MAX_NE*24+10)=(mxin==3);
			*(short*)(a_opt+A_KAP*24+10)=au_kap;
			if ( obj_do(a_opt)==OP_OK )
			{
				char *h;
				au_kap=*(short*)(a_opt+A_KAP*24+10);
				an_erl=*(short*)(a_opt+A_ERL*24+10);
				mxin=*(short*)(a_opt+MAX_2*24+10)+
				     2**(short*)(a_opt+MAX_XX*24+10)+
				     3**(short*)(a_opt+MAX_NE*24+10);
				h=(char *)(**(int **)(a_opt+MAX_XX*24+12));
				max_rund=h[0]-48;
				if (h[1])
				{
					max_rund=max_rund*10+h[1]-48;
					if (h[2])
						max_rund=max_rund*10+h[2]-48;
				}
				if (!mxin) max_rund=20;
				if (mxin==1) max_rund=50;
				if (mxin==3) max_rund=32767;
			}
		}
		else if ( m_buf[4]==SPI3 ) return( 1 );
		else if ( m_buf[4]>=MOD1 && m_buf[4]<=MOD4 )
		{                          /* Spielmodus ( wer gegen wen ) */
			menu_icheck( a_men,MOD1+mod,0 );
			mod=m_buf[4]-MOD1;
			menu_icheck( a_men,m_buf[4],1 );
		}
		else if ( m_buf[4]==MOD5 )
		{                       /* Computer auswählen */
			obj_do( a_com );
			for ( cw[0]=0; cw[0]<7; cw[0]++ )
				if ( *(short *)(a_com+24*(CN1+cw[0])+10) ) break;
			for ( cw[1]=0; cw[1]<7; cw[1]++ )
				if ( *(short *)(a_com+24*(CN2+cw[1])+10) ) break;
			for ( cx[0]=0; cx[0]<4; cx[0]++ )
				if ( *(short *)(a_com+24*(CTS1+cx[0])+10) ) break;
			for ( cx[1]=0; cx[1]<4; cx[1]++ )
				if ( *(short *)(a_com+24*(CTS2+cx[1])+10) ) break;
		}
		else if ( m_buf[4]==REG )    /* Spielregeln */
		{
			short a=0,b;
			int c;

			form_center( a_re1,&fx,&fy,&fw,&fh );
			form_center( a_re2,&fx,&fy,&fw,&fh );
			form_center( a_re3,&fx,&fy,&fw,&fh );
			sav_scr();
			form_dial( 1,190,20,30,20,fx,fy,fw,fh );
			do
			{
				objc_draw( c=a? (a<2? a_re2:a_re3):a_re1, 0,5,0,0,640,400 );
				b=form_do( c,0 );
				*(short *)(c+24*b+10)=0;
				if ( a==1 && b==R21 || a==2 && b==R31 )
				{
					b=-1;
					a=0;
				}
				if ( a==0 && b==R12 || a==2 && b==R32 )
				{
					b=-1;
					a=1;
				}
				if ( a==0 && b==R13 || a==1 && b==R23 )
				{
					b=-1;
					a=2;
				}
			}
			while ( b<0 );
			lod_scr();
			form_dial( 2,190,20,30,20,fx,fy,fw,fh );
		}
		else if ( m_buf[4]==ERG1 )
		{
			movmem(t_na[0],**(int **)(a_ein+EI_N1*24+12),8);
			movmem(t_na[1],**(int **)(a_ein+EI_N2*24+12),8);
			movmem(t_na[2],**(int **)(a_ein+EI_N3*24+12),8);
			movmem(t_na[3],**(int **)(a_ein+EI_N4*24+12),8);
			movmem(t_na[4],**(int **)(a_ein+EI_N5*24+12),8);
			movmem(t_na[5],**(int **)(a_ein+EI_N6*24+12),8);
			*(short*)(a_ein+24*ER_LOE+10)=0;
			*(short*)(a_ein+24*ER_BLE+10)=1;
			if ( obj_do( a_ein )==ER_OK )
			{
				short i,j;
				movmem(**(int **)(a_ein+EI_N1*24+12),t_na[0],8);
				movmem(**(int **)(a_ein+EI_N2*24+12),t_na[1],8);
				movmem(**(int **)(a_ein+EI_N3*24+12),t_na[2],8);
				movmem(**(int **)(a_ein+EI_N4*24+12),t_na[3],8);
				movmem(**(int **)(a_ein+EI_N5*24+12),t_na[4],8);
				movmem(**(int **)(a_ein+EI_N6*24+12),t_na[5],8);
				if (*(short*)(a_ein+24*ER_LOE+10))
					for (i=0;i<6;i++) for (j=0;j<10;j++) t_gew[i][j]=0;
			}
		}
		else if ( m_buf[4]==ERG2 )
		{
			if ( t_load() )
				form_alert(1,"[1][Kann BALLER.TAB nicht laden!][Abbruch]" );
		}
		else if ( m_buf[4]==ERG3 ) t_save();
		else if ( m_buf[4]==ERG4 )
		{
			sav_scr();
			form_dial( 1,260,20,30,20,48,52,548,308 );
			tabelle();
			bt=0;
			while ( !bt ) maus();
			lod_scr();
			form_dial( 2,260,20,30,20,48,52,548,308 );
		}
		if ( m_buf[4]>=MOD1 && m_buf[4]<=MOD5 )
		{
			if ( mod<2 ) l_nam = nsp1;
			else l_nam = cn[cw[0]];
			if ( mod&1 ) r_nam = cn[cw[1]];
			else r_nam = nsp2;
			if ( !end ) werdran(1);
		}
		menu_tnormal( a_men,m_buf[3],1 );
		menu(1);
	}
	return( 0 );
}

#else

int event(void)
{
	// printf("event...\n");

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		// printf("EVENT = %i\n", event.type);
		switch(event.type)
		{
		 case SDL_QUIT:
			printf("Leaving Ballerburg...\n");
			return 1;
		 case SDL_MOUSEMOTION:               /* Read/Update internal mouse position */
			mx = event.motion.x;
			my = event.motion.y;
			break;
		 case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				bt |= 1;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				bt |= 2;
			}
			break;
		 case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				bt &= ~1;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				bt &= ~2;
			}
			break;
		}
	} 

	return 0;
}
#endif


/* The cannoneer dialog data: */

#define WL2 3		/* Winkel um 10 verkleinern */
#define WL1 4		/* Winkel um 1 verkleinern */
#define WR1 6		
#define WR2 7		
#define PL2 9		
#define PL1 10		
#define PR1 12		
#define PR2 13
#define SOK 15
#define SAB 16

// #define WINK TBD
// #define PULV TBD

char dlg_winkel[4];
char dlg_pulver[2];

static SGOBJ cannoneerdlg[] =
{
	{ SGBOX, 0, 0, 0,0, 36,18, NULL },
	{ SGTEXT, 0, 0, 17,1, 6,1, "Kanone" },

	{ SGTEXT, 0, 0, 3,3, 7,1, "Winkel:" },
	{ SGBUTTON, SG_TOUCHEXIT, 0, 12,3, 4,1, "\x04\04" },   // 2 arrows left
	{ SGBUTTON, SG_TOUCHEXIT, 0, 17,3, 3,1, "\x04" },      // Arrow left
	{ SGTEXT, 0, 0, 22,3, 4,1, dlg_winkel },
	{ SGBUTTON, SG_TOUCHEXIT, 0, 26,3, 3,1, "\x03" },      // Arrow right
	{ SGBUTTON, SG_TOUCHEXIT, 0, 30,3, 4,1, "\x03\x03" },  // 2 arrows right

	{ SGTEXT, 0, 0, 3,5, 7,1, "Pulver:" },
	{ SGBUTTON, SG_TOUCHEXIT, 0, 12,5, 4,1, "\x04\04" },   // 2 arrows left
	{ SGBUTTON, SG_TOUCHEXIT, 0, 17,5, 3,1, "\x04" },      // Arrow left
	{ SGTEXT, 0, 0, 22,5, 4,1, dlg_pulver },
	{ SGBUTTON, SG_TOUCHEXIT, 0, 26,5, 3,1, "\x03" },      // Arrow right
	{ SGBUTTON, SG_TOUCHEXIT, 0, 30,5, 4,1, "\x03\x03" },  // 2 arrows right

	{ SGBOX, 0, 0, 2,7, 24,10, NULL },

	{ SGBUTTON, SG_DEFAULT, 0, 27,14, 8,1, "OK" },
	{ SGBUTTON, SG_CANCEL, 0, 27,16, 8,1, "Cancel" },
	{ -1, 0, 0, 0,0, 0,0, NULL }
};


/**
 * Kanonenobjektbaum, Wahl von Winkel und Pulver
 */
int sch_obj(short k)
{
	static short fig[]={ 0,0,15,20,30,20,20,15,10,0,10,-30,18,-18,20,-5,24,-6,
	                     20,-25,10,-40,0,-45, -10,-40,-20,-25,-24,-6,-20,-5,-18,-18,-10,-30,-10,0,
	                     -20,15,-30,20,-15,20, -1,-1
	                   }; /* Daten für das Männchen */
	short i = 0, wi,pv;
	// short xw,yw,xp,yp;
	short xk,yk;
	double s,c;
	char fl=1;
	char *aw,*ap;

#if GEMSTUFF
	aw=*(char **)(a_sch+24*WINK+12);
	ap=*(char **)(a_sch+24*PULV+12);
#else
	puts("sch_obj");
	dlg_winkel[0] = dlg_pulver[0] = 0;
	aw = dlg_winkel;
	ap = dlg_pulver;
#endif
	*(ap+2)=0;

	sav_scr();
#if GEMSTUFF
	form_center( a_sch,&fx,&fy,&fw,&fh );
	objc_offset( a_sch,WINK,&xw,&yw );
	objc_offset( a_sch,PULV,&xp,&yp );
	objc_offset( a_sch,KAST,&xk,&yk );
	xk+=105+f*36;
	yk+=102;
	form_dial(1, ka[n][k].x+10*f,ka[n][k].y-10,20,14,fx,fy,fw,fh );
#else
	SDLGui_CenterDlg(cannoneerdlg);
	/* FIXME: */
	xk = 280;
	yk = 360;
#endif

	wi=ka[n][k].w;
	pv=ka[n][k].p;

	vsf_interior( handle,1 );

	do
	{
		if (pv > pu[n])
		{
			pv=pu[n];
		}
		*aw=48+wi/100;
		*(aw+1)=48+wi%100/10;
		*(aw+2)=48+wi%10;
		if (wi < 100)
		{
			*aw=*(aw+1);
			*(aw+1)=*(aw+2);
			*(aw+2)=0;
		}
		*ap=48+pv/10;
		*(ap+1)=48+pv%10;
		if (pv<10)
		{
			*ap=*(ap+1);
			*(ap+1)=0;
		}

		if ( fl )
		{
#if GEMSTUFF
			objc_draw( a_sch,0,4,0,0,640,400 );
#endif
			hide();
			color(1);
			v_circle( handle,xk-88*f,yk-60,15 );
			i=0;
			while ( fig[i]!=-1 )
			{
				xy[i]=xk-88*f+fig[i];
				i++;
				xy[i]=yk-5+fig[i];
				i++;
			}
			xy[i++]=xy[0];
			xy[i++]=xy[1];
			v_fillarea( handle,i/2-1,xy );
			show();
		}
#if GEMSTUFF
		objc_draw( a_sch,0,4,xw,yw,42,18 );
		objc_draw( a_sch,0,4,xp,yp,42,18 );
#endif

		if ((i!=PL2 && i!=PR2 && i!=PL1 && i!=PR1) || fl)
		{
			hide();
			clr( xk-55,yk-76,110,90 );
			color( 1 );
			v_circle( handle,xk,yk,15 );

			s=sin(wi/P57);
			c=cos(wi/P57);
			fl=-f;
			if ( wi>90 )
			{
				fl=-fl;
				c=-c;
			}
			xy[0]=xk+fl*(c*14+s*14);
			xy[1]=yk+s*14-c*14;
			xy[2]=xk+fl*(c*14+s*40);
			xy[3]=yk+s*14-c*40;
			xy[4]=xk-fl*(c*55-s*40);
			xy[5]=yk-s*55-c*40;
			xy[6]=xk-fl*(c*55-s*14);
			xy[7]=yk-s*55-c*14;
			xy[8]=xy[0];
			xy[9]=xy[1];
			v_fillarea( handle,4,xy );
			show();
			fl=0;
		}

#if GEMSTUFF
		i=form_do( a_sch,0 );
		if ( i>=0 ) *(short *)(a_sch+24*i+10)=0;
#else
		i = SDLGui_DoDialog(cannoneerdlg, NULL);
		SDL_Delay(200);
#endif

		wi-=10*(i==WL2)-10*(i==WR2)+(i==WL1)-(i==WR1);
		if ( wi<0 ) wi=0;
		if ( wi>180 ) wi=180;
		pv-= 3*(i==PL2)- 3*(i==PR2)+(i==PL1)-(i==PR1);
		if ( pv<5 ) pv=5;
		if ( pv>20 ) pv=20;
	}
	while (i != SOK && i != SAB);

	lod_scr();

#if GEMSTUFF
	form_dial(2, ka[n][k].x+10*f,ka[n][k].y-10,20,14,fx,fy,fw,fh );
#else
	SDL_UpdateRect(surf, 0,0, 0,0);
#endif

	ka[n][k].w=wi;
	ka[n][k].p=pv;

	return (i == SOK);
}
