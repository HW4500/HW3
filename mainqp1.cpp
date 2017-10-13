#include <stdio.h>
#include <stdlib.h>
#include <string.h>
double sum(double *x, int n){
	double summation = 0;
	for(int i=0;i<n;i++){
		summation += x[i];
	}
	return summation;
}
int readit(char *nameoffile, int *addressofn, double *addressoflambda, double **, double **, double **, double **);
double F(double lamda, int n, double *covariance, double *mu, double *x);
int find_feasible_solution(double *x, int n, double *lb, double *ub);
double * find_y(double *x, int n, double lamda, double *lb, double *ub, double *mu, double *covariance);
double find_s(int n, double lamda, double *x, double *y, double * mu, double *covariance);




int main(int argc, char **argv)
{
  int retcode = 0;
  int n;
  double *lb, *ub, *covariance, *mu, lambda;
  double *solution;

  if (argc != 2){
	  printf("usage: qp1 filename\n");  retcode = 1;
	  goto BACK;
  }

  retcode = readit(argv[1], &n, &lambda, &lb, &ub, &mu, &covariance);
  double *x;
  x = (double *)calloc(n, sizeof(double));

  find_feasible_solution(x, n, lb, ub);
  double *y;
  double s;
  double oldF, newF;
  oldF = 0;
  newF = -1;
  for(int i=0;newF - oldF < -0.0001;i++){
  	  oldF = F(lambda, n, covariance, mu, x);
  	  printf ("Optimizing...Now Value = %f\n", oldF);
      y= find_y(x, n, lambda, lb, ub, mu, covariance);
      s = find_s(n, lambda, x, y, mu, covariance);
      for(int j=0;j<n;j++){
      	x[j] = x[j] + s * y[j]; 
      }
      newF = F(lambda, n, covariance, mu, x);
  }

  printf ("Hello! optimal value = %f\n", F(lambda, n, covariance, mu, x));
    
  BACK:
  return retcode;
}
double find_m_biggest(double* array, int n, int m){
	for(int i=0;i<n;i++){
		int count = 0;
		for(int j=0;j<n;j++){
			if(array[j] > array[i]){count++;}
			else{continue;} 
		}
		if(count == m){
			return array[m];
		}
	}
}
//double improve(double *x, int n, double lamda);
double find_s(int n, double lambda, double *x, double *y, double * mu, double *covariance){
	double s0 = 0, sigma1 = 0, sigma2 = 0;
	for(int i=0;i<n;i++){
        sigma1 += mu[i] * y[i];
        sigma1 -= 2 * lambda * covariance[i*n+i] * x[i] * y[i];
        sigma2 += 2 * lambda * covariance[i*n+i] * y[i] * y[i];
        for(int j=i+1;j<n;j++){
            sigma1 -= 2 * (x[i] * y[j] + x[j] * y[i]) * covariance[i*n+j];
        	sigma2 += 4 * y[i] * y[j] * covariance[i*n+j];
        }
    }
    s0 = sigma1/sigma2;
    if(s0 > 1){return 1;}
    else if(s0 < 0){return 0;}
	else{return s0;}
}
double * find_y(double *x, int n, double lambda, double *lb, double *ub, double *mu, double *covariance){
	double * g, *lb_y, *ub_y, *y;
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
    	breakpoint = find_m_biggest(g, n, m);
    	double temp_sum = 0;
    	int temp_flag = 0;
    	for(int i=0;i<n;i++){
    		if(g[i] > breakpoint + 0.0001){
    			y[i] = lb_y[i];
    			temp_sum += y[i];
    		}
    		else if(g[i] < breakpoint - 0.0001){
    			y[i] = ub_y[i];
    			temp_sum += y[i];
    		}
    		else{
    			temp_flag = i; // the ith number in g is the mth biggest and its y value not on boundary!
    			continue;
    		}
    	}
    	//then lets test whether the ith y value is legal!
    	if(-temp_sum < ub_y[temp_flag] && -temp_sum > lb_y[temp_flag]){
    		//legal! we found y!
    		y[temp_flag] = -temp_sum;
    		return y; 
    	}
    	else{
    		//not legal! try another m!
    		continue;
    	}
    }
}

