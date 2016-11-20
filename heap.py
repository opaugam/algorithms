
import random
import stub

if __name__ == '__main__':

    #
    # - prep the [0,MAX[ range
    # - randomize it
    # - setup our heap
    #
    MAX = 1000000
    R = range(0, MAX)
    random.shuffle(R)
    heap = stub.binary_heap(R)

    #
    # - pop the whole heap
    # - make sure each entries are following sequentially
    #
    last = MAX
    while len(heap):
        cur = heap.pop()
        assert cur == last - 1
        last = cur
