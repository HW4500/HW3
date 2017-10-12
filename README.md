# HW3
The file analyse.py is the one taking as an entry the dataset (csv file) and having as an output a text file containing the same info as example.txt

the cpp code is only in its second version. Very messy, only one sheet instead of one per function, but works for the example (tested it versus Matlab) and now seems to work for any file (not tested versus Matlab yet).
To do: work on the final output file, clean it, comment it, separate it on different sheets, make more protections (if file note of the good type, return error), use the returncodes to make it easy to read, test it on Matlab, try to make it more efficient(1) (for 1000 assets and 250 days), ...

To make the code work: 
1) create a void project on visual basics
2) add to this project the cpp file
3) build it
4) in the debug file (where you can find project.exe), put analyze.py, dump2.csv and example.txt
5) call the .exe via the command line



(1)=The slow part of the code is the python part. It is where we should focus our work.
