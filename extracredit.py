import sys
import csv
import os
from math import *
from ctypes import *
so = cdll.LoadLibrary('./libpart1.so') 
print("***flag***")
so.testfunc()

PM = so.powermethod
PM.restype =c_double
'''
m = [6,4,4,5]
cm = (c_double * len(m))(*m) 
print(PM(cm,2))
'''
#To read : here we consider that day 0 is the initial day, therefore if N=1, we give the return for the first day, (p(1)-p(0))/p(0)
#because of that, N cannot go over 249



#Adapt your code so that you can estimate the leading eigenvalue using a rolling horizon of 30 days. 
#That is to say, first you apply the power method to the covariance matrix obtained from days 1-30, 
#then for days 2-31, then 3 - 32, and so on. Part of the task is to quickly update the covariance 
#matrix rather than computing it from scratch. And can you think of a way to jump-start the power method? 
#The computation returns the set of eigenvalue estimates.
def cov(list1, list2):
	l1 = len(list1)
	l2 = len(list2)
	if l1 != l2:
		print("Lists do not have same length")
		return 0
	sum12 = 0
	sum1 = 0
	sum2 = 0
	for i in range(l1):
		sum12 += list1[i] * list2[i]
		sum1 += list1[i]
		sum2 += list2[i]
	return sum12/l1 - (sum1/l1)*(sum2/l1)


if len(sys.argv) != 3:
	sys.exit("usage: analyze.py datafilename number_of_asset\n")

#now reading and memorizing the values of the number of assets or the number of days


l=[]
try:
	numasset=int(sys.argv[2])
	numday=249
except ValueError:
	sys.exit("Cannot read the number of assets or the number of days, must be integers")

if numasset>1000 or numday>=250:
	sys.exit("number of assets or number of days out of range : assets from 1 to 1000, days from 0 to 249")

#now reading and copying the data from the input file
try: 
	datafile= open(sys.argv[1], 'r') 
	reader = csv.reader(datafile)
	for row in reader:
		l.append(row)
	datafile.close
except IOError:
	sys.exit("Cannot open file %s\n" % sys.argv[1])

#now opening the output file to write

	
#calculating average asset returns list and return matrix
return_matrix = [[0 for col in range(numday)] for row in range(numasset)]

for i in range(numasset):
	for j in range(numday+1):
		l[i][j]=float(l[i][j])
	for t in range(numday):
		return_matrix[i][t]+=(l[i][t+1]-l[i][t])/l[i][t]

cov_matrix = [0] * (numasset * numasset)

eigenvalues = []
for i in range(0,249-30):
	#get the cov matrix
	print("calculating eigenvalue no."+str(i))
	if i == 0:
		for k in range(numasset):
			#print("line:"+str(k))
			for j in range(numasset):
				if k > j:
				    cov_matrix[k * numasset + j] = cov_matrix[j * numasset + k]
				else:
					cov_matrix[k * numasset + j] = cov(return_matrix[k][i:i+30],return_matrix[j][i:i+30])
		#for the first time, calculate the cov matrix the 
	else:


		for k in range(numasset):
			#print("line:"+str(k))

			for j in range(numasset):
				if k > j:
					cov_matrix[k * numasset + j] = cov_matrix[j * numasset + k]
				else:
					cov_matrix[k * numasset + j] += (return_matrix[k][i+29]*return_matrix[j][i+29] - return_matrix[k][i-1]*return_matrix[j][i-1])/30
					cov_matrix[k * numasset + j] -= (return_matrix[k][i+29]*return_matrix[j][i+29] - return_matrix[k][i-1]*return_matrix[j][i-1])/900
					cov_matrix[k * numasset + j] -= (return_matrix[k][i+29]-return_matrix[k][i-1]) * sum(return_matrix[k][i:i+29])/900
					cov_matrix[k * numasset + j] -= (return_matrix[j][i+29]-return_matrix[j][i-1]) * sum(return_matrix[j][i:i+29])/900
    	#for the rolling times, we adjust the cov instead of recalculate them


	#use dll to calculate the eigenvalue
	cm = (c_double * len(cov_matrix))(*cov_matrix) 
	ev = PM(cm, numasset)
	print(ev)
	eigenvalues.append(ev)




	
	


