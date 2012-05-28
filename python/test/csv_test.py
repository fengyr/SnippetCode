import csv

context = csv.reader( open('item.csv', 'rb') )
#for i in context:
    #for ii in i:
        #print ii

item = {'name':'zenki', 'age':20, 'work':'programer'}
csvwrite = csv.DictWriter( open('dict.csv', 'wb'), ['name', 'age', 'work'] )
csvwrite.writerow(item)

csvread = csv.DictReader( open('dict.csv', 'rb'), ['name', 'age', 'work'] )
for i in csvread:
    print i



