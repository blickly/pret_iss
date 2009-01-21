// defines values in the CSD register
static unsigned int taac_mantissa_speeds_x10[16]   = { 0, 10, 12, 13, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 80 };
static unsigned int taac_exponent_speeds_div10[8]  = { 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };
static unsigned int r2w_factor_val[8]  = { 1, 2, 4, 8, 16, 32, 0, 0 };
static unsigned int trans_speed_mantisaa_x10[16]   = { 0, 10, 12, 13, 15, 20, 26, 30, 35, 40, 45, 52, 55, 60, 70, 80 };
static unsigned int trans_speed_exponent[8]  = { 100, 1000, 10000, 10000, 0 , 0 , 0 , 0 };
static unsigned int TAAC_speed;
static unsigned int Read_Access_time;
static unsigned int R2W_factor;
static unsigned int Write_Access_time;
static unsigned int Trans_speed;
 
volatile unsigned char CSDregister[16];
volatile unsigned char MMC_CSD_REGISTER_TAAC_MANTISSA;
volatile unsigned char MMC_CSD_REGISTER_TAAC_EXPONENT;
volatile unsigned char MMC_CSD_REGISTER_NSAC;
volatile unsigned char MMC_CSD_REGISTER_R2W_FACTOR;
volatile unsigned char MMC_CSD_REGISTER_TRAN_SPEED_MANTISSA;
volatile unsigned char MMC_CSD_REGISTER_TRAN_SPEED_EXPONENT;

