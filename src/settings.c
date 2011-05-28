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

#include "ballergui.h"
#include "baller1.h"
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
	{ SGTEXT, 0, 0, 27,1, 13,1, "Einstellungen" },

	{ SGBOX, 0, 0, 1,3, 32,10, NULL },
	{ SGTEXT, 0, 0, 12,4, 9,1, "Spieler 1" },
	{ SGTEXT, 0, 0, 3,6, 5,1, "Name:" },
	{ SGEDITFIELD, 0, 0, 9,6, 22,1, nsp1 },
	{ SGRADIOBUT, 0, SG_SELECTED, 3,8, 8,1, "Mensch" },
	{ SGRADIOBUT, 0, 0, 13,8, 10,1, "Computer" },
	{ SGTEXT, 0, 0, 3,10, 15,1, "Computerstärke:" },
	{ SGRADIOBUT, 0, 0, 22,10, 3,1, "1" },
	{ SGRADIOBUT, 0, SG_SELECTED, 22,11, 3,1, "2" },
	{ SGRADIOBUT, 0, 0, 28,10, 3,1, "3" },
	{ SGRADIOBUT, 0, 0, 28,11, 3,1, "4" },

	{ SGBOX, 0, 0, 34,3, 32,10, NULL },
	{ SGTEXT, 0, 0, 45,4, 9,1, "Spieler 2" },
	{ SGTEXT, 0, 0, 36,6, 5,1, "Name:" },
	{ SGEDITFIELD, 0, 0, 42,6, 22,1, nsp2 },
	{ SGRADIOBUT, 0, SG_SELECTED, 36,8, 8,1, "Mensch" },
	{ SGRADIOBUT, 0, 0, 46,8, 10,1, "Computer" },
	{ SGTEXT, 0, 0, 36,10, 15,1, "Computerstärke:" },
	{ SGRADIOBUT, 0, 0, 55,10, 3,1, "1" },
	{ SGRADIOBUT, 0, SG_SELECTED, 55,11, 3,1, "2" },
	{ SGRADIOBUT, 0, 0, 60,10, 3,1, "3" },
	{ SGRADIOBUT, 0, 0, 60,11, 3,1, "4" },

	{ SGTEXT, 0, 0, 3,14, 20,1, "Maximale Rundenzahl:" },
	{ SGRADIOBUT, 0, 0, 25,14, 4,1, "20" },
	{ SGRADIOBUT, 0, 0, 31,14, 4,1, "50" },
	{ SGRADIOBUT, 0, 0, 37,14, 5,1, "100" },
	{ SGRADIOBUT, 0, SG_SELECTED, 44,14, 12,1, "unbegrenzt" },

	{ SGCHECKBOX, 0, SG_SELECTED, 3,16, 26,1, "König kann kapitulieren" },
	{ SGCHECKBOX, 0, SG_SELECTED, 36,16, 22,1, "Anbauen ist erlaubt" },

	{ SGBUTTON, 0, 0, 2,19, 20,1, "Neues Spiel" },
	{ SGBUTTON, SG_DEFAULT, 0, 24,19, 19,1, "Weiterspielen" },
	{ SGBUTTON, 0, 0, 45,19, 20,1, "Programm beenden" },

	{ -1, 0, 0, 0,0, 0,0, NULL }
};


/**
 * Kanonenobjektbaum, Wahl von Winkel und Pulver
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
		neues();
		werdran(1);
	}
	/* Quit game? */
	else if (retbut == QUITGAME || retbut == SDLGUI_QUIT)
	{
		return DlgAlert_Query("Ballerburg beenden?", "Ja", "Nein");
	}

	return 0;
}
