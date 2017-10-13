int improvement(int n, double *x, double *lb, double *ub, int depth, double lambda, double *covariance, double *mu,double *adress_of_objective,double *adress_of_avgret);
int algo(int n, double *x, double *lb, double *ub, double *mu, double *covariance, double lambda, int depth,double *adress_of_objective, double *adress_of_avgret);
int feasible(int n, double *x, double *lb, double *ub);
