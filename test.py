primes = []
r = range(2, 100)

for i in r:
    divisible = True
    for p in primes:
        if i % p == 0:
            divisible = False
            break

    if divisible:
        primes.append(i)

print(primes)
