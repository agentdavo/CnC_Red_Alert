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

#ifndef _MCI_H_
#define _MCI_H_
/****************************************************************************
*
* FILE
*     MCI.H
*
* DESCRIPTION
*
* PROGRAMMER
*     Denzil E. Long, Jr.
*
* DATE
*     6/22/98
*
****************************************************************************/

#include "function.h"

#ifdef MCIMPEG
#include <windows.h>
#include <mmsystem.h>
#include <digitalv.h>

/* MCIDevice - MCI device capabilities and description
 *
 * name        - Name used to open device.
 * description - Product description
 * type        - Device type
 * canEject    - Can eject media flag
 * canPlay     - Can playback media
 * canRecord   - Can record media
 * canSave     - Can save media
 * usesDevElem - Uses device element
 * hasAudio    - Media supports audio
 * hasVideo    - Media supports video
 * reqElemFile - Requires element file
 */
typedef struct _MCIDevice
	{
	char name[64];
	char description[64];
	unsigned long type;
	bool canEject;
	bool canPlay;
	bool canRecord;
	bool canSave;
	bool usesDevElem;
	bool hasAudio;
	bool hasVideo;
	bool reqElemFile;
	} MCIDevice;

/* MCI enumeration callback definition */
typedef bool (MCIEnumCB)(MCIDevice* desc, void*);

class MCI
	{
	public:
		// Open MCI device
		MCIDEVICEID OpenDevice(const char* name);
		void CloseDevice(MCIDEVICEID id);

		// Enumerate devices
		bool EnumerateDevices(MCIEnumCB* callback, void* context);

		// Get number of MCI devices name in registry or [MCI] section
		// of system.ini
		unsigned int GetDeviceCount(void);

		// Get device name from registry or [MCI] section of system.ini
		bool GetDeviceName(unsigned int item, char* buffer);

		// Get general device description
		bool GetDeviceDescription(const char* name, MCIDevice* caps);

		// Get type name (IE: Digital Video) from type ID (IE: MCI_DEVTYPE_DIGITAL_VIDEO)
		const char* GetDeviceTypeName(unsigned long type);

		// Get device product name
		bool GetProductName(MCIDEVICEID id, char* buffer);

		// Get device capability
		bool GetCapability(MCIDEVICEID id, unsigned long capItem,
				unsigned long* result);
	};

#endif // MCIMPEG
#endif // _MCI_H_
