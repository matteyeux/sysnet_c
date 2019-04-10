#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// function to convert to giga bytes.
double convert2gb(unsigned long int total, unsigned long int val)
{
	double newval = 0;
	if (total == 0) {
		newval = val/pow(2,30);
	} else {
		newval = total/pow(2,30) - val/pow(2,30);
	}

	return newval;
}


int convert2percent (unsigned long val, unsigned long total)
{
	return (val * 100) / total;
}