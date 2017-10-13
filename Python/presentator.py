import sys
from math import *
import matplotlib.pyplot as plt
#To read : here we consider that day 0 is the initial day, therefore if N=1, we give the return for the first day, (p(1)-p(0))/p(0)
#because of that, N cannot go over 249


if len(sys.argv) != 3:
	sys.exit("usage: analyze.py datafilename1 datafilename2   \n")


#now reading and copying the data from the input file
try: 
	datafile1= open(sys.argv[1], 'r') 
except IOError:
	sys.exit("Cannot open file %s\n" % sys.argv[1])

try: 
	datafile2= open(sys.argv[2], 'r') 
except IOError:
	sys.exit("Cannot open file %s\n" % sys.argv[2])

	
	
	
	
	
lines1 = datafile1.readlines();
datafile1.close()
lines2 = datafile2.readlines();
datafile2.close()

#print(lines1[0])
firstline1 = lines1[0].split()
n=int(firstline1[1])
#print("first line is", firstline1)
#print(lines2[0])
firstline2 = lines2[0].split()
obj=float(firstline2[12])
#print("first line is", firstline2)

list_mu=[]
for i in range(4,n+4):
	thisline1=lines1[i].split()
	mu=float(thisline1[3])
	list_mu.append(mu)

list_vol=[]
covariance_matrix=[]
for i in range(n+9,2*n+9):
	thisline1=lines1[i].split()
	for j in range(0,n):
		if(i-n-9==j):
			vol=sqrt(float(thisline1[i-n-9]))
			list_vol.append(vol)
		covariance_matrix.append((float(thisline1[j])))
	

list_solutions=[]
list_percentages=[0]*n
port_ret=0
port_vol=0
i=2
thisline2=lines2[i].split()
while(thisline2[0]!='END'):
	index=int(thisline2[3][1:])
	percentage=float(thisline2[9][:-1])/100
	list_solutions.append(index)
	list_percentages[index]=percentage
	port_ret=port_ret+list_mu[index]*percentage
	i+=1
	thisline2=lines2[i].split()


	

for i in range(n):
	for j in range(n):
		port_vol+=covariance_matrix[i*n+j]*list_percentages[i]*list_percentages[j]
port_vol=sqrt(port_vol)

	
	
	
plt.figure()
abscissa_notsol=[]
abscissa_sol=[]
ordinate_notsol=[]
ordinate_sol=[]
for i in range(n):
	if i in list_solutions:
		abscissa_sol.append(list_vol[i])
		ordinate_sol.append(list_mu[i])
	else:
		abscissa_notsol.append(list_vol[i])
		ordinate_notsol.append(list_mu[i])


plt.plot(abscissa_notsol,ordinate_notsol,'rx',label='not in the portfolio')
plt.plot(abscissa_sol,ordinate_sol,'bx',label='in the portfolio')
plt.plot(port_vol,port_ret,'gx',label='portfolio')
plt.xlabel('volatility')
plt.ylabel('average rate of return')
plt.title('Plot of the average of return as a function of the volatility for each asset')
plt.savefig('plot.png',bbox_incher='tight')

print("ploted the solution in 'plot.png'\n")

