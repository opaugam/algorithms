
import random
import stub

if __name__ == '__main__':

    #
    # - prep the [0,MAX[ range
    # - randomize it
    # - setup our heap
    #
    MAX = 10
    R = range(0, MAX)
    random.shuffle(R)
    heap = stub.binary_heap(R)

    #
    # - pop the whole heap
    # - make sure each entries are following sequentially
    #
    last = -1
    while len(heap):
        cur = heap.pop()
        print cur
        assert cur == last + 1
        last = cur

    heap = stub.binomial_heap([1,2,3])