#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm> 


#include <sys/types.h>
#include <sys/stat.h>

#include <windows.h>

#include "algo.h"
#include "algo2.h"
#include "tools.h"


int algo2(int n, double *x, double *lb, double *ub, double *mu, double *covariance, double lambda,int depth,double *address_of_objective,double *address_of_avgret)
{
	int returncode = 0;
	int retcode1 = 0;
	int retcode21 = 0;
	int retcode22 = 0;
	int retcode23=0;
	int retcode3=0;
	int retcode4=0;
    double *y;
    double s,oldF, newF,optimal,avgret;
    oldF = 0;
    newF = -1;

	printf("\n running algorithm\n");

	retcode1 = feasible(n, x, lb, ub);
	printf("The step 1 is done, and the problem is...\n");


    while(newF - oldF < -0.0000001){
  	    retcode21 = F(lambda, n, covariance, mu, x,&oldF);

  	    printf ("Optimizing...Now Value = %f\n", oldF);
        retcode3 = find_y(x, n, lambda, lb, ub, mu, covariance,&y);
		if(retcode3){
			returncode=11;
			return returncode;
		}
      //  for(int l=0;l<n;l++){printf("%f\n",y[l]);}

        retcode4 = find_s(n, lambda, x, y, mu, covariance,&s);
        for(int j=0;j<n;j++){
      	  x[j] = x[j] + s * y[j]; 
        }
        retcode22 = F(lambda, n, covariance, mu, x,&newF);
		system("pause");
		printf("%f,%f",oldF,newF);
    }
	retcode23=F(lambda, n, covariance, mu, x,&optimal);
    printf ("Hello! optimal value = %f\nportfolio:\n",optimal );

	avgret=0;
    for(int l=0;l<n;l++){
		avgret+=x[l]*mu[l];
		printf("%f\n",x[l]);
	}
	*address_of_avgret=avgret;
    *address_of_objective=optimal; 

    BACK:
    return returncode;
	
}

int find_m_biggest(double* array, int n, int m,double *address_of_breakpoint){
	int returncode=0;
	for(int i=0;i<n;i++){
		int count = 0;
		for(int j=0;j<n;j++){
			if(array[j] > array[i])
				count++;
			else
				continue; 
		}
		if(count == m){
			*address_of_breakpoint=array[m];
			return returncode;
		}
	}
}
//double improve(double *x, int n, double lamda);
int find_s(int n, double lambda, double *x, double *y, double * mu, double *covariance,double *address_of_s){
	double s0 = 0, sigma1 = 0, sigma2 = 0;
	int returncode=0;
	for(int i=0;i<n;i++){
        sigma1 += mu[i] * y[i];
        sigma1 -= 2 * lambda * covariance[i*n+i] * x[i] * y[i];
        sigma2 += 2 * lambda * covariance[i*n+i] * y[i] * y[i];
        for(int j=i+1;j<n;j++){
            sigma1 -= 2 * lambda * (x[i] * y[j] + x[j] * y[i]) * covariance[i*n+j];
        	sigma2 += 4 * lambda * y[i] * y[j] * covariance[i*n+j];
        }
    }
    s0 = sigma1/sigma2;
    if(s0 > 1){
		*address_of_s=1;
		return returncode;
	}
    else if(s0 < 0){
		*address_of_s=0;
		return returncode;
	}
	else{
		*address_of_s=s0;
		return returncode;
	}
}
int find_y(double *x, int n, double lambda, double *lb, double *ub, double *mu, double *covariance,double **address_of_y){
	double * g, *lb_y, *ub_y, *y;
	int returncode=0;

	g = (double *)calloc(n, sizeof(double));
	lb_y = (double *)calloc(n, sizeof(double));
	ub_y = (double *)calloc(n, sizeof(double));
	y = (double *)calloc(n, sizeof(double));
	for(int k=0;k<n;k++){
		lb_y[k] = lb[k] - x[k];
		ub_y[k] = ub[k] - x[k];
		g[k] = -mu[k];
		for(int s=0; s<n;s++){
			g[k] += 2 * lambda * covariance[k*n+s] * x[s];
		}
	}
    for(int m=0;m<n;m++){
    	double breakpoint;
		int retcodef=0;
    	retcodef = find_m_biggest(g, n, m,&breakpoint);
    	double temp_sum = 0;
    	int temp_flag = 0;
    	for(int i=0;i<n;i++){
    		if(g[i] > breakpoint + 0.0000001){
    			y[i] = lb_y[i];
    			temp_sum += y[i];
    		}
    		else if(g[i] < breakpoint - 0.0000001){
    			y[i] = ub_y[i];
    			temp_sum += y[i];
    		}
    		else{
    			temp_flag = i; // the ith number in g is the mth biggest and its y value not on boundary!
    		}
    	}
    	//then lets test whether the ith y value is legal!
    	if(-temp_sum < ub_y[temp_flag] && -temp_sum > lb_y[temp_flag]){
    		//legal! we found y!
    		y[temp_flag] = -temp_sum;
			*address_of_y=y;
    		return returncode; 
    	}
    }
	printf("no y found\n");
	returncode=1;
	return returncode;
}


int F(double lambda, int n, double *covariance, double *mu, double *x, double *address_of_oldF)
{
	int retcode=0;
    double sum = 0;
    for(int i=0;i<n;i++){
        sum += lambda * x[i] * x[i] * covariance[i*n+i];
        sum -= mu[i] * x[i];
        for(int j=i+1;j<n;j++){
            sum += 2 * lambda * x[i] * x[j] * covariance[i*n+j];
        }
    }
    *address_of_oldF=sum;
	return retcode;
}
