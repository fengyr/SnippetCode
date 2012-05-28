"""
By listing the first six prime numbers: 2, 3, 5, 7, 11, and 13, we can see that the 6^(th) prime is 13.
What is the 10001^(st) prime number?
"""

def isPrime(num):
    import math
    m = int(math.sqrt(num))
    if num < 2: return False
    if num == 2: return True

    for i in range(2, m+1):
        if (num % i) == 0:
            return False

    return True

def last_prime(id):
    num = 3
    total = 2
    while total < id:
        num += 1
        if isPrime(num):
            #print num
            total += 1

    return num

print last_prime(10001)




