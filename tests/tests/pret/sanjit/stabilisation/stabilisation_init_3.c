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
// static bool_t low_battery = FALSE;

/* end of stuff from main.c */

#define ALTITUDE_PGAIN -0.025
#define LATERAL_MODE_MANUAL    0


uint16_t nav_desired_gaz;
uint16_t estimator_flight_time;
//bool_t launch = FALSE;

float nav_pitch = NAV_PITCH;
float estimator_z_dot;
//bool_t auto_pitch = FALSE;

/* below vars from pid.c (mostly) */

//float desired_roll = 0.;
//float desired_pitch = 0.;
int16_t desired_gaz, desired_aileron, desired_elevator;
//float roll_pgain = ROLL_PGAIN;
//float pitch_pgain = PITCH_PGAIN;
//float pitch_of_roll = PITCH_OF_ROLL;

//float pitch_of_vz_pgain = CLIMB_PITCH_OF_VZ_PGAIN;
//float pitch_of_vz = 0.;

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


#define IR_ESTIM_MODE_ON 1

#define IR_ROLL_NEUTRAL_DEFAULT -915
#define IR_PITCH_NEUTRAL_DEFAULT 110
#define AV_NB_SAMPLE 0x20


// from radio.h, airframe.h
#define RADIO_CTL_NB 9
#define RADIO_CTL_D 0
#define RADIO_THROTTLE RADIO_CTL_D
#define RADIO_CTL_C 1
#define RADIO_ROLL RADIO_CTL_C
#define RADIO_CTL_B 2
#define RADIO_PITCH RADIO_CTL_B
#define RADIO_CTL_E 5
#define RADIO_GAIN1 RADIO_CTL_E
#define IR_RollOfIrs(x1,x2) (-1*(x1)+ -1*(x2))
#define IR_PitchOfIrs(x1,x2) (-1*(x1)+ 1*(x2))

struct inter_mcu_msg {
  int16_t channels[RADIO_CTL_NB];
  uint8_t ppm_cpt;
  uint8_t status;
  uint8_t nb_err;
  uint8_t vsupply; /* 1e-1 V */
};


//
//
// FIXME estimator_flight_time should not be manipuled here anymore
//
/** Define minimal speed for takeoff in m/s */
#define MIN_SPEED_FOR_TAKEOFF 5.


uint8_t fatal_error_nb = 0;
static const uint16_t version = 1;

/**  in seconds */
static uint16_t cputime = 0;

uint8_t pprz_mode = PPRZ_MODE_MANUAL;
uint8_t vertical_mode = VERTICAL_MODE_MANUAL;
uint8_t lateral_mode = LATERAL_MODE_MANUAL;
uint8_t ir_estim_mode = IR_ESTIM_MODE_ON;
bool_t auto_pitch = FALSE;

bool_t rc_event_1, rc_event_2;

uint8_t vsupply;

static uint8_t  mcu1_status, mcu1_ppm_cpt;

static bool_t low_battery = FALSE;

float slider_1_val, slider_2_val;

bool_t launch = FALSE;


#define Min(x, y) (x < y ? x : y)
#define Max(x, y) (x > y ? x : y)


#define NO_CALIB               0	/**< \enum No calibration state */
#define WAITING_CALIB_CONTRAST 1	/**< \enum Waiting calibration contrast state */
#define CALIB_DONE             2	/**< \enum Calibration done state */

/** Maximal delay for calibration */
#define MAX_DELAY_FOR_CALIBRATION 10



#ifdef EST_TEST
float est_pos_x;
float est_pos_y;
float est_fcourse;
uint8_t ticks_last_est; // 20Hz
#endif /* EST_TEST */



/* 
   called at 20Hz.
   sends a serie of initialisation messages followed by a stream of periodic ones
*/ 

/** Define number of message at initialisation */
#define INIT_MSG_NB 2
/** @@@@@ A FIXER @@@@ */
#define HI_FREQ_PHASE_NB  5

//static char signature[16] = MESSAGES_MD5SUM;

/** \def PERIODIC_SEND_BAT()
 *  @@@@@ A FIXER @@@@@
 */
#define PERIODIC_SEND_BAT() DOWNLINK_SEND_BAT(&vsupply, &estimator_flight_time, &low_battery, &block_time, &stage_time)
/** \def EventPos(_cpt, _channel, _event)
 *  @@@@@ A FIXER @@@@@
 */
#define PERIODIC_SEND_DEBUG() DOWNLINK_SEND_DEBUG(&link_fbw_nb_err, &link_fbw_fbw_nb_err, &modem_nb_ovrn, &gps_nb_ovrn, &mcu1_ppm_cpt);
/** \def EventPos(_cpt, _channel, _event)
 *  @@@@@ A FIXER @@@@@
 */
#define PERIODIC_SEND_ATTITUDE() DOWNLINK_SEND_ATTITUDE(&estimator_phi, &estimator_psi, &estimator_theta);  
/** \def EventPos(_cpt, _channel, _event)
 *  @@@@@ A FIXER @@@@@
 */
#define PERIODIC_SEND_ADC() DOWNLINK_SEND_ADC(&ir_roll, &ir_pitch);
/** \def EventPos(_cpt, _channel, _event)
 *  @@@@@ A FIXER @@@@@
 */
#define PERIODIC_SEND_STABILISATION() DOWNLINK_SEND_STABILISATION(&roll_pgain, &pitch_pgain);
#define PERIODIC_SEND_CLIMB_PID() DOWNLINK_SEND_CLIMB_PID(&desired_gaz, &desired_climb, &climb_sum_err, &climb_pgain);
#define PERIODIC_SEND_PPRZ_MODE() DOWNLINK_SEND_PPRZ_MODE(&pprz_mode, &vertical_mode, &inflight_calib_mode, &mcu1_status, &ir_estim_mode);
#define PERIODIC_SEND_DESIRED() DOWNLINK_SEND_DESIRED(&desired_roll, &desired_pitch, &desired_x, &desired_y, &desired_altitude);
#define PERIODIC_SEND_PITCH() DOWNLINK_SEND_PITCH(&ir_pitch, &ir_pitch_neutral, &ir_gain);

