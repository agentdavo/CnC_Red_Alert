/*
 * audio/soundlck.c - sound driver locking helpers
 * Last updated: 2025-06-24
 */

/***************************************************************************
 **      C O N F I D E N T I A L --- W E S T W O O D   S T U D I O S      **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Westwood 32 bit Library                  *
 *                                                                         *
 *                    File Name : soundlck.c                             *
 *                                                                         *
 *                   Programmer : Phil W. Gorrow                           *
 *                                                                         *
 *                   Start Date : June 23, 1995                            *
 *                                                                         *
 *                  Last Update : June 23, 1995   [PWG]                    *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <string.h>
#include "soundint.h"



LockedDataType LockedData;

/*=========================================================================*/
/* The following PRIVATE functions are in this file:                       */
/*=========================================================================*/


/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

/***************************************************************************
 * INIT_LOCKED_DATA -- Initializes sound driver locked data                *
 *                                                                         *
 * INPUT:		none                                                          *
 *                                                                         *
 * OUTPUT:     none                                                        *
 *                                                                         *
 * HISTORY:                                                                *
 *   06/23/1995 PWG : Created.                                             *
 *=========================================================================*/
void Init_Locked_Data(void)
{

	/*
	** Initialize all of the data elements that need to be locked.
	*/
	LockedData.DigiHandle			= -1;
	LockedData.ServiceSomething	= FALSE;
	LockedData.MagicNumber			= 0xDEAF;
	LockedData.UncompBuffer			= NULL;
//	LockedData.StreamBufferSize	= (2*SECONDARY_BUFFER_SIZE)+128;
	LockedData.StreamBufferSize	= (SECONDARY_BUFFER_SIZE/4)+128;
	LockedData.StreamBufferCount	= 16;
	LockedData.SoundVolume			= 255;
	LockedData.ScoreVolume			= 255;
	LockedData._int					= FALSE;

}

