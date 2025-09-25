import math
from pkcs1 import *

def find_invpow(x, n):
    """Finds the integer component of the n'th root of x,
    an integer such that y ** n <= x < (y + 1) ** n.
    """
    high = 1
    while high ** n < x:
        high *= 2
    low = high//2
    while low < high:
        mid = (low + high) // 2
        if low < mid and mid**n < x:
            low = mid
        elif high > mid and mid**n > x:
            high = mid
        else:
            return mid
    return mid + 1

M = "PPTI SERVER ACCESS ON 2023-11-06 20:02 UTC"
n = 0xed6b8f06d5b030d247e97994dd1d04f3016981dbde88e5b8f799a30817ce8f7097c6e8a0ba5f7579eba746e77d1bd3627d01cab793d1185710a9c4d8822310471bb60126f95bfc0dac38ee26c4fb6742d39ac49f0e828e4cdaf42f491b10cc8edd8474af88e6ed31a5f7de20b784260a8a70af6e0206606491a32b2ba3d6cc5065c31835a90bb9f33779622c8400b4a46c4a247d6aebf02235861353cb9ced948027706633944362bf6f823730dc7d9f6e5c2b52a88359fcfd182dbd1d753009030a6488aa160d021c715e35065d542c0f6f62ef5fd1fd02d4f80ecef6af2abf695cbfdfae2fe769034ef2849c9a92d010970a3739c0f37efb71afdf16abda4d
e = 3
k = key_length(n)
by = emsa_pkcs1_encode(M, k)
x = os2ip(by)
S = i2osp(find_invpow(x, 3), k)
print(S.hex())