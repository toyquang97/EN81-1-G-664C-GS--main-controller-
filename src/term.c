
#include "typedef.h"
#include "parameter.h"
#include "state.h"
#include "call.h"
#include "can.h"
#include "drive.h"
#include "nice5000.h"
#include "io.h"


/****************************************************************************************************/
/* terminal function																		*/
/****************************************************************************************************/
void terminal_state (void){
	BYTE doorcommand = 0;

	nice5000_Tcmd_data2 = (nice5000_Tcmd_data2 & (~(NICE5000_TCMD2_STATE|NICE5000_TCMD2_TARGET))) | TCMD2_STATE_INSPECTION;
	send_hse_state ();
	calls_off = 1;										/* disable calls						*/
	clearcalls (ALL_CALLS);								/* clear all calls						*/
	fault_signal = 0;									/* reset fault signal					*/
	out_of_order = 1;									/* set out of order indication			*/
	triperrorcount = 0;								/* reset error counter							*/

	while (1)
	{
		cycle ();
		set_state ();									/* set new state						*/
		if (hse_state != H_TERM)						/* state changed						*/
		{
			if (drive_state != D_STOPPED)				/* drive not in stopped state			*/
			{
				emergency_stop ();					/* stop drive							*/
				hse_state = H_WAIT_STOP;				/* wait for stop						*/
			}
			return;								/* go to new state					*/
		}

		switch (term_active & 0x07)
		{
			case (3):									/* up direction						*/
				if (drive_state == D_STOPPED)			/* drive in stopped state				*/
				{
					if (!doorcommand)					/* no door command					*/
					{
						door_command (CLOSE_DOOR, CLOSE_DOOR);
						doorcommand = 1;
					}
					else if ((safety_circuit & SC3)
							&&((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == RCMD2_STATE_INSPECTION))
						start_drive (DIR_UP);
				}
				else									/* drive in moving state				*/
				{
					if (direction == DIR_DN)				/* drive is moving downwards			*/
						emergency_stop ();			/* stop drive							*/
				}
				if((door_zone & POS_ILU) && (door_zone & POS_SDU1) && (drive_state != D_STOPPED) )
					{
						stop_drive();
						doorcommand = 0;
					}
				break;

			case (5):									/* down direction						*/
				if (drive_state == D_STOPPED)			/* drive in stopped state				*/
				{
					if (!doorcommand)					/* no door command					*/
					{
						door_command (CLOSE_DOOR, CLOSE_DOOR);
						doorcommand = 1;
					}
					else if ((safety_circuit & SC3)
							&&((nice5000_Rcmd_data2 & NICE5000_RCMD2_STATE) == RCMD2_STATE_INSPECTION))
						start_drive (DIR_DN);
				}
				else									/* drive in moving state				*/
				{
					if (direction == DIR_UP)				/* drive is moving upwards			*/
						emergency_stop ();				/* stop drive					*/
				}
				if((door_zone & POS_ILD) && (door_zone & POS_SDD1) && (drive_state != D_STOPPED) )
					{
						stop_drive();
						doorcommand = 0;
					}
				break;

			case (7):
			case 6:
				stop_drive();
				doorcommand = 0;
				break;

			default:
				if (drive_state != D_STOPPED)			/* drive not in stopped state			*/
					emergency_stop ();				/* stop drive							*/
				doorcommand = 0;
				break;
		}
		
		fan_on ();
	}
}


