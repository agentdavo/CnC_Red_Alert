/*
** Command & Conquer Red Alert(tm)
** Copyright 2025 Electronic Arts Inc.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/***************************************************************************
 *                                                                         *
 *                 Project Name : Command & Conquer                        *
 *                                                                         *
 *                    File Name : STAGE.H                                  *
 *                                                                         *
 *                   Programmer : Joe L. Bostic                            *
 *                                                                         *
 *                   Start Date : June 17, 1994                            *
 *                                                                         *
 *                  Last Update : June 19, 2025                           *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef STAGE_H
#define STAGE_H

#include "ftimer.h"

/* Stage structure for animation timing */
typedef struct Stage {
    UINT stage;     /* Current animation stage */
    CDTimer *timer; /* Countdown timer for stage changes */
    INT rate;       /* Rate of stage changes */
} Stage;

/* Function prototypes for stage management */
Stage *Stage_Create(VOID);
VOID Stage_Destroy(Stage *stage);
INT Stage_Fetch_Stage(const Stage *stage);
INT Stage_Fetch_Rate(const Stage *stage);
VOID Stage_Set_Stage(Stage *stage, INT stage_val);
VOID Stage_Set_Rate(Stage *stage, INT rate);
VOID Stage_AI(Stage *stage);
BOOL Stage_About_To_Change(const Stage *stage);
BOOL Stage_Graphic_Logic(Stage *stage);

#endif /* STAGE_H */