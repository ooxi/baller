
		============================
		=   Ballerburg SDL v1.2.0  =
		============================

	    Copyright (C) 1987, 1989  Eckhard Kruse
	    Copyright (C) 2010, 2015  Thomas Huth


	   For my sister Martina Huth - I will never
	 forget the countless happy hours that we had
	     with this game during our childhood.


 1) Lizenz
 ---------

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


 2) What is Ballerburg?
 ----------------------

Ballerburg is a classical castle combat game. Two castles, separated by a hill,
shoot each other with their cannons until one of the king is hit or surrenders.

Ballerburg was originally developed in 1987 by Eckhard Kruse for the Atari ST
computers - a cutting-edge 16/32-bit computer at that time. Now, a quarter of
a century later, here's the adaption of Ballerburg for recent operating systems
that are supported by the SDL library (like GNU/Linux for example) - thanks to
the original source code of the old Ballerburg that has kindly been released
by Eckhard Kruse to the public.


 3) Compiling and installing
 ---------------------------

For compiling the source code of Ballerburg SDL, you need the GCC C-compiler,
CMake and the SDL library (preferably version 1.2, see http://www.libsdl.org/).

For configuring the build, you've got to run "cmake" first, using the path of
the sources as parameter. Alternatively, you can also use the supplied
"configure" script or a GUI tool like "cmake-gui".

After the configuration step, you can compile the source code with "make" and
if everything worked fine, install the program with "make install".


 4) Starting the game
 --------------------

To run the game, simply execute the "ballerburg" program from the folder where
you've installed the game into. The game itself should be self-explanatory 
most of the time, for the details please have a look at the manual in the doc
directory. Please press the "ESC" key to enter the main menu (for setting up
various game options or for starting a new game). Press "f" to toggle between
windowed and fullscreen mode. The key "q" is used for leaving the game.


 5) Contact
 ----------

If you have questions, suggestions or patches, please write a mail to the
following address:

	huth at tuxfamily.org

Now have fun with Ballerburg!
