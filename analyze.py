import sys
import csv
import os
#To read : here we consider that day 0 is the initial day, therefore if N=1, we give the return for the first day, (p(1)-p(0))/p(0)
#because of that, N cannot go over 249

if len(sys.argv) != 7:
	sys.exit("usage: analyze.py datafilename outputfilename number_of_asset number_of_days namein nameout\n")

#now reading and memorizing the values of the number of assets or the number of days
l=[]
print(sys.argv[3])
try:
	numasset=int(sys.argv[3])
	numday=int(sys.argv[4])
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
try:
	output = open(sys.argv[2], 'w') # opens the file
except IOError:
	print("Cannot open outputfile file %s for writing\n" % sys.argv[2])
	
#variables
lambdain=10
avg_asset_return=[]
upper=max(0.01,2.0/numasset)
lower=0
	
print("now writing output to file", sys.argv[2])
output.write("n "+str(numasset)+"\n\nj_lower_upper_mu\n\n")	
	
#calculating average asset returns list
for i in range(numasset):
	asset_prices_i=l[i][:numday+1]
	returns=0
	for t in range(numday):
		returns+=(float(asset_prices_i[t+1])-float(asset_prices_i[t]))/float(asset_prices_i[t])
	returns=returns/numday
	avg_asset_return.append(returns)
	output.write(str(i)+" "+str(lower)+"   "+str(upper)+"  "+str(avg_asset_return[i])+"\n")

output.write("\nlambda "+str(lambdain)+"\n\n")

#creating and calculating covariance matrix
output.write("Covariance_matrix: \n\n")
if numday==1:
	#Not sure of the covariance with just one term, but I choose 0 by default for now
	covariance_matrix=[0]*(numasset*numasset)
else:
	covariance_matrix=[]
	for i in range(numasset):
		asset_prices_i=l[i][:numday+1]
		for j in range(numasset):
			asset_prices_j=l[j][:numday+1]
			sum=0
			for t in range(numday):
				sum+=(((float(asset_prices_i[t+1])-float(asset_prices_i[t]))/float(asset_prices_i[t]))-avg_asset_return[i])*(((float(asset_prices_j[t+1])-float(asset_prices_j[t]))/float(asset_prices_j[t]))-avg_asset_return[j])
			#as it is a sample, we divide by T-1 and not by T
			covariance=sum/(numday-1)
			covariance_matrix.append(covariance)
			output.write(str(covariance_matrix[i*numasset+j])+" ")
		output.write("\n")

		

output.write("\nEND")
	
output.close
print("renaming ", sys.argv[5], "into ", sys.argv[6])
os.rename(sys.argv[5], sys.argv[6])


import matplotlib.pyplot as plt

plt.figure()
abscissa=[]
for i in range(numasset):
	abscissa.append(covariance_matrix[i*numasset+i])
ordinate=avg_asset_return

plt.plot(abscissa,ordinate,'rx')
plt.xlabel('volatility')
plt.ylabel('average rate of return')
plt.title('Plot of the average of return as a function of the volatility for each asset')
plt.savefig('plot.png',bbox_incher='tight')

