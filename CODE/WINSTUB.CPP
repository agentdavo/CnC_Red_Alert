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

/* $Header: /CounterStrike/WINSTUB.CPP 3     3/13/97 2:06p Steve_tall $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : WINSTUB.CPP                                                  *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : 10/04/95                                                     *
 *                                                                                             *
 *                  Last Update : October 4th 1995 [ST]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Overview:                                                                                   *
 *   This file contains stubs for undefined externals when linked under Watcom for Win 95      *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 * Functions:                                                                                  *
 *   Assert_Failure -- display the line and source file where a failed assert occurred         *
 *   Check_For_Focus_Loss -- check for the end of the focus loss                               *
 *   Create_Main_Window -- opens the MainWindow for C&C                                        *
 *   Focus_Loss -- this function is called when a library function detects focus loss          *
 *   Memory_Error_Handler -- Handle a possibly fatal failure to allocate memory                *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"

#ifdef WINSOCK_IPX
#include "WSProto.h"
#else	//WINSOCK_IPX
#include "tcpip.h"
#include "ipx95.h"
#endif	//WINSOCK_IPX

void output(short,short)
{}


unsigned long CCFocusMessage = WM_USER+50;	//Private message for receiving application focus
extern	void VQA_PauseAudio(void);
extern	void VQA_ResumeAudio(void);

//#include "WolDebug.h"


/***********************************************************************************************
 * Focus_Loss -- this function is called when a library function detects focus loss            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    2/1/96 2:10PM ST : Created                                                               *
 *=============================================================================================*/

void Focus_Loss(void)
{
	Theme.Suspend();
	Stop_Primary_Sound_Buffer();
	if (WWMouse) WWMouse->Clear_Cursor_Clip();
}

void Focus_Restore(void)
{
	Restore_Cached_Icons();
	Map.Flag_To_Redraw(true);
	Start_Primary_Sound_Buffer(TRUE);
	if (WWMouse) WWMouse->Set_Cursor_Clip();
	VisiblePage.Clear();
	HiddenPage.Clear();
}



/***********************************************************************************************
 * Check_For_Focus_Loss -- check for the end of the focus loss                                 *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    2/2/96 10:49AM ST : Created                                                              *
 *=============================================================================================*/

void Check_For_Focus_Loss(void)
{
	static BOOL	focus_last_time = 1;
			 MSG	msg;


	if ( !GameInFocus ) {
		Focus_Loss();
                while ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE | PM_NOYIELD ) ) {
                        if (!GetMessage( &msg, NULL, 0, 0 ) ) {
                                return;
                        }
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
#ifdef USE_LVGL
                        lv_timer_handler();
#endif
                }
	}

	if (!focus_last_time && GameInFocus) {

		VQA_PauseAudio();
		CountDownTimerClass cd;
		cd.Set(60*1);

                do {
                        while (PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE )) {
                                if (!GetMessage( &msg, NULL, 0, 0 ) ) {
                                        return;
                                }
                                TranslateMessage(&msg);
                                DispatchMessage(&msg);
#ifdef USE_LVGL
                                lv_timer_handler();
#endif
                        }

                } while (cd.Time());
		VQA_ResumeAudio();
		PostMessage (MainWindow, CCFocusMessage, 0, 0);
//		AllSurfaces.Restore_Surfaces();
//		VisiblePage.Clear();
//		HiddenPage.Clear();
//		Map.Flag_To_Redraw(true);
	}

	focus_last_time = GameInFocus;

}



extern BOOL InMovie;

