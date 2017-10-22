import csv
import datetime
import numpy
import sys

if(len(sys.argv) != 5):
    sys.exit("usage: cov.py datafilename outputfilename number_of_asset number_of_days\n")
    

    
def asset_retrun(R,C):
    z=datetime.datetime.now()
    try:
        
        csv_file =open(sys.argv[1],'r')
        data_in_file=csv.reader(csv_file)

        output = open(sys.argv[2], 'w') # opens the file
        #variables
        lambdain=10
        
        upper=max(0.01,2.0/rows_)
        lower=0

        output.write("n "+str(rows_)+"\n\nj_lower_upper_mu\n\n")
        
        avg_return_for_each_asset=[] # stores avg return (mu) of each asset for number of columns given
        day_over_day_return=[] # stores list of lists for day over day return of all assest
        for row_no,row_content in enumerate(data_in_file):
            avg_return=0 
            if(row_no<=R-1):
                column_no=0
                temp=[]
                while (column_no<=C-2):
                    current_day_return=(float(row_content[column_no + 1])-float(row_content[column_no]))/float(row_content[column_no])
                    temp.append(current_day_return)
                    avg_return=avg_return + current_day_return
                    column_no=column_no+1
                else:
                    day_over_day_return.append(temp)
                    (avg_return_for_each_asset.append(avg_return/(C-1)))
                    output.write(str(row_no)+" "+str(lower)+"   "+str(upper)+"  "+str((avg_return/(C-1)))+"\n")
            else:
                break

        output.write("\nlambda "+str(lambdain)+"\n\n")
        #creating and calculating covariance matrix	
        output.write("Covariance_matrix: \n\n")
        cov_matrix=numpy.zeros(shape=(R,R))
        for j in range(R): #matrix rows
            for i in range(R): # matrix columns 
                if(i>=j):
                    sum=0
                    T=0
                    while(T<=C-2): # calculating the covariance of asset i and j
                        sum=sum+((day_over_day_return[j][T]-avg_return_for_each_asset[j])*(day_over_day_return[i][T]-avg_return_for_each_asset[i]))
                        T=T+1
                    else:
                        cov_matrix[j][i]=sum/(C-2)
                        output.write(str(cov_matrix[j][i])+" ")
                else:
                    cov_matrix[j][i]=cov_matrix[i][j]
                    output.write(str(cov_matrix[j][i])+" ")
                    
            output.write("\n")
        output.write("\nEND")
        output.close()
        csv_file.close()

    except Exception as e:
        print(e)
    #for i in avg_return_for_each_asset:
    #    print(i)
    #for j in day_over_day_return:
        #print(j)
    #print(cov_matrix)
    
    
    

    y=datetime.datetime.now()
    duration= (y-z)
    seconds = duration.total_seconds()
    hours = seconds // 3600
    minutes = (seconds % 3600) // 60
    seconds = seconds % 60
    print("completed in {}hours {} mins {} seconds".format(hours,minutes,seconds))
    
rows_=int(sys.argv[3])
columns_=int(sys.argv[4])
asset_retrun(rows_,columns_)