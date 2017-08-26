#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double convert2gb(unsigned long int total, unsigned long int val){ //function to convert to giga bytes
	double newval;
	if (total < 0){
		fprintf(stderr, "%s : [ERROR] total is value is : %lu \n", __FUNCTION__, total);
	}
	else if (total == 0)
	{
		newval = val/pow(2,30);
	} else {
		newval = total/pow(2,30) - val/pow(2,30);
	}
	return newval;
}