
#include <stdio.h>
#include <SDL.h>

#include "baller1.h"
#include "ballergui.h"

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
