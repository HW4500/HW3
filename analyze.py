import sys
import csv
#To read : here we consider that day 0 is the initial day, therefore if N=1, we give the return for the first day, (p(1)-p(0))/p(0)
#Because of that, N cannot go over 249

if len(sys.argv) != 5:
	sys.exit("usage: analyze.py datafilename outputfilename number_of_asset number_of_days\n")

#now open and read data file
l=[]
try:
	numasset=int(sys.argv[3])
	numday=int(sys.argv[4])
except ValueError:
	sys.exit("Cannot read the number of assets or the number of days, must be integers")

if numasset>1000 or numday>=250:
	sys.exit("number of assets or number of days out of range : assets from 1 to 1000, days from 0 to 249")


try: 
	datafile= open(sys.argv[1], 'r') 
	reader = csv.reader(datafile)
	for row in reader:
		l.append(row)
	datafile.close
except IOError:
	sys.exit("Cannot open file %s\n" % sys.argv[1])

try:
	output = open(sys.argv[2], 'w') # opens the file
except IOError:
	print("Cannot open outputfile file %s for writing\n" % sys.argv[2])
	

print("now writing output to file", sys.argv[2])
output.write("number_of_assets: "+str(numasset)+" number_of_days: "+str(numday)+"\n")	
	

avg_asset_return=[]
for i in range(numasset):
	output.write("Average_return_of_asset_"+str(i+1)+": ")
	asset_prices=l[i][:numday+1]
	returns=0
	for j in range(numday):
		returns+=(float(asset_prices[j+1])-float(asset_prices[j]))/float(asset_prices[j])
	returns=returns/numday
	avg_asset_return.append(returns)
	output.write(str(returns)+"\n")
	

	


