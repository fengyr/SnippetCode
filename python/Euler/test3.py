"""
The prime factors of 13195 are 5, 7, 13 and 29.

What is the largest prime factor of the number 600851475143 ?
"""

roots = []; product = 1; x = 2; number = input("number?: "); y = number
while product != number:
	while (y % x == 0):
		roots.append(x)
		y /= x
		product *= roots[-1]
	x += 1
print roots

#id = 7751468
#jd = id - 1

#while jd > 1:
    #mark = False
    #if id % jd == 0:
        #for j in range(2, jd):
            #if jd % j == 0:
                #mark = True
                #break
        #if not mark:
            #print jd
            #break

    #jd = jd - 1


