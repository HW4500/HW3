# HW3
The file analyse2.py is the one taking as an entry the dataset (csv file) and having as an output a text file containing the same info as example.txt

the cpp code calls analyse2.py or works on a dataset, depending on the command. Then computes the gradient descent. then calls presentator.py for a graph. Messy, but works for the example (tested it versus Matlab) and now seems to work for any file (tested Marin versus Kecheng). To do: work on the final output file, clean it, comment it, make more protections (if file note of the good type, return error), use the returncodes to make it easy to read, test it on Matlab

presentator.py plots the results

To make the code work:

1) create a void project on visual basics
2) add to this project the cpp files in "source"
3) add the h files (headers) in "header"
4) build the project
5) in the debug file (where you can find project.exe), put analyze.py, dump2.csv, example.txt and presentator.py
6) Call the .exe via the command line. Do be suprised if the result is not immediate, it takes for now 2 minutes to have a result for K=1000, N=250

To launch Kecheng's code, replace in the main "retcode = algo(n, x, lb, ub, mu, covariance, lambda,depth,&objective,&avgret);" 
by "retcode = algo2(n, x, lb, ub, mu, covariance, lambda,depth,&objective,&avgret);" and vice versa.

