#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm> 


#include <sys/types.h>
#include <sys/stat.h>

#include <windows.h>


double get_min(double a, double b)
{
	if(a<b)
		return a;
	return b;
}


char does_it_exist(char *filename)
{
	struct stat buf;

	// the function stat returns 0 if the file exists

	if (0 == stat(filename, &buf)){
		return 1;
	}
	else return 0;
}

double sum(double *x, int n){
	double summation = 0;
	for(int i=0;i<n;i++){
		summation += x[i];
	}
	return summation;
}