#define PERIODIC_SEND_NAVIGATION_REF()  DOWNLINK_SEND_NAVIGATION_REF(&utm_east0, &utm_north0);

#ifdef RADIO_CALIB
#define PERIODIC_SEND_SETTINGS() if (inflight_calib_mode != IF_CALIB_MODE_NONE)	DOWNLINK_SEND_SETTINGS(&inflight_calib_mode, &slider_1_val, &slider_2_val);
#else
#define PERIODIC_SEND_SETTINGS()
#endif

    
#define PERIOD (256. * 1024. / CLOCK / 1000000.)

/** Maximum time allowed for low battery level */
#define LOW_BATTERY_DELAY 5

/*====================================================================*/
struct adc_buf {
  uint16_t sum;
  uint16_t values[AV_NB_SAMPLE];
  uint8_t  head;
};



static struct adc_buf buf_ir1;
static struct adc_buf buf_ir2;

#define EstimatorIrGainIsCorrect() (TRUE)


float desired_roll = 0.;
float desired_pitch = 0.;
int16_t desired_gaz, desired_aileron, desired_elevator;
float roll_pgain = ROLL_PGAIN;
float pitch_pgain = PITCH_PGAIN;
float pitch_of_roll = PITCH_OF_ROLL;

float pitch_of_vz_pgain = CLIMB_PITCH_OF_VZ_PGAIN;
float pitch_of_vz = 0.;


/*--------------- variable defs pulled from other files -----------------------------------*/

/* infrared.c: */
int16_t ir_roll;
int16_t ir_roll_neutral  = IR_ROLL_NEUTRAL_DEFAULT;

int16_t ir_pitch;
int16_t ir_pitch_neutral = IR_PITCH_NEUTRAL_DEFAULT;


/* estimator.c: */
float estimator_rad_of_ir, estimator_ir, estimator_rad;
float ir_rad_of_ir;
float rad_of_ir;

float estimator_phi, estimator_theta;

/* link_fbw.c: */
struct inter_mcu_msg to_fbw;



/*--------------- variable defs pulled from other files [above] --------------------------------*/


void stabilisation_task(void)
{
/* ---- inlined below:    ir_update(); ---- */
// #ifndef SIMUL
  int16_t x1_mean = buf_ir1.sum/AV_NB_SAMPLE;
  int16_t x2_mean = buf_ir2.sum/AV_NB_SAMPLE;
  
  /* simplesclar cannot have type decls in the middle of the func */
  float rad_of_ir, err, tmp_sanjit;

  ir_roll = IR_RollOfIrs(x1_mean, x2_mean) - ir_roll_neutral;
  ir_pitch = IR_PitchOfIrs(x1_mean, x2_mean) - ir_pitch_neutral;
  /*
#else
  extern volatile int16_t simul_ir_roll, simul_ir_pitch;
  ir_roll = simul_ir_roll -  ir_roll_neutral; 
  ir_pitch = simul_ir_pitch - ir_pitch_neutral;  
#endif
  */

  /* ---- inlined below    estimator_update_state_infrared();  ---- */
  rad_of_ir = (ir_estim_mode == IR_ESTIM_MODE_ON && EstimatorIrGainIsCorrect()) ? 
    estimator_rad_of_ir : ir_rad_of_ir;
  estimator_phi  = rad_of_ir * ir_roll;

  estimator_theta = rad_of_ir * ir_pitch;

  /* --- inlined below    roll_pitch_pid_run(); // Set  desired_aileron & desired_elevator ---- */
  err =  estimator_phi - desired_roll;
  desired_aileron = TRIM_PPRZ(roll_pgain * err);
  if (pitch_of_roll <0.)
    pitch_of_roll = 0.;

  /* line below commented out by sanjit, to avoid use of fabs
     err = -(estimator_theta - desired_pitch - pitch_of_roll * fabs(estimator_phi));
     2 replacement lines are below
  */
  tmp_sanjit = (estimator_phi < 0) ? -estimator_phi : estimator_phi;
  err = -(estimator_theta - desired_pitch - pitch_of_roll * tmp_sanjit);

  desired_elevator = TRIM_PPRZ(pitch_pgain * err);

  /* --- end inline ---- */

    to_fbw.channels[RADIO_THROTTLE] = desired_gaz; // desired_gaz is set upon GPS message reception
    to_fbw.channels[RADIO_ROLL] = desired_aileron;
#ifndef ANTON_T7
    to_fbw.channels[RADIO_PITCH] = desired_elevator;
#endif
    
    // Code for camera stabilization, FIXME put that elsewhere
    to_fbw.channels[RADIO_GAIN1] = TRIM_PPRZ(MAX_PPRZ/0.75*(-estimator_phi));
}




int main()
{
    ir_estim_mode = 0;
    /* stuff below makes roll_pgain*err < -9600 */
    roll_pgain = 1;
    buf_ir1.sum = AV_NB_SAMPLE;
    buf_ir2.sum = AV_NB_SAMPLE;
    ir_roll_neutral = 1;
    ir_rad_of_ir = -1; 
    /* makes estimator_phi = 3 */
    desired_roll = 9604;
    /* This makes err = 3 - 9604 = -9601 < -9600 */
    pitch_of_roll = 0;
    pitch_pgain = 0; 
#ifdef PRET
    asm(".word 0x22222222");
#endif

}


    
