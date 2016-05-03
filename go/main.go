package main

import (
	"fmt"
	"math/rand"
	"os"
	"reflect"
	"sort"
	"strconv"
	"time"
)

// constants
const thQsort = 32

// insertion sort
func insertionSort(s []int, first int, last int) {
	for i := first + 1; i != last; i++ {
		for j := i; j != first && s[j] < s[j-1]; j-- {
			s[j], s[j-1] = s[j-1], s[j]
		}
	}
}

// quick sort
func quickSort(s []int, first int, last int) {
	// small range
	distance := last - first
	if distance <= thQsort {
		insertionSort(s, first, last)
		return
	}

	// find pivot
	mid := first + (distance / 2)

	// first > mid
	if s[mid] < s[first] {
		s[mid], s[first] = s[first], s[mid]
	}
	// first > last - 1
	if s[last-1] < s[first] {
		s[last-1], s[first] = s[first], s[last-1]
	}
	// mid > last - 1
	if s[last-1] < s[mid] {
		s[last-1], s[mid] = s[mid], s[last-1]
	}

	pivot := s[mid]
	left := first + 1
	right := last - 2
	for {
		// left
		for s[left] < pivot {
			left++
		}

		// right
		for pivot < s[right] {
			right--
		}

		if left >= right {
			break
		}

		s[left], s[right] = s[right], s[left]
		left++
		right--
	}

	// recursive
	quickSort(s, first, right+1)
	quickSort(s, left, last)
}

// main
func main() {
	// parameters
	size := 1
	if len(os.Args) > 1 {
		size, _ = strconv.Atoi(os.Args[1])
	}
	trials := 1
	if len(os.Args) > 2 {
		trials, _ = strconv.Atoi(os.Args[2])
	}

	// generate randomly-shuffled integers
	start := time.Now()
	slice := make([]int, size)
	for i := range slice {
		slice[i] = i
	}
	ans := make([]int, size)
	copy(ans, slice)
	for i := range slice {
		j := rand.Intn(i + 1)
		slice[i], slice[j] = slice[j], slice[i]
	}
	timeGen := time.Now().Sub(start).Seconds()

	// my sort
	timeMySort := 0.0
	for i := 0; i < trials; i++ {
		// copy
		sliceCopy := make([]int, size)
		copy(sliceCopy, slice)

		// sort
		start := time.Now()
		quickSort(sliceCopy, 0, len(sliceCopy))
		timeMySort += time.Now().Sub(start).Seconds()

		// check
		if i == 0 && !reflect.DeepEqual(ans, sliceCopy) {
			fmt.Fprintln(os.Stderr, "sort error")
			os.Exit(1)
		}
	}

	// std::sort
	timeStdSort := 0.0
	for i := 0; i < trials; i++ {
		// copy
		sliceCopy := make([]int, size)
		copy(sliceCopy, slice)

		// sort
		start := time.Now()
		sort.Ints(sliceCopy)
		timeStdSort += time.Now().Sub(start).Seconds()
	}

	// print
	fmt.Printf("%10d %12.9f", len(slice), timeGen)
	fmt.Printf(" %12.9f", (timeMySort / float64(trials)))
	fmt.Printf(" %12.9f", (timeStdSort / float64(trials)))
	fmt.Println()
}
