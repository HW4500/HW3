#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

#include <sys/types.h>
#include <sys/stat.h>

int find_max_index(double * vec, int n){
	int index = 0;
	double max = fabs(vec[0]);
	for (int i=0;i<n;i++){
		if(fabs(vec[i])>max){
			index = i;
			max = fabs(vec[i]);
		}
	}
    return index;
}

double * normalize(double * vec, int n){
	double * returnvec; 
	returnvec = (double *)calloc(n, sizeof(double));
	double max = fabs(vec[0]);
	for (int i=0;i<n;i++){
		if(fabs(vec[i])>max){
			max = fabs(vec[i]);
		}
	}
	for (int i =0;i<n;i++){
		returnvec[i] = vec[i]/max;
	}
	return returnvec;
}

double distance(double * vec1, double * vec2, int n){
	double sum = 0;
	for(int i=0;i<n;i++){
		sum += fabs(vec1[i] - vec2[i]);
	}
	return sum;
}

double * Qxw(double * mat, double * vec, int n){
	double * returnvec; 
	double temp_sum = 0; 
	returnvec = (double *)calloc(n, sizeof(double));
	for(int i=0;i<n;i++){
		temp_sum = 0;
		for(int j=0;j<n;j++){
			temp_sum += mat[n*i+j]*vec[j];
 		}
	    returnvec[i] = temp_sum;
	}
	return returnvec;

}

double __stdcall powermethod(double * Q, int n){
	//get a random vector
	double * w, * w0;
	w = (double *)calloc(n, sizeof(double));
	w0 = (double *)calloc(n, sizeof(double));
	for(int i=0;i<n;i++){
		w[i] = 100*rand()/(double)RAND_MAX;
		w0[i] = w[i];
	}
	double largest_eigenv;
	while(distance(normalize(Qxw(Q, w, n), n), w, n) > 0.00001){
		w = normalize(Qxw(Q, w, n), n);
	}
	int ind = find_max_index(w, n);
	largest_eigenv = Qxw(Q, w, n)[ind]/w[ind];
	return largest_eigenv;
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

int __stdcall readit(char *filename, double *covariance,int n)
{
	int readcode = 0, fscancode;
	FILE *datafile = NULL;
	char buffer[100];
	int i, j;

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



	if (!covariance){
		printf("not enough memory for covariance\n"); readcode = 3; goto BACK;
	}

	fscanf(datafile, "%s", buffer); /* reading 'covariance'*/

	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){ 
			fscanf(datafile, "%s", buffer);
			covariance[i*n + j] = atof(buffer);
		}
	}


	fscanf(datafile, "%s", buffer);
	if (strcmp(buffer, "END") != 0){
		printf("possible error in data file: 'END' missing\n%s",buffer);
	}

	fclose(datafile);

BACK:

	return readcode;
}

