#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
int build_dynamic_arr() {
  int *arr = NULL;
  int initial_size = 5;
  int new_size = 10;
  arr = (int *)malloc(initial_size * sizeof(int));
  if (arr == NULL) {
    perror("malloc failed");
    return EXIT_FAILURE;
  }
  for (int i = 0; i < initial_size; i++) {
    arr[i] = i + 1;
  }
  int *temp = (int *)realloc(arr, new_size * sizeof(int));
  if (temp == NULL) {
    perror("Failed to reallocate memory");
    free(arr);
    return EXIT_FAILURE;
  }
  arr = temp;
  for (int i = 0; i < new_size; i++) {
    arr[i] = i + 1;
  }
  for (int i = 0; i < new_size; i++) {
    printf("%d", arr[i]);
  }
  free(arr);
  arr = NULL;
  return EXIT_SUCCESS;
}
void greet() { printf("Hello, World!\n"); }
int compare(const void *a, const void *b) {
  int int_a = *((int *)a);
  int int_b = *((int *)b);
  return (int_a - int_b);
}
typedef struct {
  int id;
  char name[20];
} Person;
int main() {
  // int *intPtr;
  // char *charPtr;
  // float *floatPtr;

  int var = 53;
  int *intPtr = &var;
  int value = *intPtr;
  *intPtr = 10;
  void *genericPtr;
  // A void *pointer is a generic pointer type that can hold the address of any
  //     data type.However,
  //     it cannot be dereferenced directly without casting.
  int num = 10;
  genericPtr = &num;

  printf("Value: %d\n", *((int *)genericPtr));
  int arr[5] = {10, 20};
  int *ptr = arr;
  ptr++;
  printf("%d\n", *ptr);
  ptr += 2;
  printf("%d\n", *ptr);
  // Addition/Subtraction: Adding or subtracting an integer n to/from a pointer
  // moves it n elements forward or backward, not n bytes. Difference:
  // Subtracting two pointers gives the number of elements between them.
  // Comparison: Pointers can be compared using relational operators if they
  // point to elements within the same array.

  // 在C语言中，数组名的行为类似于指针，因为它表示其第一个元素的内存地址。
  // 然而，数组和指针是不同的。数组名是一个常量指针，这意味着它的地址是固定的，不能改变，而指针可以重新分配指向不同的内存位置。
  int narr[3] = {1, 2, 3};
  int *nptr = narr;
  printf("%d\n", *(nptr + 1));
  printf("%d\n", narr[1]);
  for (int i = 0; i < 3; i++) {
    printf("%d", *(nptr + i));
  }
  for (int i = 0; i < 3; i++) {
    printf("%d", *(narr + i));
  }

  // Dynamic memory allocation allows programs to request memory during runtime,
  //, providing flexibility for data structures whose size may change.
  //
  // providing flexibility for data structures whose size may change. Allocates
  // a specified number of bytes and returns a pointer to the first byte.
  int *n_ptr = (int *)malloc(5 * sizeof(int));
  if (n_ptr == NULL) {
  }
  // Allocates memory for an array of elements, initializes all bytes to zero,
  // and returns a pointer.
  int *r_ptr = (int *)calloc(5, sizeof(int));

  if (r_ptr == NULL) {
  }
  // Resizes previously allocated memory.
  ptr = (int *)realloc(ptr, 10 * sizeof(int));
  if (ptr == NULL) {
  }

  // Deallocates previously allocated memory to prevent memory leaks.
  free(ptr);
  ptr = NULL;

  int num_1 = 6;
  int *pptr = &num_1;
  int **dptr = &ptr;
  printf("%d\n", **dptr);

  // dynamic 2d arrays
  int rows = 3, cols = 4;
  int **matrix = (int **)malloc(rows * sizeof(int *));
  for (int i = 0; i < rows; i++) {
    matrix[i] = (int *)malloc(cols * sizeof(int));
  }
  matrix[0][0] = 1;
  matrix[2][3] = 10;
  for (int i = 0; i < rows; i++) {
    free(matrix[i]);
  }
  free(matrix);
  // arrays of strings

  char *strings[] = {"one", "two", "three"};
  char **sptr = strings;
  printf("%s %s!\n", sptr[0], sptr[1]);

  // multidimensional arrays
  int new_matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};

  int(*n_ptr2)[3] = new_matrix; // pointers to an array of integers

  // 函数指针存储函数的地址，允许将函数作为参数传递、存储在数组中或动态调用。
  void (*funcPtr)() = greet;
  int my_arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
  int size = sizeof(arr) / sizeof(arr[0]);
  qsort(my_arr, size, sizeof(int), compare);
  for (int i = 0; i < size; i++) {
    printf("%d", arr[i]);
  }

  // Accessing Struct Members via Pointers
  Person person = {1, "Alice"};
  Person *p_ptr = &person;
  printf("ID:%d, Name:%s\n", p_ptr->id, p_ptr->name);
  (*p_ptr).id = 2;
  printf("ID:%d, Name:%s\n", p_ptr->id, p_ptr->name);
  return 0;
}
