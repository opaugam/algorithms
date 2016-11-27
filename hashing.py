
import binascii
import os
import stub


def evaluate(method, S, K):

    class token:
        def __init__(self):

            self.key = binascii.b2a_hex(os.urandom(8))

        def __hash__(self):

            return getattr(stub, method)(self.key)

    slots = stub.hash_table(S, [token() for n in range(0, K)]).slots

    hits = 0
    for n in slots:
        if n > 1:
            hits += n - 1

    avg = sum(slots) / len(slots)
    variance = sum((avg - n) ** 2 for n in slots) / len(slots)
    print '%s : %.1d%% efficiency (%d hits), avg %d, variance %d' % (method, 100 - (100.0 * hits) / K, hits, avg, variance)

def evaluate2(method, S, K):
    pass


if __name__ == '__main__':

    methods = \
        [
            'simple_xor',
            'rotating_xor',
            'bernstein',
            'shift_add_xor',
            'fnv',
            'jenkins'
        ]

    [evaluate(method, 100000, 100000) for method in methods]

