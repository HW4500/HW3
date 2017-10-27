#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm> 


#include <sys/types.h>
#include <sys/stat.h>

#include <windows.h>

#include "algo.h"
#include "read_write.h"
#include "tools.h"
#include "errors.h"




int main(int argc, char **argv)
{
  int retcode = 0;
  char mybuffer[100];
  int n;
  double *lb, *ub, *covariance, *mu, lambda, *x,time,objective,avgret;
  char file[100];
  int val_argc,depth;
  FILE *out = NULL;
  struct bag *pmybag = NULL;

  val_argc=argc;
  if (val_argc != 2 && val_argc !=3){
	  printf("usage: qp1 filename\nor\nusage: qp1 nb_asset nb_day");  retcode = 1;
	  goto BACK;
  }

  if (val_argc==3){
	    out = fopen("hidden.txt", "w");
		fclose(out);

		if (does_it_exist("analyze2.py") == 0){
			printf("need 'analyze2.py'\n"); retcode = 1; goto BACK;
		}
		
		sprintf(mybuffer, "python analyze2.py dump2.csv output.txt %s %s hidden.txt nothidden.txt", argv[1], argv[2]);
		
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
	  sprintf(file,argv[1]);
  }

  retcode = readit(file, &n, &lb, &ub, &mu, &lambda, &covariance);
  if (retcode) goto BACK;


  x = (double *)calloc(n, sizeof(double));
  if (x == NULL){
	  printf(" no memory for x\n"); retcode = 1; goto BACK;
  }

	pmybag = (struct bag *)calloc(1, sizeof(struct bag));
	if (!pmybag){
		retcode = NOMEMORY; goto BACK;
	}
  
    pmybag->n = n;
	pmybag->covariance = covariance;
	pmybag->lb = lb;
	pmybag->ub = ub;
	pmybag->mu = mu;
	pmybag->x = x;
	pmybag->lambda=lambda;
	pmybag->address_of_objective=&pmybag->objective;
	pmybag->address_of_avgret=&pmybag->avgret;
  
  
  retcode = algo(pmybag); 
  if(retcode){
	  printf("Error, the program returned the code %i\n",retcode);
	  goto BACK;
  }

  retcode = presentation(pmybag);
  if(val_argc==3){
	sprintf(mybuffer, "python presentator.py output.txt final_sol.txt");

	printf("mybuffer: %s\n", mybuffer);

	system(mybuffer);
  }
  else{
	  sprintf(mybuffer, "python presentator.py %s final_sol.txt",argv[1]);
	  
	  printf("mybuffer: %s\n", mybuffer);

	  system(mybuffer);
  }

  BACK:
  return retcode;
}






