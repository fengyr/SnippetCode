"""
For example 9009 = 91 x 99
Find the largest palindrome made from the product of two 3-digit numbers
"""

result = []

for x in range(100, 1000):
    for y in range(100, 1000):
        num = str(x*y)
        if num == num[::-1]:
            result.append(int(num))
print max(result)

