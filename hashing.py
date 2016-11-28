
import stub
import sys


#
# - reading material / sources / inspiration
#
#   * http://www.eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
#   * http://www.burtleburtle.net/bob/hash/doobs.html
#

def integer_to_bytes(value):
    return "".join(map(chr, [value >> i & 0xFF for i in (56, 48, 40, 32, 24, 16, 8, 0)]))


def bitrot32(value, n):
    assert n < 32
    return (value << n & 0xFFFFFFFF) ^ (value >> (32 - n) & 0xFFFFFFFF)


def bitcnt(value):
    n = 0
    while value:
        value &= value - 1
        n += 1
    return n


def histo(slots, ratio):

    from math import ceil

    #
    # - quantize the hit counts into a fixed number of buckets
    # - the bucket size is determined by the specified ratio
    #
    B = len(slots) * ratio
    bck = [0] * int(ceil(1/ratio))
    for n, collisions in enumerate(slots):
        bck[int(n/B)] += collisions

    r = 0
    l = sys.maxint
    for n in bck:
        if n < l:
            l = n
        if n > r:
            r = n

    print

    #
    # - scale each bucket into at most 8 characters
    #
    for n in bck:
        print '|%s' % ('#' * int(ceil(float((n - l) * 8) / (r - l))))

    print


def collision_check(method, S, K, generator=lambda n: str(n)):

    assert S > 0
    assert K > 0

    class hashed:

        #
        # - simply provide a wrapper for the desired hash implementation from our C++ module
        #
        def __init__(self, key):
            self.key = key

        def __hash__(self):
            return getattr(stub, method)(self.key)

    slots = stub.hash_table(S, [hashed(generator(n)) for n in range(0, K)]).slots

    collisions = 0
    for n in slots:
        if n > 1:
            collisions += n - 1

    expected = float(K)/S
    avg = float(sum(slots)) / len(slots)
    variance = sum((avg - n) ** 2 for n in slots) / len(slots)
    chi2 = sum((expected - n) ** 2 for n in slots) / expected

    print '%s : %d collisions [exp %d, mean %d, var %.1f, x2 %.1f]' % (method, collisions, expected, avg, variance, chi2)
    histo(slots, 0.05)


def bit_check(method):

    avg = 0
    impl = getattr(stub, method)
    for i in xrange(4096):
        for j in xrange(8):

            #
            # - rotate our pair of 32bit integer
            # - their differ by definition in exactly 1 bit
            #
            A = bitrot32(2 * i,     j)
            B = bitrot32(2 * i + 1, j)

            #
            # - compute their respective hash
            # - check how many bits are different
            #
            HA = impl(integer_to_bytes(A))
            HB = impl(integer_to_bytes(B))
            avg += bitcnt(HA ^ HB)

    print '%s : %d bits differ on average' % (method, avg/(4096*8))

if __name__ == '__main__':

    methods = \
        [
            'simple_xor',
            'rotating_xor',
            'bernstein',
            'shift_add_xor',
            'fnv',
            'one_at_a_time'
        ]

    #
    # - hash K keys into S slots and measure the collision rate
    #
    [collision_check(method, 4096, 4096, lambda n: integer_to_bytes(n)) for method in methods]

    #
    # - check how many bits change if 1 single bit differs in 2 hashed integers
    #
    [bit_check(method) for method in methods]