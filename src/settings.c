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

static SGOBJ settingsdlg[] =
{
	{ SGBOX, 0, 0, 0,0, 68,25, NULL },
	{ SGTEXT, 0, 0, 26,1, 13,1, "Einstellungen" },

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

	{ SGBUTTON, SG_DEFAULT, 0, 33,22, 8,1, "OK" },

	{ -1, 0, 0, 0,0, 0,0, NULL }
};


/**
 * Kanonenobjektbaum, Wahl von Winkel und Pulver
 */
void settings(void)
{
	int i;

	SDLGui_CenterDlg(settingsdlg);

	SDLGui_DoDialog(settingsdlg, NULL);

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
			cw[0] = i - P1_COMPLEV1 + 1;
			break;
		}
	}

	/* Computer 2 strategy */
	for (i = P2_COMPLEV1; i <= P2_COMPLEV4; i++) {
		if (settingsdlg[i].state & SG_SELECTED) {
			cw[1] = i - P2_COMPLEV1 + 1;
			break;
		}
	}

}
