#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm> 


#include <sys/types.h>
#include <sys/stat.h>

#include <windows.h>

int readit(char *nameoffile, int *addressofn, double **, double **, double **, double *, double **);

int algo(int n, double *x, double *lb, double *ub, double *mu, double *covariance, double lambda, int depth);

int feasible(int n, double *x, double *lb, double *ub);

int improvement(int n, double *x, double *lb, double *ub, int depth, double lambda, double *covariance, double *mu);

double get_min(double a, double b);

char does_it_exist(char *filename);

int main(int argc, char **argv)
{
  int retcode = 0;
  char mybuffer[100];
  int n;
  int depth=1000;
  double *lb, *ub, *covariance, *mu, lambda, *x,time;
  char file[100];
  FILE *out = NULL;

  if (argc != 2 && argc !=3){
	  printf("usage: qp1 filename\nor\nusage: qp1 nb_asset nb_day");  retcode = 1;
	  goto BACK;
  }

  if (argc==3){
	    out = fopen("hidden.txt", "w");
		fclose(out);

		if (does_it_exist("analyze.py") == 0){
			printf("need 'analyze.py'\n"); retcode = 1; goto BACK;
		}

		sprintf(mybuffer, "python analyze.py dump2.csv output.txt %s %s hidden.txt nothidden.txt", argv[1], argv[2]);

		printf("mybuffer: %s\n", mybuffer);

		if (does_it_exist("nothidden.txt")){
			remove("nothidden.txt");
		}

		system(mybuffer);

		/** sleep-wake cycle **/
		
		time=0;
		while (time<100000){
			if (does_it_exist("nothidden.txt")){
				printf("\npython done!\n");
				Sleep(1000);
				break;
			}
			else{
				Sleep(100);
				time+=100;
			}
		}
		sprintf(file, "output.txt");

  }
  else{
	  sprintf(file,argv[2]);
  }

  retcode = readit(file, &n, &lb, &ub, &mu, &lambda, &covariance);
  if (retcode) goto BACK;

  x = (double *)calloc(n, sizeof(double));
  if (x == NULL){
	  printf(" no memory for x\n"); retcode = 1; goto BACK;
  }

  retcode = algo(n, x, lb, ub, mu, covariance, lambda,depth); 
  BACK:
  return retcode;
}

int readit(char *filename, int *address_of_n, double **plb, double **pub,
		double **pmu, double *plambda, double **pcovariance)
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
	*plambda = atof(buffer);

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


int algo(int n, double *x, double *lb, double *ub, double *mu, double *covariance, double lambda,int depth)
{
	int returncode = 0;
	int returncodef = 0;
	int returncodei = 0;

	printf("\n running algorithm\n");

	returncodef = feasible(n, x, lb, ub);
	printf("The step 1 is done, and the problem is...\n");

	returncodei = improvement(n, x, lb, ub,depth, lambda,covariance, mu);


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

int improvement(int n, double *x, double *lb, double *ub, int depth,double lambda,double *covariance, double *mu)
{
	int returncode=0;
	int pairs_visited=0;
	int i,j,k;
	double a,b,int_value,epsi,epsiminus,epsiplus,new_obj;
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
	printf("%d pairs have been corrected",pairs_visited);



	return returncode;
}



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
