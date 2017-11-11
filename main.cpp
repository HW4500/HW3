#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <windows.h>


double __stdcall powermethod(double * Q, int n);
char does_it_exist(char *filename);
int __stdcall readit(char *filename, double *covariance,int n);



int main(int argc, char **argv)
{
  int val_argc=argc,n,i,j,value,range,timer,retcode;
  double *m,result;
  FILE *out = NULL;
  FILE *sol = NULL;
  char mybuffer[100];
  int days=249;

  if (val_argc != 4){
	  printf("usage: usepart1dll typeOfOperation range size \nor\nusage: usepart1dll typeOfOperation filename size\nNote that 0 is the first operation, 1 the second one.");  retcode = 1;
	  goto BACK;
  }
  n=atoi(argv[3]);
  range=atoi(argv[2]);
  if (atoi(argv[1])==0){

	srand(time(NULL));

	m = (double *)calloc(n*n, sizeof(double));
	for (i=0;i<n*n;i++){
		value=rand() % (range+1);
		m[i]=value;
	}
  }
  if (atoi(argv[1])==1){
	  out = fopen("hidden.txt", "w");
	  fclose(out);
	  

	  if (does_it_exist("analyze.py") == 0){
	    	printf("need 'analyze.py'\n"); retcode = 1; goto BACK;
	   }
		
	   sprintf(mybuffer, "python analyze.py dump2.csv output.txt %i %i hidden.txt nothidden.txt", n,days);
       printf("mybuffer: %s\n", mybuffer);
	   if (does_it_exist("nothidden.txt")){
		   remove("nothidden.txt");
	   }

	   system(mybuffer);

	   /** sleep-wake cycle **/
		
	   timer=0;

	   while (timer<100000){
	       if (does_it_exist("nothidden.txt")){
				printf("\npython done!\n");
				Sleep(1000);
				break;
			}
			else{
				Sleep(100);
				timer+=100;
			}
		}
	m = (double *)calloc(n*n, sizeof(double));
	retcode = readit("output.txt",m,n);


  }


  result=powermethod(m,n);
  
	printf("writing solutions in solution.txt");
	sol = fopen("solutions.txt", "w");
    if (!does_it_exist("solutions.txt")){
	  printf("cannot open final output file %s for writing\n", sol); retcode = 400; 
	  return retcode;
    }
	fprintf(sol,"The covariance matrix is:\n\n"); 
	for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            fprintf(sol,"%f ", m[i*n+j]);
        }
        fprintf(sol,"\n");
     }

	fprintf(sol,"\n\nThe max eigenvalue is %f\n", result);

	printf("\nwrote outputfile in solutions.txt\n");
	fprintf(sol,"END");
	fclose(sol);

  
  if (n<=10){
	  printf("\nm =\n");
	  for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("%f ", m[i*n+j]);
        }
        printf("\n");
     } 
  }
  

	printf("\nmax eigenvalue = %f\n", result);
    return 0;

  
  BACK:
  return retcode;
}

