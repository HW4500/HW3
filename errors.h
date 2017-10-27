#define NOMEMORY 100

typedef struct bag{
	int n;
	double *x;
	double *lb;
	double *ub;
	double *mu;
	double *covariance;
	double *gradient;
	double *copyofgradient;
	int *sortindex;
	int depth;
	double *address_of_objective;
	double objective;
	double *address_of_avgret;
	double avgret;
	double lambda;
	int num_iter;
	int iter_counter;
	FILE *output;
	double *address_of_F;
	double *address_of_s;
	double *y;
};