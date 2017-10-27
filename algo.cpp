#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm> 


#include <sys/types.h>
#include <sys/stat.h>

#include <windows.h>

#include "algo.h"
#include "tools.h"
#include "errors.h"


int algo(struct bag *pmybag)
{
	int returncode = 0;
	int retcode1 = 0;
	int retcode21 = 0;
	int retcode22 = 0;
	int retcode23=0;
	int retcode3=0;
	int retcode4=0;
    double s,oldF, newF,optimal,avgret;
    oldF = 0;
    newF = -1;

	printf("\n running algorithm\n");

	retcode1 = feasible(pmybag);
	if (retcode1) goto BACK;
	printf("The step 1 is done\n");

	
    while(newF - oldF < -0.0000001){
		pmybag->address_of_F=&oldF;
  	    retcode21 = F(pmybag);

  	    printf ("Optimizing...Now Value = %f\n", oldF);
        retcode3 = find_y(pmybag);
		if(retcode3){
			returncode=11;
			return returncode;
		}

	    pmybag->address_of_s=&s;
        retcode4 = find_s(pmybag);
        for(int j=0;j<pmybag->n;j++){
      	  pmybag->x[j] = pmybag->x[j] + s * pmybag->y[j]; 
        }
		pmybag->address_of_F=&newF;
        retcode22 = F(pmybag);
    }
	pmybag->address_of_F=&optimal;
	retcode23=F(pmybag);
    printf ("Hello! optimal value = %f\nportfolio:\n",optimal );

	avgret=0;
    for(int l=0;l<pmybag->n;l++){
		avgret+=pmybag->x[l]*pmybag->mu[l];
		//printf("%f\n",x[l]);
	}
	*pmybag->address_of_avgret=avgret;
    *pmybag->address_of_objective=optimal; 

    BACK:
    return returncode;
	
}

int find_m_biggest(double* array, int n, int m,double *address_of_breakpoint){
	int returncode=0;
		
	//create a copy of gradient
	double * copy = NULL;
	copy = (double *)calloc(n, sizeof(double));
	if (!copy){
		returncode = NOMEMORY;
	}
	for(int i=0;i<n;i++){
		copy[i]=array[i];
	}
	//sort the copy
	quickSort(copy, 0, n);
	*address_of_breakpoint=copy[n-m];

	
	return returncode;
	
}
//double improve(double *x, int n, double lamda);
int find_s(struct bag *pmybag){
	double s0 = 0, sigma1 = 0, sigma2 = 0;
	int returncode=0;
	int n = pmybag->n;
	for(int i=0;i<n;i++){
        sigma1 += pmybag->mu[i] * pmybag->y[i];
        sigma1 -= 2 * pmybag->lambda * pmybag->covariance[i*n+i] * pmybag->x[i] * pmybag->y[i];
        sigma2 += 2 * pmybag->lambda * pmybag->covariance[i*n+i] * pmybag->y[i] * pmybag->y[i];
        for(int j=i+1;j<n;j++){
            sigma1 -= 2 * pmybag->lambda * (pmybag->x[i] * pmybag->y[j] + pmybag->x[j] * pmybag->y[i]) * pmybag->covariance[i*n+j];
        	sigma2 += 4 * pmybag->lambda * pmybag->y[i] * pmybag->y[j] * pmybag->covariance[i*n+j];
        }
    }
    s0 = sigma1/sigma2;//optimal point
    if(s0 > 1){
		*pmybag->address_of_s=1;
		return returncode;
	}
    else if(s0 < 0){
		*pmybag->address_of_s=0;
		return returncode;
	}
	else{
		*pmybag->address_of_s=s0;
		return returncode;
	}
}
int find_y(struct bag *pmybag){
	double * g, *lb_y, *ub_y, test1=0,test2=0;
	int returncode=0;

	g = (double *)calloc(pmybag->n, sizeof(double));
	lb_y = (double *)calloc(pmybag->n, sizeof(double));
	ub_y = (double *)calloc(pmybag->n, sizeof(double));
	pmybag->y = (double *)calloc(pmybag->n, sizeof(double));
	for(int k=0;k<pmybag->n;k++){
		lb_y[k] = pmybag->lb[k] - pmybag->x[k];
		test1 += lb_y[k];
		ub_y[k] = pmybag->ub[k] - pmybag->x[k];
		test2 += ub_y[k];
		g[k] = -pmybag->mu[k];
		for(int s=0; s<pmybag->n;s++){
			g[k] += 2 * pmybag->lambda * pmybag->covariance[k*pmybag->n+s] * pmybag->x[s];

		}
	}
	if(test1>=0 || test2<=0){
		printf("boundary wrong, test1=%f, test2=%f",test1,test2);
	}
    for(int m=0;m<pmybag->n;m++){
    	double breakpoint;
		int retcodef=0;
    	retcodef = find_m_biggest(g, pmybag->n, m,&breakpoint);
    	double temp_sum = 0;
    	int temp_flag = 0;
    	for(int i=0;i<pmybag->n;i++){
    		if(g[i] > breakpoint + 0.0000001){
    			pmybag->y[i] = lb_y[i];
    			temp_sum += pmybag->y[i];
    		}
    		else if(g[i] < breakpoint - 0.0000001){
    			pmybag->y[i] = ub_y[i];
    			temp_sum += pmybag->y[i];
    		}
    		else{
    			temp_flag = i; // the ith number in g is the mth biggest and its y value not on boundary!
    		}
    	}
    	//then lets test whether the ith y value is legal!
    	if(-temp_sum < ub_y[temp_flag] && -temp_sum > lb_y[temp_flag]){
    		//legal! we found y!
    		pmybag->y[temp_flag] = -temp_sum;
    		return returncode; 
    	}
		
    }
	printf("no y found\n");
	returncode=1;
	return returncode;
}


int F(struct bag *pmybag)
{
	int retcode=0;
    double sum = 0;
	int n = pmybag->n;
    for(int i=0;i<n;i++){
        sum += pmybag->lambda * pmybag->x[i] * pmybag->x[i] * pmybag->covariance[i*n+i];
        sum -= pmybag->mu[i] * pmybag->x[i];
        for(int j=i+1;j<n;j++){
            sum += 2 * pmybag->lambda * pmybag->x[i] * pmybag->x[j] * pmybag->covariance[i*n+j];
        }
    }
    *pmybag->address_of_F=sum;
	return retcode;
}

int feasible(struct bag *pmybag)
{
	int returncode = 0, j;
	double sum, increase;

	printf(" computing first feasible solution\n");

	sum = 0;
	/* set variables to lower bounds */
	for (j = 0; j < pmybag->n; j++){
		pmybag->x[j] = pmybag->lb[j];
		sum += pmybag->lb[j];
	}
	if(sum>1){
		printf("sorry but the problem is infeasible");
		returncode=1;
	}
	else{
		printf("after first pass, sum = %g\n", sum);
		for (j = 0; j < pmybag->n; j++){
			increase = 1.0 - sum;
			if (increase > pmybag->ub[j] - pmybag->x[j])
				increase = pmybag->ub[j] - pmybag->x[j];
			pmybag->x[j] += increase;
			sum += increase;
			printf("after j = %d, sum = %g\n", j, sum);
			if (sum >= 1.0){
				printf("Done\n");
				break;
			}
		}
		if (sum<1)
		{
			printf("Sorry but the problem is infeasible");
			returncode=2;
		}

	}

	return returncode;
}
