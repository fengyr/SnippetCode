"""
2520 is the smallest number that can be divided by each of the numbers from 1 to 10 without any remainder.
What is the smallest number that is evenly divisible by all of the numbers from 1 to 20?
"""

def isPrimer(num):
    import math
    m = int(math.sqrt(num))
    if num < 2: return False
    if num == 2: return True

    for i in range(2, m+1):
        if (num % i) == 0:
            return False

    return True

def max_divisor(num):
    numlist = []
    flag = False
    for i in range(1, num):
        if isPrimer(i):
            x = 2
            while True:
                if i**x > num:
                    numlist.append(str(i**(x-1)))
                    flag = True
                    break
                else:
                    x += 1

            if not flag:
                numlist.append(str(i))

    return eval('*'.join(numlist))

print max_divisor(21)


