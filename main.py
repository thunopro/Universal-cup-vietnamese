def signature(x):
    s = list(str(x) + str(x+1))
    s.sort()
    return ''.join(s)

def compute_D(d):
    lo = 10**(d-1)
    hi = 10**d - 1
    seen = set()
    CHUNK = 10**6
    for start in range(lo, hi+1, CHUNK):
        for x in range(start, min(hi, start+CHUNK-1) + 1):
            seen.add(signature(x))
        print(f"  filled up to {min(hi, start+CHUNK-1)} / {hi}")
    return len(seen)

for d in [7, 8,9]:  # you can do 9 if you like
    print(f"D[{d}] =", compute_D(d))
sdfdsf