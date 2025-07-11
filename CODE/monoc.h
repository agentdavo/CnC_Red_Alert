/*
**	Command & Conquer Red Alert(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* $Header: /CounterStrike/MONOC.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : MONO.H                                                       *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : July 2, 1994                                                 *
 *                                                                                             *
 *                  Last Update : July 2, 1994   [JLB]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MONOC_Hx
#define MONOC_Hx

/*
**	The "bool" integral type was defined by the C++ committee in
**	November of '94. Until the compiler supports this, use the following
**	definition.
*/

class MonoClass {
	public:
		enum MonoClassPageEnums {
			COLUMNS=80,						// Number of columns.
			LINES=25,						// Number of lines.
			MAX_MONO_PAGES=16	// Maximum RAM pages on mono card.
		};

      typedef enum MonoAttribute {
			INVISIBLE=0x00,				// Black on black.
			UNDERLINE=0x01,				// Underline.
			BLINKING=0x90,					// Blinking white on black.
			NORMAL=0x02,					// White on black.
			INVERSE=0x70,					// Black on white.
      } MonoAttribute;

		/*
		**	These are the various box styles that may be used.
		*/
		typedef enum BoxStyleType {
			SINGLE,				// Single thickness.
			DOUBLE_HORZ,		// Double thick on the horizontal axis.
			DOUBLE_VERT,		// Double thick on the vertical axis.
			DOUBLE,				// Double thickness.

			COUNT
		} BoxStyleType;

		MonoClass(void);
		~MonoClass(void);

		static void Enable(void) {Enabled = true;};
		static void Disable(void) {Enabled = false;};
		static bool Is_Enabled(void) {return Enabled;};
		static MonoClass * Get_Current(void) {return PageUsage[0];};

		void Sub_Window(int x=0, int y=0, int w=-1, int h=-1);
		void Fill_Attrib(int x, int y, int w, int h, MonoAttribute attrib);
		void Draw_Box(int x, int y, int w, int h, MonoAttribute attrib=NORMAL, BoxStyleType thick=SINGLE);
		void Set_Default_Attribute(MonoAttribute attrib) {Attrib = attrib;};
		void Clear(void);
		void Set_Cursor(int x, int y);
		void Print(char const *text);
		void Print(int text);
		void Printf(char const *text, ...);
		void Printf(int text, ...);
		void Text_Print(char const *text, int x, int y, MonoAttribute attrib=NORMAL);
		void Text_Print(int text, int x, int y, MonoAttribute attrib=NORMAL);
		void View(void);
		void Scroll(int lines=1);
		void Pan(int cols=1);
		int Get_X(void) const {return X;};
		int Get_Y(void) const {return Y;};
		int Get_Width(void) const {return(SubW);};
		int Get_Height(void) const {return(SubH);};

		/*
		**	Handles deep copies for the mono class objects. This performs what is essentially
		**	a screen copy.
		*/
		MonoClass & operator = (MonoClass const & );

		/*
		**	This merely makes a duplicate of the mono object into a newly created mono
		**	object.
		*/
		MonoClass (MonoClass const &);

	private:

		/*
		**	Cursor coordinate (relative to sub-window).
		*/
		int X;
		int Y;

		/*
		**	Default attribute to use when printing text.
		*/
		MonoAttribute Attrib;

		/*
		**	The current physical page that this mono class object refers to.
		*/
		int Page;

		/*
		**	Sub window coordinates.
		*/
		int SubX;
		int SubY;
		int SubW;
		int SubH;

		/*
		**	Pointer to the monochrome RAM.
		*/
//		static MonoPageType * MonoRAM;

		/*
		** This the the arrays of characters used for drawing boxes.
		*/
		/*
		**	This is a private structure that is used to control which characters
		**	are used when a box is drawn. Line drawing on the monochrome screen is
		**	really made up of characters. This specifies which characters to use.
		*/
		struct BoxDataType {
			char	UpperLeft;
			char	TopEdge;
			char	UpperRight;
			char	RightEdge;
			char	BottomRight;
			char	BottomEdge;
			char	BottomLeft;
			char	LeftEdge;
		};
		static BoxDataType const CharData[4];

		/*
		**	Each cell is constructed of the actual character that is displayed and the
		**	attribute to use. This character pair is located at every position on the
		**	display (80 x 25). Since this cell pair can be represented by a "short"
		**	integer, certain speed optimizations are taken in the monochrome drawing
		**	code.
		*/
		struct CellType {
			unsigned char Character;	// Character to display.
			unsigned char Attribute;	// Attribute.
		};

		struct MonoPageType {
			CellType Data[LINES][COLUMNS];
		};

		/*
		**	These private constants are used in the various monochrome operations.
		*/
		enum MonoClassPortEnums {
			CONTROL_PORT=0x03B4,			// CRTC control register.
			DATA_PORT=0x03B5,				// CRTC data register.
			SIZE_OF_PAGE=(int)LINES*(int)COLUMNS*sizeof(CellType)	// Entire page size.
		};

		/*
		**	This array contains pointers to the monochrome objects that are assigned
		**	to each of the monochrome pages. As the monochrome pages are made visible,
		**	they can be shuffled around between the actual locations. The first entry
		**	in this table is the one that is visible.
		*/
		static MonoClass * PageUsage[MAX_MONO_PAGES];

		/*
		**	Fetches pointers to the appropriate mono RAM.
		*/
		MonoPageType * Raw_Ptr(int page) const {
			return &((MonoPageType *)0xB0000)[page];
		}
		MonoPageType * Page_Ptr(void) const {
			return(Raw_Ptr(Page));
		}

		/*
		**	If this is true, then monochrome output is allowed. It defaults to false
		**	so that monochrome output must be explicitly enabled.
		*/
		static bool Enabled;
};

#ifndef WIN32
int Mono_Printf(int string, ...);
#else
extern void Mono_Set_Cursor(int x, int y);
extern int Mono_Printf(int string, ...);
extern int Mono_Printf(char const * string, ...);
extern void Mono_Clear_Screen(void);
extern void Mono_Text_Print(void const *text, int x, int y, int attrib);
extern void Mono_Draw_Rect(int x, int y, int w, int h, int attrib, int thick);
extern void Mono_Print(void const *text);
extern int Mono_X(void);
extern int Mono_Y(void);
#endif

#endif