long FAR PASCAL _export Windows_Procedure(HWND hwnd, UINT message, UINT wParam, LONG lParam)
{

	int	low_param = LOWORD(wParam);

	if (message == CCFocusMessage) {
		Start_Primary_Sound_Buffer(TRUE);
		if (!InMovie) {
			Theme.Stop();
			Theme.Queue_Song(THEME_PICK_ANOTHER);
		}
		return(0);
	}

#ifdef WINSOCK_IPX
	/*
	** Pass on any messages intended for the winsock message handler.
	*/
	if ( PacketTransport ) {
		if ( message == (UINT) PacketTransport->Protocol_Event_Message() ) {
			if ( PacketTransport->Message_Handler (hwnd, message, wParam, lParam) ){
				return ( DefWindowProc (hwnd, message, wParam, lParam) );
			}else{
				return (0);
			}
		}
	}
#endif	//WINSOCK_IPX

	
	/*
	**	Pass this message through to the keyboard handler. If the message
	**	was processed and requires no further action, then return with
	**	this information.
	*/
	if (Keyboard->Message_Handler(hwnd, message, wParam, lParam)) {
		return(1);
	}

	switch ( message ) {
//		case WM_SYSKEYDOWN:
//			Mono_Printf("wparam=%08X lparam=%08X\n", (long)wParam, (long)lParam);
			// fall through

//		case WM_MOUSEMOVE:
//		case WM_KEYDOWN:
//		case WM_SYSKEYUP:
//		case WM_KEYUP:
//		case WM_LBUTTONDOWN:
//		case WM_LBUTTONUP:
//		case WM_LBUTTONDBLCLK:
//		case WM_MBUTTONDOWN:
//		case WM_MBUTTONUP:
//		case WM_MBUTTONDBLCLK:
//		case WM_RBUTTONDOWN:
//		case WM_RBUTTONUP:
//		case WM_RBUTTONDBLCLK:
//	 		Keyboard->Message_Handler(hwnd, message, wParam, lParam);
//			return(0);

			/*
			** Windoze message says we have to shut down. Try and do it cleanly.
			*/
		case WM_DESTROY:
			Prog_End();
			Invalidate_Cached_Icons();
			VisiblePage.Un_Init();
			HiddenPage.Un_Init();
			AllSurfaces.Release();
			if (!InDebugger) Reset_Video_Mode();
			Stop_Profiler();
			PostQuitMessage( 0 );

			/*
			** If we are shutting down gracefully than flag that the message loop has finished.
			** If this is a forced shutdown (ReadyToQuit == 0) then try and close down everything
			** before we exit.
			*/
			switch (ReadyToQuit) {
				case 1:
					ReadyToQuit = 2;
					break;

				case 0:
					Shutdown_Network();
#ifndef WINSOCK_IPX
					if (Winsock.Get_Connected()) Winsock.Close();
					/*
					** Free the THIPX32 dll
					*/
					Unload_IPX_Dll();
#endif	//WINSOCK_IPX
					ExitProcess(0);
					break;
				case 3:
					Shutdown_Network();
#ifndef WINSOCK_IPX
					/*
					** Call the function to disable the IPX callback as horrible things can
					** happen if we get a callback after the process has exited!
					*/
					if (Session.Type == GAME_IPX){
						IPX_Shut_Down95();
					}
					/*
					** Free the THIPX32 dll
					*/
#ifdef FIXIT_CSII	//	checked - ajw 9/28/98
#else
					Unload_IPX_Dll();
#endif

					if (Winsock.Get_Connected()) Winsock.Close();
#endif	//WINSOCK_IPX
					ReadyToQuit = 2;
					break;

			}
			return(0);

		case WM_ACTIVATEAPP:
			GameInFocus=(BOOL)wParam;
			if (!GameInFocus) Focus_Loss();
			AllSurfaces.Set_Surface_Focus (GameInFocus);
			AllSurfaces.Restore_Surfaces();
//			if (GameInFocus) {
//				Restore_Cached_Icons();
//				Map.Flag_To_Redraw(true);
//				Start_Primary_Sound_Buffer(TRUE);
//				if (WWMouse) WWMouse->Set_Cursor_Clip();
//			}
			return(0);
#ifdef NEVER
		case WM_ACTIVATE:
			if (low_param == WA_INACTIVE) {
				GameInFocus = FALSE;
				Focus_Loss();
			}
			return(0);
#endif	//NEVER


		case WM_SYSCOMMAND:
			switch ( wParam ) {

				case SC_CLOSE:
					/*
					** Windows sent us a close message. Probably in response to Alt-F4. Ignore it by
					** pretending to handle the message and returning true;
					*/
					return (0);

				case SC_SCREENSAVE:
					/*
					** Windoze is about to start the screen saver. If we just return without passing
					** this message to DefWindowProc then the screen saver will not be allowed to start.
					*/
					return (0);
			}
			break;


#ifndef WINSOCK_IPX
		case WM_ACCEPT:
		case WM_HOSTBYADDRESS:
		case WM_HOSTBYNAME:
		case WM_ASYNCEVENT:
		case WM_UDPASYNCEVENT:
	 		Winsock.Message_Handler(hwnd, message, wParam, lParam);
			return (0);
#endif	//WINSOCK_IPX
	}



	return DefWindowProc (hwnd, message, wParam, lParam);
}






