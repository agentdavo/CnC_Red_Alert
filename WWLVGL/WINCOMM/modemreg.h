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


#ifndef WIN32
#define WIN32
#ifndef _WIN32 // Denzil 6/2/98 Watcom 11.0 complains without this check
#define _WIN32
#endif // _WIN32
#endif	//WIN32



class ModemRegistryEntryClass {

	public:

		ModemRegistryEntryClass (int modem_number);
		~ModemRegistryEntryClass (void);


		char *Get_Modem_Name (void) { return (ModemName); }

		char *Get_Modem_Device_Name (void) { return (ModemDeviceName); }

		char *Get_Modem_Error_Correction_Enable (void) { return (ErrorCorrectionEnable); }

		char *Get_Modem_Error_Correction_Disable (void) { return (ErrorCorrectionDisable); }

		char *Get_Modem_Compression_Enable (void) { return (CompressionEnable); }

		char *Get_Modem_Compression_Disable (void) { return (CompressionDisable); }

		char *Get_Modem_Hardware_Flow_Control (void) { return (HardwareFlowControl); }

		char *Get_Modem_No_Flow_Control (void) { return (HardwareFlowControl); }

	private:

		char *ModemName;
		char *ModemDeviceName;
		char *ErrorCorrectionEnable;
		char *ErrorCorrectionDisable;
		char *CompressionEnable;
		char *CompressionDisable;
		char *HardwareFlowControl;
		char *NoFlowControl;

};







