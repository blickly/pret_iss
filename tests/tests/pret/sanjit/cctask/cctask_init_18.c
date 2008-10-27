// #include "autopilot.h"
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
uint8_t pprz_mode;
uint8_t vertical_mode = VERTICAL_MODE_MANUAL;
static bool_t low_battery = FALSE;

/* end of stuff from main.c */

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
float desired_climb = 0., pre_climb = 0.;
static const float level_gaz = CLIMB_LEVEL_GAZ;
float climb_sum_err  = 0;

float climb_pitch_pgain = CLIMB_PITCH_PGAIN;
float climb_pitch_igain = CLIMB_PITCH_IGAIN;
float climb_pitch_sum_err = 0.;
float max_pitch = MAX_PITCH;
float min_pitch = MIN_PITCH;


void climb_control_task(void)
{
   if (pprz_mode == PPRZ_MODE_AUTO2 || pprz_mode == PPRZ_MODE_HOME) 
   {
     if (vertical_mode >= VERTICAL_MODE_AUTO_CLIMB) {
       /*        	climb_pid_run();  function inlined below */
       /* inlined climb_pid_run begins */
       
       float err  = estimator_z_dot - desired_climb;
       float fgaz;

       if (auto_pitch) { /* gaz constant */
	 desired_gaz = nav_desired_gaz;
	 desired_pitch = climb_pitch_pgain * (err + climb_pitch_igain * climb_pitch_sum_err);
	 if (desired_pitch > max_pitch)
	   desired_pitch = max_pitch;
	 if (desired_pitch < min_pitch)
	   desired_pitch = min_pitch;
	 climb_pitch_sum_err += err;
	 if (climb_pitch_sum_err > MAX_PITCH_CLIMB_SUM_ERR)
	   climb_pitch_sum_err = MAX_PITCH_CLIMB_SUM_ERR;
	 if (climb_pitch_sum_err < - MAX_PITCH_CLIMB_SUM_ERR)
	   climb_pitch_sum_err = - MAX_PITCH_CLIMB_SUM_ERR; 
       } else { /* pitch almost constant */
	 /* pitch offset for climb */
	 pitch_of_vz = (desired_climb > 0) ? desired_climb * pitch_of_vz_pgain : 0.;
	 fgaz = climb_pgain * (err + climb_igain * climb_sum_err) + CLIMB_LEVEL_GAZ + CLIMB_GAZ_OF_CLIMB*desired_climb;
	 climb_sum_err += err;
	 if (climb_sum_err > MAX_CLIMB_SUM_ERR) climb_sum_err = MAX_CLIMB_SUM_ERR;
	 if (climb_sum_err < - MAX_CLIMB_SUM_ERR) climb_sum_err = - MAX_CLIMB_SUM_ERR;
	 desired_gaz = TRIM_UPPRZ(fgaz * MAX_PPRZ);
	 desired_pitch = nav_pitch + pitch_of_vz;
       }

       /* inlined climb_pid_run ends */
     }

     if (vertical_mode == VERTICAL_MODE_AUTO_GAZ)
       desired_gaz = nav_desired_gaz;

     if (low_battery || (!estimator_flight_time && !launch))
       desired_gaz = 0.;
  }  
}


int main() {

    pprz_mode = 2;
    vertical_mode = 2;
    auto_pitch = 1;
    desired_pitch = max_pitch;
    climb_pitch_sum_err = -101;
    vertical_mode = 0;
    low_battery = 0;
    estimator_flight_time = 1;
#ifdef PRET
    asm(".word 0x22222222");
#endif

}


    
