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

#include <stdbool.h>

#ifndef BOOL
#define BOOL int
#endif

#ifndef __stdcall
#define __stdcall
#endif

/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S        **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : IPX95PP                                  *
 *                                                                         *
 *                   Programmer : Steve Tall                               *
 *                                                                         *
 *                   Start Date : January 22nd, 1996                       *
 *                                                                         *
 *                  Last Update : January 22nd, 1996   [ST]                *
 *                                                                         *
 *-------------------------------------------------------------------------*
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



/*
** Types for function pointers
*/
typedef BOOL __stdcall (*IPXInitialiseType) (void);
typedef BOOL __stdcall (*IPXGetOutstandingBuffer95Type) (unsigned char*);
typedef void __stdcall (*IPXShutDown95Type) (void);
typedef int  __stdcall (*IPXSendPacket95Type) (unsigned char *, unsigned char *, int, unsigned char*, unsigned char*);
typedef int  __stdcall (*IPXBroadcastPacket95Type) (unsigned char *, int);
typedef BOOL __stdcall (*IPXStartListening95Type) (void);
typedef int  __stdcall (*IPXOpenSocket95Type) (int);
typedef void __stdcall (*IPXCloseSocket95Type) (int);
typedef int  __stdcall (*IPXGetConnectionNumber95Type) (void);
typedef int  __stdcall (*IPXGetLocalTarget95) (unsigned char *, unsigned char*, unsigned short, unsigned char*);



/*
** Function pointers
*/
//extern "C"{
	extern IPXInitialiseType					IPX_Initialise;
	extern IPXGetOutstandingBuffer95Type  	IPX_Get_Outstanding_Buffer95;
	extern IPXShutDown95Type              	IPX_Shut_Down95;
	extern IPXSendPacket95Type            	IPX_Send_Packet95;
	extern IPXBroadcastPacket95Type       	IPX_Broadcast_Packet95;
	extern IPXStartListening95Type        	IPX_Start_Listening95;
	extern IPXOpenSocket95Type            	IPX_Open_Socket95;
	extern IPXCloseSocket95Type           	IPX_Close_Socket95;
	extern IPXGetConnectionNumber95Type   	IPX_Get_Connection_Number95;
	extern IPXGetLocalTarget95            	IPX_Get_Local_Target95;
//}

/*
** Functions
*/
bool Load_IPX_Dll (void);
void Unload_IPX_Dll (void);


#if (0)
extern "C"{
	extern BOOL __stdcall IPX_Initialise(void);
	extern BOOL __stdcall IPX_Get_Outstanding_Buffer95(unsigned char *buffer);
	extern void __stdcall IPX_Shut_Down95(void);
	extern int  __stdcall IPX_Send_Packet95(unsigned char *, unsigned char *, int, unsigned char*, unsigned char*);
	extern int  __stdcall IPX_Broadcast_Packet95(unsigned char *, int);
	extern BOOL __stdcall IPX_Start_Listening95(void);
	extern int  __stdcall IPX_Open_Socket95(int socket);
	extern void __stdcall IPX_Close_Socket95(int socket);
	extern int  __stdcall IPX_Get_Connection_Number95(void);
	extern int  __stdcall IPX_Get_Local_Target95(unsigned char *, unsigned char*, unsigned short, unsigned char*);
}
#endif	//(0)

extern bool WindowsNT;
