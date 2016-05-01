//------------------------------------------------------------------------------
/**
 * @file Main.java
 */
//------------------------------------------------------------------------------

import java.util.List;
import java.util.Collections;
import java.util.Arrays;
import java.util.stream.IntStream;
import java.util.stream.Collectors;

public class Main {

    private static final int TH_QSORT = 32;

    // swap
    private static <T> void swap(T[] a, int left, int right) {
        final T tmp = a[left];
        a[left] = a[right];
        a[right] = tmp;
    }

    // insertion sort
    private static <T extends Comparable<? super T>>
        void insertionSort(T[] a, int first, int last) {

        for (int i = first + 1; i != last; i++) {
            for (int j = i; j != first && a[j].compareTo(a[j - 1]) < 0; j--) {
                swap(a, j, j - 1);
            }
        }
    }

    // quick sort
    private static <T extends Comparable<? super T>>
        void quickSort(T[] a, int first, int last) {

        // small range
        if (last - first <= TH_QSORT) {
            insertionSort(a, first, last);
            return;
        }

        // find pivot
        int mid = first + (last - first) / 2;

        // first > mid
        if (a[mid].compareTo(a[first]) < 0) {
            swap(a, mid, first);
        }
        // first > last - 1
        if (a[last - 1].compareTo(a[first]) < 0) {
            swap(a, last - 1, first);
        }
        // mid > last - 1
        if (a[last - 1].compareTo(a[mid]) < 0) {
            swap(a, last - 1, mid);
        }

        final T pivot = a[mid];
        int left = first + 1;
        int right = last - 2;
        while (true) {
            // left
            while (a[left].compareTo(pivot) < 0) {
                ++left;
            }

            // right
            while (pivot.compareTo(a[right]) < 0) {
                --right;
            }

            if (left >= right) {
                break;
            }

            swap(a, left++, right--);
        }

        // recursive
        quickSort(a, first, right + 1);
        quickSort(a, left, last);


    }

    // main
    public static void main(String[] args) {
        // parameters
        final int size = (args.length > 0) ? Integer.parseInt(args[0]) : 1;
        final int trials = (args.length > 1) ? Integer.parseInt(args[1]) : 1;

        // generate randomly-shuffled integers
        long start = System.nanoTime();
        List<Integer> list = IntStream.range(0, size)
            .boxed()
            .collect(Collectors.toList());
        Integer[] ans = (Integer[])list.toArray(new Integer[list.size()]);
        Collections.shuffle(list);
        Integer[] array = (Integer[])list.toArray(new Integer[list.size()]);
        final double timeGen = (System.nanoTime() - start) * 1e-9;

        // my sort
        double timeMySort = 0.0;
        for (int i = 0; i < trials; i++) {
            // copy
            Integer[] arrayCopy = array.clone();

            // sort
            start = System.nanoTime();
            quickSort(arrayCopy, 0, arrayCopy.length);
            timeMySort += (System.nanoTime() - start) * 1e-9;

            // check
            if (i == 0 && !Arrays.equals(ans, arrayCopy)) {
                System.err.println("sort error");
                System.exit(1);
            }
        }

        // Arrays.sort
        double timeStdSort = 0.0;
        for (int i = 0; i < trials; i++) {
            // copy
            Integer[] arrayCopy = array.clone();

            // sort
            start = System.nanoTime();
            Arrays.sort(arrayCopy);
            timeStdSort += (System.nanoTime() - start) * 1e-9;
        }

        // print
        System.out.printf("%10d", array.length);
        System.out.printf(" %12.9f", timeGen);
        System.out.printf(" %12.9f", timeMySort / trials);
        System.out.printf(" %12.9f", timeStdSort / trials);
        System.out.println();
    }
}

//------------------------------------------------------------------------------
