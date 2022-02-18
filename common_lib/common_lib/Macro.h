#ifndef _MACRO_H_
#define _MACRO_H_

#define TRUE (0x1)
#define FALSE (0x0)

#define FP_DIFF_THRESHOLD 0.0001 //부동 소수점 차이 임계값 (epsilon)
#define LENGTH(array) ((sizeof(array)) / (sizeof(array[0])))
#define COMPARE(x, y) ((x) > (y) ? 1 : (x) == (y) ? 0 : -1) //x > y : 1, x == y : 0, x < y : -1
#define SWAP(x, y, tmp) ((tmp) = (x), (x) = (y), (y) = (tmp))

#define MAX(x, y) (COMPARE(x, y) == 1 ? x : y) //x > y : x, x <= y : y
#define MIN(x, y) (COMPARE(x, y) == 1 ? y : x) //x > y : y, x <= y : x
#define MEDIAN(x, y, z) MAX(MIN(x, y), MIN(MAX(x, y), z)) //ex) x < y && x > z : x

#define MAX_ARRAY_ELEMENT(array, idx1, idx2) \
(COMPARE(array[idx1], array[idx2]) == 1 ? array[idx1] : array[idx2]) //array[idx1] > array[idx2] : array[idx1], array[idx1] <= array[idx2] : array[idx2]
#define MIN_ARRAY_ELEMENT(array, idx1, idx2) \
(COMPARE(array[idx1], array[idx2]) == 1 ? array[idx2] : array[idx1]) //array[idx1] > array[idx2] : array[idx2], array[idx1] <= array[idx2] : array[idx1]
#define MEDIAN_ARRAY_ELEMENT(array, idx1, idx2, idx3) \
MAX_ARRAY_ELEMENT(array, \
MIN_ARRAY_ELEMENT(array, idx1, idx2), \
MIN_ARRAY_ELEMENT(array, \
MAX_ARRAY_ELEMENT(array, idx1, idx2), idx3)) //ex) x < y && x > z : x

#define MAX_ARRAY_ELEMENT_INDEX(array, idx1, idx2) \
(COMPARE(array[idx1], array[idx2]) == 1 ? idx1 : idx2) //array[idx1] > array[idx2] : idx1, array[idx1] <= array[idx2] : idx2
#define MIN_ARRAY_ELEMENT_INDEX(array, idx1, idx2) \
(COMPARE(array[idx1], array[idx2]) == 1 ? idx2 : idx1) //array[idx1] > array[idx2] : idx2, array[idx1] <= array[idx2] : idx1
#define MEDIAN_ARRAY_ELEMENT_INDEX(array, idx1, idx2, idx3) \
MAX_ARRAY_ELEMENT_INDEX(array, \
MIN_ARRAY_ELEMENT_INDEX(array, idx1, idx2), \
MIN_ARRAY_ELEMENT_INDEX(array, \
MAX_ARRAY_ELEMENT_INDEX(array, idx1, idx2), idx3)) //ex) x < y && x > z : x
#endif