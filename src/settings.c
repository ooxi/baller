/*
    settings.c

    Copyright (C) 2011  Thomas Huth

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

#include <math.h>
#include <SDL.h>
#include <SDL_gfxPrimitives.h>

#include "i18n.h"
#include "ballergui.h"
#include "baller1.h"
#include "baller2.h"
#include "sdlgui.h"
#include "screen.h"
#include "settings.h"


#define P1_HUMAN	6
#define P1_COMPUTER	7
#define P1_COMPLEV1	9
#define P1_COMPLEV2	10
#define P1_COMPLEV3	11
#define P1_COMPLEV4	12
#define P2_HUMAN	17
#define P2_COMPUTER	18
#define P2_COMPLEV1	20
#define P2_COMPLEV2	21
#define P2_COMPLEV3	22
#define P2_COMPLEV4	23
#define ROUNDS_20	25
#define ROUNDS_50	26
#define ROUNDS_100	27
#define ROUNDS_NOLIMIT	28
#define GIVEUPALLOWED	29
#define REPAIRALLOWED	30
#define NEWGAME		31
#define CONTINUE	32
#define QUITGAME	33


static SGOBJ settingsdlg[] =
{
	{ SGBOX, 0, 0, 0,0, 67,21, NULL },
	{ SGTEXT, 0, 0, 27,1, 13,1, N_("Settings") },

	{ SGBOX, 0, 0, 1,3, 32,10, NULL },
	{ SGTEXT, 0, 0, 12,4, 9,1, N_("Player 1") },
	{ SGTEXT, 0, 0, 3,6, 5,1, N_("Name:") },
	{ SGEDITFIELD, 0, 0, 9,6, 22,1, nsp1 },
	{ SGRADIOBUT, 0, SG_SELECTED, 3,8, 8,1, N_("Human") },
	{ SGRADIOBUT, 0, 0, 13,8, 10,1, N_("Computer") },
	{ SGTEXT, 0, 0, 3,10, 15,1, N_("AI strength:") },
	{ SGRADIOBUT, 0, 0, 22,10, 3,1, "1" },
	{ SGRADIOBUT, 0, SG_SELECTED, 22,11, 3,1, "2" },
	{ SGRADIOBUT, 0, 0, 28,10, 3,1, "3" },
	{ SGRADIOBUT, 0, 0, 28,11, 3,1, "4" },

	{ SGBOX, 0, 0, 34,3, 32,10, NULL },
	{ SGTEXT, 0, 0, 45,4, 9,1, N_("Player 2") },
	{ SGTEXT, 0, 0, 36,6, 5,1, N_("Name:") },
	{ SGEDITFIELD, 0, 0, 42,6, 22,1, nsp2 },
	{ SGRADIOBUT, 0, SG_SELECTED, 36,8, 8,1, N_("Human") },
	{ SGRADIOBUT, 0, 0, 46,8, 10,1, N_("Computer") },
	{ SGTEXT, 0, 0, 36,10, 15,1, N_("AI strength:") },
	{ SGRADIOBUT, 0, 0, 55,10, 3,1, "1" },
	{ SGRADIOBUT, 0, SG_SELECTED, 55,11, 3,1, "2" },
	{ SGRADIOBUT, 0, 0, 60,10, 3,1, "3" },
	{ SGRADIOBUT, 0, 0, 60,11, 3,1, "4" },

	{ SGTEXT, 0, 0, 3,14, 20,1, N_("Maximum rounds:") },
	{ SGRADIOBUT, 0, 0, 25,14, 4,1, "20" },
	{ SGRADIOBUT, 0, 0, 31,14, 4,1, "50" },
	{ SGRADIOBUT, 0, 0, 37,14, 5,1, "100" },
	{ SGRADIOBUT, 0, SG_SELECTED, 44,14, 12,1, N_("unlimited") },

	{ SGCHECKBOX, 0, SG_SELECTED, 3,16, 26,1, N_("King may capitulate") },
	{ SGCHECKBOX, 0, SG_SELECTED, 36,16, 22,1, N_("Players may build") },

	{ SGBUTTON, 0, 0, 2,19, 20,1, N_("New game") },
	{ SGBUTTON, SG_DEFAULT, 0, 24,19, 19,1, N_("Continue game") },
	{ SGBUTTON, 0, 0, 45,19, 20,1, N_("Exit program") },

	{ -1, 0, 0, 0,0, 0,0, NULL }
};


/**
 * Settings dialog
 */
