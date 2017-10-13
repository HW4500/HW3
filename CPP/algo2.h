int algo2(int n, double *x, double *lb, double *ub, double *mu, double *covariance, double lambda, int depth,double *adress_of_objective, double *adress_of_avgret);
int F(double lamda, int n, double *covariance, double *mu, double *x,double *address_of_oldF);
int find_y(double *x, int n, double lamda, double *lb, double *ub, double *mu, double *covariance,double **address_of_y);
int find_s(int n, double lamda, double *x, double *y, double * mu, double *covariance,double *address_of_s);
int find_biggest_m(double *array, int n, int m, double *address_of_breakpoint);