HANDLE DebugFile = INVALID_HANDLE_VALUE;

/***********************************************************************************************
 * WWDebugString -- sends a string to the debugger and echos it to disk                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    string                                                                            *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    10/28/96 12:48PM ST : Created                                                              *
 *=============================================================================================*/
void WWDebugString (char *string)
{
#if (0)
	char outstr[256];

	sprintf (outstr, "%s", string);

	DWORD	actual;
	if (DebugFile == INVALID_HANDLE_VALUE){
		DebugFile = CreateFile("debug.txt", GENERIC_WRITE, 0,
												NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}else{
		DebugFile = CreateFile("debug.txt", GENERIC_WRITE, 0,
												NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	if (DebugFile != INVALID_HANDLE_VALUE){
		SetFilePointer (DebugFile, 0, NULL, FILE_END);
		WriteFile(DebugFile, outstr, strlen(outstr)+1, &actual, NULL);
		CloseHandle (DebugFile);
	}

	OutputDebugString (string);
#else	//(0)

	string = string;
//	debugprint( string );

#endif	//(0)

}









/***********************************************************************************************
 * Create_Main_Window -- opens the MainWindow for C&C                                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    instance -- handle to program instance                                            *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    10/10/95 4:08PM ST : Created                                                             *
 *=============================================================================================*/

#define CC_ICON 1

#if (ENGLISH)
#define WINDOW_NAME		"Red Alert"
#endif

#if (FRENCH)
#define WINDOW_NAME		"Alerte Rouge"
#endif

#if (GERMAN)
#define WINDOW_NAME		"Alarmstufe Rot"
#endif


void Create_Main_Window ( HANDLE instance , int command_show , int width , int height )

{
	HWND        	hwnd ;
	WNDCLASS    	wndclass ;
	//
	// Register the window class
	//

	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = Windows_Procedure ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = instance ;
	wndclass.hIcon         = LoadIcon (instance, MAKEINTRESOURCE(CC_ICON)) ;
	wndclass.hCursor       = NULL;
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName  = WINDOW_NAME;	//NULL
	wndclass.lpszClassName = WINDOW_NAME;

	RegisterClass (&wndclass) ;


	//
	// Create our main window
	//
	hwnd = CreateWindowEx (
							WS_EX_TOPMOST,
							WINDOW_NAME,
							WINDOW_NAME,
							WS_POPUP, // Denzil | WS_MAXIMIZE,
							0,
							0,
							// Denzil 5/18/98 - Making window fullscreen prevents other apps
							// from getting WM_PAINT messages
							GetSystemMetrics(SM_CXSCREEN), //width,
							GetSystemMetrics(SM_CYSCREEN), //height,
							// End Denzil
							NULL,
							NULL,
							instance,
							NULL );
// Denzil
width = width; height = height;
// End

	ShowWindow (hwnd, command_show );
	ShowCommand = command_show;
	UpdateWindow (hwnd);
	SetFocus (hwnd);
	MainWindow=hwnd;			//Save the handle to our main window
	hInstance = instance;

	CCFocusMessage = RegisterWindowMessage ("CC_GOT_FOCUS");

	Audio_Focus_Loss_Function = &Focus_Loss;
	Misc_Focus_Loss_Function = &Focus_Loss;
	Misc_Focus_Restore_Function = &Focus_Restore;
	Gbuffer_Focus_Loss_Function = &Focus_Loss;
}


void Window_Dialog_Box(HANDLE  hinst, LPCTSTR  lpszTemplate, HWND  hwndOwner, DLGPROC  dlgprc)
{
	MSG					msg;
	/*
	** Get rid of the Westwood mouse cursor because we are showing a
	** dialog box and we want to have the right windows cursor showing
	** for it.
	*/
	Hide_Mouse();
	ShowCursor(TRUE);

	/*
	** Pop up the dialog box and then run a standard message handler
	** until the dialog box is closed.
	*/

        DialogBox(hinst, lpszTemplate, hwndOwner, dlgprc);
        while (GetMessage(&msg, NULL, 0, 0) && !AllDone) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
#ifdef USE_LVGL
                lv_timer_handler();
#endif
        }

	/*
	** Restore the westwood mouse cursor and get rid of the windows one
	** because it is now time to restore back to the westwood way of
	** doing things.
	*/
	ShowCursor(FALSE);
	Show_Mouse();
}



typedef struct tColourList {

	char	Red;
	char	Green;
	char	Blue;
} ColourList;

ColourList	ColourLookup[9]={
	0,0,0,
	63,0,0,
	0,63,0,
	0,0,63,
	63,0,63,
	63,63,0,
	0,63,63,
	32,32,32,
	63,63,63
};




int	DebugColour=1;


extern "C" void Set_Palette_Register(int number, int red, int green, int blue);
#pragma off (unreferenced)
void Colour_Debug (int call_number)
{
	//#if 0
	//if (DebugColour==call_number || !call_number) {

		//if (call_number) {
		//	Wait_Vert_Blank();
		//}

		Set_Palette_Register (0,ColourLookup[call_number].Red ,
										ColourLookup[call_number].Green,
										ColourLookup[call_number].Blue);
	//}
	//#endif
}

#pragma on (unreferenced)





BOOL Any_Locked (void)
{
	if (SeenBuff.Get_LockCount() ||
		HidPage.Get_LockCount()) {
		return (TRUE);
	} else {
		return(FALSE);
	}
}








//
// Miscellaneous stubs. Mainly for multi player stuff
//
//
//

//IPXAddressClass::IPXAddressClass(void) {
//	int i;
//	i++;
//}
//int IPXManagerClass::Num_Connections(void) { return (0); }
//int IPXManagerClass::Connection_ID( int ) { return (0); }
//IPXAddressClass * IPXManagerClass::Connection_Address( int ) { return ((IPXAddressClass*)0); }
//char * IPXManagerClass::Connection_Name( int ) { return ((char*)0); }
//int IPXAddressClass::Is_Broadcast() { return (0); }
//int IPXManagerClass::Send_Global_Message( void *, int, int, IPXAddressClass * ) { return (0); }
//int IPXManagerClass::Service() { return (0); }
//int IPXManagerClass::Get_Global_Message( void  *, int  *, IPXAddressClass  *, short unsigned  * ) { return (0); }
//int IPXAddressClass::operator ==( IPXAddressClass  & ) { return (0); }
//IPXManagerClass::IPXManagerClass( int, int, int, int, short unsigned, short unsigned ) {}
//IPXManagerClass::~IPXManagerClass() {
//	int i;
//	i++;
//	}
//int  IPXManagerClass::Delete_Connection( int ) { return (0); }
//IPXAddressClass::IPXAddressClass( char unsigned  *, char unsigned  * ) {}
//void  IPXManagerClass::Set_Socket( short unsigned ) {}
//int  IPXManagerClass::Is_IPX() { return (0); }
//int  IPXManagerClass::Init() { return (0); }
//void  IPXAddressClass::Get_Address( char unsigned  *, char unsigned  * ) {}
//void  IPXManagerClass::Set_Bridge( char unsigned  * ) {}
//int  IPXManagerClass::Global_Num_Send() { return (0); }
//void  IPXManagerClass::Set_Timing( long unsigned, long unsigned, long unsigned ) {}
//unsigned long IPXManagerClass::Global_Response_Time() { return (0); }
//int  IPXManagerClass::Create_Connection( int, char  *, IPXAddressClass  * ) { return (0); }
//int  IPXAddressClass::operator !=( IPXAddressClass  & ) { return (0); }
//int  IPXManagerClass::Send_Private_Message( void  *, int, int, int ) { return (0); }
//int  IPXManagerClass::Get_Private_Message( void  *, int  *, int  * ) { return (0); }
//int  IPXManagerClass::Connection_Index( int ) { return (0); }
//void  IPXManagerClass::Reset_Response_Time() {}
//long unsigned  IPXManagerClass::Response_Time() { return (0); }
//int  IPXManagerClass::Private_Num_Send( int ) { return (0); }

//_VQAHandle  *  VQA_Alloc(void) { return ((_VQAHandle *)0); }
//void  VQA_Init( _VQAHandle  *, long ( *)()) {}
//long  VQA_Open( _VQAHandle  *, char const  *, _VQAConfig  * ) { return (0); }
//void  VQA_Free( _VQAHandle  * ) {}
//void  VQA_Close( _VQAHandle  * ) {}
//long  VQA_Play( _VQAHandle  *, long ) { return (0); }

//void VQA_Init(VQAHandle *, long(*)(VQAHandle *vqa, long action,	void *buffer, long nbytes)) {}

//long VQA_Open(VQAHandle *, char const *, VQAConfig *)
//{
//	return (0);
//}

//void VQA_Close(VQAHandle *) {}

//long VQA_Play(VQAHandle *, long)
//{
//	return (0);
//}


unsigned char	*VQPalette;
long				VQNumBytes;
unsigned long	VQSlowpal;
bool				VQPaletteChange = false;


extern "C"{
	void __cdecl SetPalette(unsigned char *palette, long numbytes, unsigned long slowpal);
}



void Flag_To_Set_Palette(unsigned char *palette, long numbytes, unsigned long slowpal)
{
	VQPalette = palette;
	VQNumBytes = numbytes;
	VQSlowpal = slowpal;
	VQPaletteChange = true;
}



void Check_VQ_Palette_Set(void)
{
	if (VQPaletteChange) {
		SetPalette(VQPalette, VQNumBytes, VQSlowpal);
		VQPaletteChange = false;
	}
}





void __cdecl SetPalette(unsigned char *palette, long, unsigned long)
{
	for (int i=0 ; i<256*3 ; i++) {
		*(palette+i)&=63;
	}
	Increase_Palette_Luminance(palette , 15 , 15 , 15 ,63);

	if (PalettesRead) {
		memcpy (&PaletteInterpolationTable[0][0] , InterpolatedPalettes[PaletteCounter++] , 65536);
	}
	Set_Palette(palette);
}


#ifndef NDEBUG
/***********************************************************************************************
 * Assert_Failure -- display the line and source file where a failed assert occurred           *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    line number in source file                                                        *
 *           name of source file                                                               *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    4/17/96 9:58AM ST : Created                                                              *
 *=============================================================================================*/

void Assert_Failure (char *expression, int line, char *file)
{
	char	assertbuf[256];
	char   timebuff[512];
	SYSTEMTIME	time;

	sprintf (assertbuf, "assert '%s' failed at line %d in module %s.\n", expression, line, file);

	if (!MonoClass::Is_Enabled()) MonoClass::Enable();

	Mono_Clear_Screen();
	Mono_Printf("%s", assertbuf);

	WWDebugString(assertbuf);

	GetLocalTime(&time);

	sprintf (timebuff, "%02d/%02d/%04d %02d:%02d:%02d - %s", time.wMonth, time.wDay, time.wYear,
																		time.wHour, time.wMinute, time.wSecond,
																		assertbuf);


	HMMIO handle = mmioOpen("ASSERT.TXT", NULL, MMIO_WRITE);
	if (!handle) {
		handle = mmioOpen("ASSERT.TXT", NULL, MMIO_CREATE | MMIO_WRITE);
		//mmioClose(handle, 0);
		//handle = mmioOpen("ASSERT.TXT", NULL, MMIO_WRITE);
	}

	if (handle) {

		mmioWrite(handle, timebuff, strlen(timebuff));
		mmioClose(handle, 0);
	}

	WWMessageBox().Process(assertbuf);
//	WWMessageBox().Process("Red Alert demo timed out - Aborting");
	//Get_Key();

	Prog_End();
	Invalidate_Cached_Icons();
	PostQuitMessage( 0 );
	ExitProcess(0);
}
#endif







/***********************************************************************************************
 * Memory_Error_Handler -- Handle a possibly fatal failure to allocate memory                  *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/22/96 3:57PM ST : Created                                                              *
 *=============================================================================================*/
void Memory_Error_Handler(void)
{
	VisiblePage.Clear();
	CCPalette.Set();
	while (Get_Mouse_State()) {Show_Mouse();};
	WWMessageBox().Process(TEXT_MEMORY_ERROR, TEXT_ABORT, false);

	ReadyToQuit = 1;

        PostMessage(MainWindow, WM_DESTROY, 0, 0);
        do
        {
                Keyboard->Check();
#ifdef USE_LVGL
                lv_timer_handler();
#endif
        }while (ReadyToQuit == 1);

	ExitProcess(0);
}








GraphicBufferClass* Read_PCX_File(char* name, char* Palette, void *Buff, long Size);
void Load_Title_Screen(char *name, GraphicViewPortClass *video_page, unsigned char *palette)
{

	GraphicBufferClass *load_buffer;


	load_buffer = Read_PCX_File (name, (char*)palette, NULL, 0);

	if (load_buffer) {
		load_buffer->Blit(*video_page);
		delete load_buffer;
	}
}



#include "filepcx.h"

/***************************************************************************
 * READ_PCX_FILE -- read a pcx file into a Graphic Buffer                  *
 *                                                                         *
 *	GraphicBufferClass* Read_PCX_File (char* name, char* palette ,void *Buff, long size );	*
 *  																								*
 *                                                                         *
 * INPUT: name is a NULL terminated string of the format [xxxx.pcx]        *
 *        palette is optional, if palette != NULL the the color palette of *
 *					 the pcx file will be place in the memory block pointed	   *
 *               by palette.																*
 *			 Buff is optional, if Buff == NULL a new memory Buffer		 		*
 *					 will be allocated, otherwise the file will be placed 		*
 *					 at location pointed by Buffer;										*
 *			Size is the size in bytes of the memory block pointed by Buff		*
 *				  is also optional;															*                                                                         *
 * OUTPUT: on success a pointer to a GraphicBufferClass containing the     *
 *         pcx file, NULL otherwise.                                       *
 *																									*
 * WARNINGS:                                                               *
 *         Appears to be a comment-free zone                               *
 *                                                                         *
 * HISTORY:                                                                *
 *   05/03/1995 JRJ : Created.                                             *
 *   04/30/1996 ST : Tidied up and modified to use CCFileClass             *
 *=========================================================================*/

#define	POOL_SIZE 2048
#define	READ_CHAR()  *file_ptr++ ; \
							 if ( file_ptr	>= & pool [ POOL_SIZE ]	) { \
								 file_handle.Read (pool , POOL_SIZE ); \
								 file_ptr = pool ; \
							 }


GraphicBufferClass* Read_PCX_File(char* name, char* palette, void *Buff, long Size)
{
	unsigned					i, j;
	unsigned					rle;
	unsigned					color;
	unsigned					scan_pos;
	char						*file_ptr;
	int						width;
	int						height;
	char						*buffer;
	PCX_HEADER				header;
	RGB						*pal;
	char						pool [POOL_SIZE];
	GraphicBufferClass	*pic;

	CCFileClass file_handle(name);

	if (!file_handle.Is_Available()) return (NULL);

	file_handle.Open(READ);

	file_handle.Read (&header, sizeof (PCX_HEADER));

	if (header.id != 10 &&  header.version != 5 && header.pixelsize != 8 ) return NULL ;

	width = header.width - header.x + 1;
	height = header.height - header.y + 1;

	if (Buff) {
    	buffer = (char *)Buff;
    	i = Size / width;
    	height = MIN (i - 1, height);
    	pic = new GraphicBufferClass(width, height, buffer, Size);
    	if ( !(pic && pic->Get_Buffer()) ) return NULL ;
	} else {
    	pic = new GraphicBufferClass(width, height, NULL, width*(height+4));
    	if ( !(pic && pic->Get_Buffer()) ) return NULL ;
	}

	buffer = (char *) pic->Get_Buffer();
	file_ptr = pool ;
	file_handle.Read (pool , POOL_SIZE);

	if ( header.byte_per_line != width ) {

		for ( scan_pos = j = 0 ; j < height ; j ++, scan_pos += width ) {
			for ( i = 0 ; i < width ; ) {
				rle = READ_CHAR ();
				if ( rle > 192 ) {
					rle -= 192 ;
					color =	READ_CHAR (); ;
					memset ( buffer + scan_pos + i , color , rle );
					i += rle;
				} else {
					*(buffer+scan_pos + i++ ) = (char)rle;
				}
			}
      	}

		if ( i == width ) rle = READ_CHAR ();
		if ( rle > 192 )  rle = READ_CHAR ();

	} else {

		for ( i = 0 ; i < width * height ; ) {
  			rle = READ_CHAR ();
  			rle &= 0xff;
  			if ( rle > 192 ) {
        		rle -= 192 ;
        		color = READ_CHAR ();
  				memset ( buffer + i , color , rle );
        		i += rle ;
     		} else {
				*(buffer + i++) = (char)rle;
			}
		}
	}

	if ( palette ) {
		file_handle.Seek (- (256 * sizeof ( RGB )) , SEEK_END );
		file_handle.Read (palette , 256L * sizeof ( RGB ));

		pal = ( RGB * ) palette;
		for (i = 0 ; i < 256 ; i ++) {
			pal ->red	>>= 2;
			pal ->green	>>= 2;
			pal ->blue	>>= 2;
			pal ++ ;
		}
	}

	file_handle.Close();
	return pic;
}
