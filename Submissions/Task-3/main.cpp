#include <iostream>

/**
 *Time: O(n), Space: O(n)
 * Print numbers 1 thru n
 * @param n Highest number to print
 */
void printNaturalNumbers(int n) {
    if (n>1) {
        printNaturalNumbers(n-1);
    }
    std::cout<<n<<" ";
}

/**
 * Time: O(n), Space: O(n)
 * @param n int to calculate the factorial of
 * @return Factorial of n
 */
int calculateFactorial(int n) {
    if (n==0)
        return 1;
    else
        return  n * calculateFactorial(n-1);
}

/**
 * Time: O(n), Space: O(n)
 * @param base Base
 * @param exponent Exponent
 * @return base to the power of exponent
 */
int power(int base, int exponent) {
    if (exponent < 1)
        return 1;
    base = base * power(base, exponent-1);
    return base;
}

/**
 * This is not recursive.
 * @param n
 * @return Print n-th number of the fibonacci sequence
 */
int fibonacci(int n) {
    int f_R=0;
    int f_0 = 1;
    int f_1 = 1;
    for (size_t a = 0; a <= n; ++a) {
        f_R = f_0 + f_1;
        f_0=f_1;
        f_1=f_R;
    }
    return f_R;
}

/* #Python code for countOccurrences
 * def f(in:str, count:str)->int:
 *    if in[0] == count:
 *        return 1 + f(in[1:])
 *    elif in[0] != count:
 *        return 0 + f(in[1:])
 */

/**
 * Time: O(n), Space: O(n)
 * @param s Input string
 * @param c Character to find and count in s
 * @return Amount of c in s
 */
int countOccurrences(std::string s, char c) {
    if (s.length()==0)
        return 0;
    if (s.at(0) == c) {
        return 1 + countOccurrences(s.substr(1, -1), c);
    }
    else if (s.at(0) != c) {
        return 0 + countOccurrences(s.substr(1, -1), c);
    }
    return 0;
}


/**
 * Time: O(n), Space: O(n)
 * @param arr Array of integers
 * @param len Length of arr
 * @return Highest value in arr
 */
int findLargestElement(int arr[], int len) {
    if (len == 1) {
        return arr[0];
    }
    else if (len == 2) {
        if (arr[0] > arr[1]) {
            return arr[0];
        } else {
            return arr[1];
        }
    }
    else if (len > 2) {
        int len_1 = len / 2;
        int len_2 = len - len_1;
        int* arr_2 = arr + len_1;
        int can_1 = findLargestElement(arr, len_1);
        int can_2 = findLargestElement(arr_2, len_2);
        if (can_1 > can_2) {
            return can_1;
        }
        else {
            return can_2;
        }
    }
    else {
        return 0;
    }
}

/**
 * @brief Print ASCII characters form start to end
 *
 * Time: O(n), Space: O(n)
 * @param start First ASCII character
 * @param end Last ASCII character
 */
void traverseAsciiTable(char start, char end) {
    if (start <= end) {
        printf("%c ", start);
        ++start;
        traverseAsciiTable(start, end);
        printf("\033[31m %c\033[0m", start);
    }
}

int main() {
    printNaturalNumbers(36);
    std::cout<<std::endl<<calculateFactorial(4)<<std::endl;
    std::cout<<power(3,3)<<std::endl;
    std::cout<<fibonacci(6)<<std::endl;
    std::cout<<countOccurrences("hello world",'l')<<std::endl;

    int a[] = {1,2,600,4,5,6,32,8,-82,5,11,12,13,14,15,11};
    std::cout<<findLargestElement(a, 16)<<std::endl;;
    traverseAsciiTable('A','Z');
    return 0;
}
