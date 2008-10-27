/*
 * $Id$
 *  
 * Copyright (C) 2003  Pascal Brisset, Antoine Drouin
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA. 
 *
 */
/** \file main.c
 *  \brief Regroup main functions
 *
 */

/* --- stuff pulled in from other files ---- sanjit */

// timer.h mostly

#define TCCR1A 0x00  /* sanjit guess from other files */
#define TCCR1B 0x01  /* sanjit guess from other files */
#define TCCR2 0x05  /* sanjit guess from other files */

#define TCNT1 0x1 /* sanjit BIG guess */
#define TCNT1L 0x1 /* sanjit BIG guess */
#define TIFR 0x1 /* sanjit BIG guess */
#define TOV2 0x1 /* sanjit BIG guess */


/*--------------------------------------------- sanjit */


/*
#include "link_autopilot.h"

//sanjit #include "timer.h"
#include "adc.h"
#include "pid.h"
#include "gps.h"
#include "infrared.h"
// sanjit #include "downlink.h"
#include "nav.h"
#include "autopilot.h"
#include "estimator.h"
#include "if_calib.h"
*/



/**** similar stuff as for cctask below ------------- */


#define  PPRZ_MODE_MANUAL   0
#define  PPRZ_MODE_AUTO1    1
#define  PPRZ_MODE_AUTO2    2
#define  PPRZ_MODE_HOME     3
#define  PPRZ_MODE_NB       4

#define TRIM_PPRZ(pprz) (pprz <  MIN_PPRZ ? MIN_PPRZ :  \
                         (pprz >  MAX_PPRZ ? MAX_PPRZ : \
                                   pprz))

#define TRIM_UPPRZ(pprz) (pprz <  0 ? 0 :  \
                          (pprz >  MAX_PPRZ ? MAX_PPRZ : \
                                    pprz))


/* from autopilot.h ends */

/* from include/std.h */
#define FALSE 0
#define TRUE (!FALSE)
/* include/std.h */

#define NAV_PITCH 0 /* from var/....h */

/* added by sanjit */
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef unsigned char bool_t;
/* sanjit add ends */


/* from sw/var/include/airframe.h */
#define ROLL_PGAIN 10000.
#define PITCH_OF_ROLL 0.0
#define PITCH_PGAIN 15000.
#define MAX_ROLL 0.35
#define MAX_PITCH 0.35
#define MIN_PITCH -0.35

#define CLIMB_PITCH_PGAIN -0.1
#define CLIMB_PITCH_IGAIN 0.025
#define CLIMB_PGAIN -0.03
#define CLIMB_IGAIN 0.1
#define CLIMB_MAX 1.
#define CLIMB_LEVEL_GAZ 0.31
#define CLIMB_PITCH_OF_VZ_PGAIN 0.05
#define CLIMB_GAZ_OF_CLIMB 0.2
/* airframe.h */


#define VERTICAL_MODE_MANUAL      0
#define VERTICAL_MODE_AUTO_GAZ    1
#define VERTICAL_MODE_AUTO_CLIMB  2
#define VERTICAL_MODE_AUTO_ALT    3
#define VERTICAL_MODE_NB          4

#define MAX_CLIMB_SUM_ERR 100
#define MAX_PITCH_CLIMB_SUM_ERR 100

/*---- from fly_by_wire/link_autopilot.h */
/*
 * System clock in MHz.
 */
#define CLOCK		16

/* !!!!!!!!!!!!!!!!!!! Value used in gen_airframe.ml !!!!!!!!!!!!!!!!! */
#define MAX_PPRZ (600 * CLOCK)
#define MIN_PPRZ -MAX_PPRZ
/* --- fly_by_wire/link_autopilot.h */

/* from main.c */
// defined below uint8_t pprz_mode;
// defined below uint8_t vertical_mode = VERTICAL_MODE_MANUAL;
static bool_t low_battery = FALSE;

/* end of stuff from main.c */

#define ALTITUDE_PGAIN -0.025
#define LATERAL_MODE_MANUAL    0


uint16_t nav_desired_gaz;
uint16_t estimator_flight_time;
bool_t launch = FALSE;

float nav_pitch = NAV_PITCH;
float estimator_z_dot;
bool_t auto_pitch = FALSE;

/* below vars from pid.c (mostly) */

float desired_roll = 0.;
float desired_pitch = 0.;
int16_t desired_gaz, desired_aileron, desired_elevator;
float roll_pgain = ROLL_PGAIN;
float pitch_pgain = PITCH_PGAIN;
float pitch_of_roll = PITCH_OF_ROLL;

float pitch_of_vz_pgain = CLIMB_PITCH_OF_VZ_PGAIN;
float pitch_of_vz = 0.;

const float climb_pgain   = CLIMB_PGAIN;
const float climb_igain   =  CLIMB_IGAIN;
// defined below float desired_climb = 0., pre_climb = 0.;
static const float level_gaz = CLIMB_LEVEL_GAZ;
float climb_sum_err  = 0;

float climb_pitch_pgain = CLIMB_PITCH_PGAIN;
float climb_pitch_igain = CLIMB_PITCH_IGAIN;
float climb_pitch_sum_err = 0.;
float max_pitch = MAX_PITCH;
float min_pitch = MIN_PITCH;


/**** similar stuff as for cctask above ------------- */


//
//
// FIXME estimator_flight_time should not be manipuled here anymore
//
/** Define minimal speed for takeoff in m/s */
#define MIN_SPEED_FOR_TAKEOFF 5.

uint8_t pprz_mode = PPRZ_MODE_MANUAL;
uint8_t vertical_mode = VERTICAL_MODE_MANUAL;
uint8_t lateral_mode = LATERAL_MODE_MANUAL;

float desired_climb = 0., pre_climb = 0.;
float altitude_pgain = ALTITUDE_PGAIN;

float estimator_z = 0.0, desired_altitude = 0.0; // init by sanjit

/*====================================================================*/




void altitude_control_task(void)
{
	if (pprz_mode == PPRZ_MODE_AUTO2 || pprz_mode == PPRZ_MODE_HOME) {
	  if (vertical_mode == VERTICAL_MODE_AUTO_ALT) {
	    /* inlined below 	    altitude_pid_run();   */
	    float err = estimator_z - desired_altitude;
	    desired_climb = pre_climb + altitude_pgain * err;
	    if (desired_climb < -CLIMB_MAX) desired_climb = -CLIMB_MAX;
	    if (desired_climb > CLIMB_MAX) desired_climb = CLIMB_MAX;
	  }
	}
}



int main()
{
    pprz_mode = 1; // different from 2 and 3

  altitude_control_task();

#ifdef PRET
  asm(".word 0x22222222");
#endif
}


    
