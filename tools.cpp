#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm> 


#include <sys/types.h>
#include <sys/stat.h>

#include <windows.h>
#include "tools.h"
#include "errors.h"


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






void quickSort( double *a, int l, int r)
{
   int j,i;
   

   if( l < r ) 
   {
   	// divide and conquer
        j = partition( a, l, r);
       quickSort( a, l, j-1);
       quickSort( a, j+1, r);
   }
	
}



int partition( double *a, int l, int r) {
   int pivot, i, j;
   double q;
   pivot = a[l];
   i = l; j = r+1;
		
   while( 1)
   {
   	do ++i; while( a[i] <= pivot && i <= r );
   	do --j; while( a[j] > pivot );
   	if( i >= j ) break;
   	q = a[i]; a[i] = a[j]; a[j] = q;

	
   }
   q = a[l]; a[l] = a[j]; a[j] = q;
   return j;
}