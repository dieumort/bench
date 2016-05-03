#!/usr/bin/env python
# -*- coding: utf-8 -*-

#-------------------------------------------------------------------------------

import sys
import time
import random

#-------------------------------------------------------------------------------

# insertion sort
def insertion_sort(l, first, last):
    for i in xrange(first + 1, last):
        for j in reversed(xrange(first + 1, i + 1)):
            if l[j] < l[j - 1]:
                l[j], l[j - 1] = l[j - 1], l[j]
            else:
                break

#-------------------------------------------------------------------------------

#  quick sort
def quick_sort(l, first, last):
    # small range
    distance = last - first
    if distance <= 32:
        insertion_sort(l, first, last)
        return

    # find pivot
    mid = first + distance / 2

    # first > mid
    if l[mid] < l[first]:
        l[mid], l[first] = l[first], l[mid]
    # first > last - 1
    if l[last - 1] < l[first]:
        l[last - 1], l[first] = l[first], l[last - 1]
    # mid > last - 1
    if l[last - 1] < l[mid]:
        l[last - 1], l[mid] = l[mid], l[last - 1]

    pivot = l[mid]
    left = first + 1
    right = last - 2;
    while 1:
        # left
        while l[left] < pivot:
            left += 1

        # right
        while pivot < l[right]:
            right -= 1

        if left >= right:
            break

        l[left], l[right] = l[right], l[left]
        left += 1
        right -= 1

    # recursive
    quick_sort(l, first, right + 1)
    quick_sort(l, left, last)

#-------------------------------------------------------------------------------

def main():
    # parameters
    size = int(sys.argv[1]) if len(sys.argv) > 1 else 1
    trials = int(sys.argv[2]) if len(sys.argv) > 2 else 1

    # generate randomly-shuffled integers
    start = time.time()
    l = range(0, size)
    ans = range(0, size)
    random.shuffle(l)
    t_gen = time.time() - start

    # my sort
    t_my_sort = 0.0;
    for i in xrange(0, trials):
        # copy
        l_copy = l[:]

        # sort
        start = time.time()
        quick_sort(l_copy, 0, len(l_copy))
        t_my_sort += time.time() - start

        # check
        if i == 0 and ans != l_copy:
            print >> sys.stderr, "sort error"
            sys.exit(1)

    # std::sort
    t_std_sort = 0.0;
    for i in xrange(0, trials):
        # copy
        l_copy = l[:]

        # sort
        start = time.time()
        l_copy.sort()
        t_std_sort += time.time() - start

    print "%10d %12.9f" % (size, t_gen),
    print "%12.9f %12.9f" % (t_my_sort / trials, t_std_sort / trials)

#-------------------------------------------------------------------------------

if __name__ == "__main__":
    main()

#-------------------------------------------------------------------------------
