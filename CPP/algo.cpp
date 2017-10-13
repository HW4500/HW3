#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm> 


#include <sys/types.h>
#include <sys/stat.h>

#include <windows.h>

#include "algo.h"
#include "tools.h"

int algo(int n, double *x, double *lb, double *ub, double *mu, double *covariance, double lambda,int depth,double *adress_of_objective,double *adress_of_avgret)
{
	int returncode = 0;
	int returncodef = 0;
	int returncodei = 0;

	printf("\n running algorithm\n");

	returncodef = feasible(n, x, lb, ub);
	printf("The step 1 is done, and the problem is...\n");

	returncodei = improvement(n, x, lb, ub,depth, lambda,covariance, mu,adress_of_objective,adress_of_avgret);


	return returncode;
}

int feasible(int n, double *x, double *lb, double *ub)
{
	int returncode = 0, j;
	double sum, increase;

	printf(" computing first feasible solution\n");

	sum = 0;
	/* set variables to lower bounds */
	for (j = 0; j < n; j++){
		x[j] = lb[j];
		sum += lb[j];
	}
	if(sum>1){
		printf("sorry but the problem is infeasible");
		returncode=1;
	}
	else{
		printf("after first pass, sum = %g\n", sum);
		for (j = 0; j < n; j++){
			increase = 1.0 - sum;
			if (increase > ub[j] - x[j])
				increase = ub[j] - x[j];
			x[j] += increase;
			sum += increase;
			printf("after j = %d, sum = %g\n", j, sum);
			if (sum >= 1.0){
				printf("finito\n");
				break;
			}
		}
		if (sum<1)
		{
			printf("sorry but the problem is infeasible");
			returncode=2;
		}

	}

	return returncode;
}

int improvement(int n, double *x, double *lb, double *ub, int depth,double lambda,double *covariance, double *mu,double *adress_of_objective,double *adress_of_avgret)
{
	int returncode=0;
	int pairs_visited=0;
	int i,j,k;
	double a,b,int_value,epsi,epsiminus,epsiplus,new_obj,avgret;
	double obj=0;
	double delta=1;

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			obj=obj+lambda*covariance[i*n+j]*x[i]*x[j];
		}
		obj=obj-mu[i]*x[i];
	}


	
	while (pairs_visited<depth && delta>1e-010){
		for(i=0;i<n-1;i++){
			for(j=i+1;j<n;j++){
				a=lambda*(covariance[i*n+i]+covariance[j*n+j]-2*covariance[i*n+j]);
				int_value=0;
				for(k=0;k<n;k++){
					if(k!=i&&k!=j){
						int_value+=(covariance[j*n+k]-covariance[i*n+k])*x[k];
					}
				}
				b=2*lambda*(-covariance[i*n+i]*x[i]+covariance[j*n+j]*x[j]+covariance[i*n+j]*(x[i]-x[j])+int_value)+(mu[i]-mu[j]);
				if(a==0){

				}
				else{
					epsi=-b/(2*a);
					epsiminus=-get_min(ub[i]-x[i],x[j]-lb[j]);
					epsiplus=get_min(x[i]-lb[i],ub[j]-x[j]);
					if(epsi<epsiminus){
						epsi=epsiminus;
					}
					else if(epsi>epsiplus){
						epsi=epsiplus;
					}
					x[i]=x[i]-epsi;
					x[j]=x[j]+epsi;
				}
			}
		}
		new_obj=0;
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				new_obj=new_obj+lambda*covariance[i*n+j]*x[i]*x[j];
			}
			new_obj=new_obj-mu[i]*x[i];
		}
		delta=obj-new_obj;
		obj=new_obj;
		new_obj=0;
		pairs_visited+=n*(n-1)/2;
	}
	printf("%d pairs have been corrected\n",pairs_visited);

	*adress_of_objective=obj;
	
	avgret=0;
	for (i=0;i<n;i++){
		avgret+=x[i]*mu[i];
	}
	*adress_of_avgret=avgret;


	return returncode;
}