int settings(void)
{
	int i;
	int retbut;

	SDLGui_CenterDlg(settingsdlg);

	retbut = SDLGui_DoDialog(settingsdlg, NULL);

	SDL_UpdateRect(surf, 0,0, 0,0);

	if (settingsdlg[P1_HUMAN].state & SG_SELECTED)
	{
		if (settingsdlg[P2_HUMAN].state & SG_SELECTED)
			mod = 0;	/* Human vs. human */
		else
			mod = 1;	/* Human vs. computer */
	}
	else
	{
		if (settingsdlg[P2_HUMAN].state & SG_SELECTED)
			mod = 2;	/* Computer vs. human */
		else
			mod = 3;	/* Computer vs. computer */
	}

	/* Computer 1 strategy */
	for (i = P1_COMPLEV1; i <= P1_COMPLEV4; i++) {
		if (settingsdlg[i].state & SG_SELECTED) {
			// FIXME: What's the difference between cx and cw?
			cx[0] = cw[0] = i - P1_COMPLEV1 + 1;
			break;
		}
	}

	/* Computer 2 strategy */
	for (i = P2_COMPLEV1; i <= P2_COMPLEV4; i++) {
		if (settingsdlg[i].state & SG_SELECTED) {
			cx[1] = cw[1] = i - P2_COMPLEV1 + 1;
			break;
		}
	}

	/* Max. amount of rounds */
	if (settingsdlg[ROUNDS_20].state & SG_SELECTED)
		max_rund = 20;
	else if (settingsdlg[ROUNDS_50].state & SG_SELECTED)
		max_rund = 50;
	else if (settingsdlg[ROUNDS_100].state & SG_SELECTED)
		max_rund = 100;
	else if (settingsdlg[ROUNDS_NOLIMIT].state & SG_SELECTED)
		max_rund = 32767;

	/* Is the king allowed to give up? */
	au_kap = settingsdlg[GIVEUPALLOWED].state & SG_SELECTED;

	/* Is repairing allowed? */
	an_erl = settingsdlg[REPAIRALLOWED].state & SG_SELECTED;

	/* New game? */
	if (retbut == NEWGAME)
	{
		dlg_new_game();
	}
	/* Quit game? */
	else if (retbut == QUITGAME || retbut == SDLGUI_QUIT)
	{
		return DlgAlert_Query(_("Quit Ballerburg?"), _("Yes"), _("No"));
	}

	return 0;
}


/* ---------------------------------------------------------------- */

static void draw_castle1(int x, int y, int w, int h);
static void draw_castle2(int x, int y, int w, int h);

#define NEW_P1_PREV	3
#define NEW_P1_NEXT	4
#define NEW_P2_PREV	6
#define NEW_P2_NEXT	7
#define NEW_OK		8
#define NEW_ABORT	9

static SGOBJ newgamedlg[] =
{
	{ SGBOX, 0, 0, 0,0, 58,20, NULL },
	{ SGTEXT, 0, 0, 24,1, 12,1, N_("New game") },

	{ SGTEXT, 0, 0, 9,3, 23,1, N_("Player 1") },
	{ SGBUTTON, SG_EXIT, 0, 2,16, 12,1, "\x04" },     // Arrow left
	{ SGBUTTON, SG_EXIT, 0, 16,16, 12,1, "\x03" },    // Arrow right

	{ SGTEXT, 0, 0, 37,3, 23,1, N_("Player 2") },
	{ SGBUTTON, SG_EXIT, 0, 30,16, 12,1, "\x04" },    // Arrow left
	{ SGBUTTON, SG_EXIT, 0, 44,16, 12,1, "\x03" },    // Arrow right

	{ SGBUTTON, SG_DEFAULT, 0, 19,18, 8,1, "OK" },
	{ SGBUTTON, SG_CANCEL, 0, 31,18, 8,1, "Cancel" },

	{ SGBOX, 0, 0, 2,5, 26,10, NULL },
	{ SGUSER, 0, 0, 2,5, 26,10, (void*)draw_castle1 },

	{ SGBOX, 0, 0, 30,5, 26,10, NULL },
	{ SGUSER, 0, 0, 30,5, 26,10, (void*)draw_castle2 },

	{ -1, 0, 0, 0,0, 0,0, NULL }
};


/**
 * Draw castle
 */
static void draw_castle(int n, int x, int y, int w, int h)
{
	SDL_Rect rect;
	short oy0,oy1;

	rect.x = x + 1;
	rect.y = y + 1;
	rect.w = w - 2;
	rect.h = h - 2;
	SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format,0xff,0xff,0xff));


	oy0 = by[0];
	oy1 = by[1];

	bx[0]=x;
	by[0]=y+h;
	bx[1]=x+w;
	by[1]=y+h;

	burg(n+1);

	by[0]=oy0;
	by[1]=oy1;
}


static void draw_castle1(int x, int y, int w, int h)
{
	draw_castle(1, x, y, w, h);
}

static void draw_castle2(int x, int y, int w, int h)
{
	draw_castle(2, x, y, w, h);
}


/**
 * Dialog for "new game"
 */
void dlg_new_game(void)
{

	int retbut;
	short ob0, ob1, ol[8];

	/* Sichern der alten Werte */
	ob0 = bur[0];
	ob1 = bur[1];
	ol[0]=ge[0];
	ol[1]=ge[1];
	ol[2]=pu[0];
	ol[3]=pu[1];
	ol[4]=ku[0];
	ol[5]=ku[1];
	ol[6]=vo[0];
	ol[7]=vo[1];

	SDLGui_CenterDlg(newgamedlg);

	do
	{
		retbut = SDLGui_DoDialog(newgamedlg, NULL);
		switch (retbut)
		{
		case NEW_P1_PREV:
			bur[0] = (bur[0] - 1 + b_anz) % b_anz;
			break;
		case NEW_P1_NEXT:
			bur[0] = (bur[0] + 1 + b_anz) % b_anz;
			break;
		case NEW_P2_PREV:
			bur[1] = (bur[1] - 1 + b_anz) % b_anz;
			break;
		case NEW_P2_NEXT:
			bur[1] = (bur[1] + 1 + b_anz) % b_anz;
			break;
		}
	}
	while (retbut != NEW_OK && retbut != NEW_ABORT);

	ge[0]=ol[0];
	ge[1]=ol[1];
	pu[0]=ol[2];
	pu[1]=ol[3];
	ku[0]=ol[4];
	ku[1]=ol[5];
	vo[0]=ol[6];
	vo[1]=ol[7];
	fn();

	if (retbut == NEW_OK)
	{
		neues();
		werdran(1);
	}
	else /* if (retbut == NEW_ABORT) */
	{
		bur[0]=ob0;
		bur[1]=ob1;
	}

	SDL_UpdateRect(surf, 0,0, 0,0);
}