int find_feasible_solution(double *x, int n, double *lb, double *ub){
	double sum_of_lb;
	for(int i=0;i<n;i++){
		sum_of_lb += lb[i];
		x[i] = lb[i];
	}
	if(sum_of_lb > 1 + 0.0001){
		printf("No feasible solution!!\n");
		return 0;
	}
	else if(sum_of_lb > 1 - 0.0001){
		printf("Find feasible solution!!\n");
		return 1;
	}
    else{
    	for(int i=0;i<n;i++){
    		if(sum(x,n) - x[i] + ub[i] > 1+0.0001){
    			x[i] = 1 - sum(x,n) - x[i];
		        printf("Find feasible solution!!\n");
		        return 1;
    		}
    		else{
    			x[i] = ub[i];
    			printf("Still searching...\n");
    		}
    	}
    }
}

double F(double lambda, int n, double *covariance, double *mu, double *x)
{
    double sum = 0;
    for(int i=0;i<n;i++){
        sum += lambda * x[i] * x[i] * covariance[i*n+i];
        sum -= mu[i] * x[i];
        for(int j=i+1;j<n;j++){
            sum += 2 * lambda * x[i] * x[j] * covariance[i*n+j];
        }
    }
    return sum;
}
int readit(char *filename, int *address_of_n, double *address_of_lambda, double **plb, double **pub,
		double **pmu, double **pcovariance)
{
	int readcode = 0, fscancode;
	FILE *datafile = NULL;
	char buffer[100];
	int n, i, j;
	double *lb = NULL, *ub = NULL, *mu = NULL, *covariance = NULL;

	datafile = fopen(filename, "r");
	if (!datafile){
		printf("cannot open file %s\n", filename);
		readcode = 2;  goto BACK;
	}

	printf("reading data file %s\n", filename);

	fscanf(datafile, "%s", buffer);
	fscancode = fscanf(datafile, "%s", buffer);
	if (fscancode == EOF){
		printf("problem: premature file end at ...\n");
		readcode = 4; goto BACK;
	}

	n = *address_of_n = atoi(buffer);

	printf("n = %d\n", n);

	lb = (double *)calloc(n, sizeof(double));
	*plb = lb;
	ub = (double *)calloc(n, sizeof(double));
	*pub = ub;
	mu = (double *)calloc(n, sizeof(double));
	*pmu = mu;
	covariance = (double *)calloc(n*n, sizeof(double));
	*pcovariance = covariance;

	if (!lb || !ub || !mu || !covariance){
		printf("not enough memory for lb ub mu covariance\n"); readcode = 3; goto BACK;
	}

	fscanf(datafile, "%s", buffer);
	for (j = 0; j < n; j++){
		fscanf(datafile, "%s", buffer);
		fscanf(datafile, "%s", buffer);
		lb[j] = atof(buffer);
		fscanf(datafile, "%s", buffer);
		ub[j] = atof(buffer);
		fscanf(datafile, "%s", buffer);
		mu[j] = atof(buffer);
		printf("j = %d lb = %g ub = %g mu = %g\n", j, lb[j], ub[j], mu[j]);
	}

	fscanf(datafile, "%s", buffer);

	fscanf(datafile, "%s", buffer);
	*address_of_lambda = atof(buffer);
   
	fscanf(datafile, "%s", buffer); /* reading 'covariance'*/

	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){ 
			fscanf(datafile, "%s", buffer);
			covariance[i*n + j] = atof(buffer);
		}
	}


	fscanf(datafile, "%s", buffer);

	if (strcmp(buffer, "END") != 0){
		printf("possible error in data file: 'END' missing\n");
	}


	fclose(datafile);

BACK:

	return readcode;
}
