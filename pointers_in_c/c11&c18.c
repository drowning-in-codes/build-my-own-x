#include <corecrt.h>
#include<stdio.h>
#include <stdlib.h>

void add_vector(int* restrict a,int* restrict b,int *restrict result,int n){
/* 告知编译器，在指针的生命周期内，只有它或从它直接派生的值才能访问它所指向的对象。
 * 这可以优化性能。 */
  for(int i =0;i<n;i++){
    result[i] = a[i] + b[i];
  }
}
int* createNumbers(int num){
  int* ptr = (int*)malloc(sizeof(int));
  if(!ptr){
    perror("Failed to allocate memory");
    exit(EXIT_FAILURE);
  }
  *ptr = num;
  return ptr;
}
typedef int* (*FuncPtr)(int);
int main(int argc, char *argv[]) {
  _Static_assert(sizeof(int) == 4, "int is not 4 bytes");
  size_t alignment = 16;
  size_t size = 64;
  // only available in linux now
  // Allocates memory with specified alignment.
  void *ptr = aligned_alloc(alignment, size);
  if (ptr == NULL) {
    // Handle allocation failure
  }
  // Use memory
  free(ptr);

/* 适当地使用const和volatile限定符可以增强代码的安全性和清晰度。 */
  int var = 20;
  const int* c_ptr = &var;
  int* volatile vptr = &var;

/* 将函数指针与返回指针的函数组合 */
  FuncPtr func = createNumbers;

  int* number = func(43);
  printf("Number: %d\n",*number);
  free(number);
  
  int arr[3][4] = {
  {1,2,3,4},
  {5,6,7,8},
  {9,10,11,12}
  };
/* 指针可以引用整个数组，从而允许多维数据操作。 */
  int (*di_ptr)[4] = arr;
  for(int i = 0;i<3;i++){
    for(int j = 0;j<4;j++){
      printf("%d ",di_ptr[i][j]);
    }
    printf("\n");
  }

  return 0;
}
