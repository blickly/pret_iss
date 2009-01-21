/* this file is used to cause delays using the DEAD function
	Based on the value of Frequency_MMC as LOW or high it uses 
	the required delay function.
	Value of Dead has been calculated to give a frequency of
	400KHz or 20 MHz. Based on the value of Frequency_MMC being 
	LOW or HIGH.
	
	Created by Devesh Dedhia
*/

#include "globals.h"
#include "deadline.h"

void delay(){
	if(Frequency_MMC==LOW){
		DEAD(864);
		DEAD(0);
	}
	else {
		DEAD(17);
		DEAD(0);
	}		
}
