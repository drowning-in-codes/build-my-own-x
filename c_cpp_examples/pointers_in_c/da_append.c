#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *items;
  size_t counts;
  size_t capacity;
} Numbers;
#define EMPTY()
#define NOT_EMPTY(x) x

#ifndef DEBUG
#define DEBUG
#endif
#ifdef DEBUG
#define LOG(...) fprintf(stderr, "Debug Log: " __VA_ARGS__)
#else
#define LOG(...)
#endif
#define DA_ARRAY(name, dtype)                                                  \
  typedef struct {                                                             \
    dtype *items;                                                              \
    size_t counts;                                                             \
    size_t capacity;                                                           \
  } name##Array##_t;

#define da_append(xs, x)                                                       \
  if ((xs)->counts >= (xs)->capacity) {                                        \
    if ((xs)->capacity == 0) {                                                 \
      (xs)->capacity = 256;                                                    \
    } else {                                                                   \
      (xs)->capacity *= 2;                                                     \
    }                                                                          \
    (xs)->items = realloc((xs)->items, (xs)->capacity * sizeof(*(xs)->items)); \
  }                                                                            \
  (xs)->items[(xs)->counts++] = (x);

#define da_reset(xs, dtype, new_capacity)                                      \
  do {                                                                         \
    free((xs)->items);                                                         \
    (xs)->items = NULL;                                                        \
    (xs)->counts = 0;                                                          \
    (xs)->items = realloc((xs)->items, (new_capacity) * sizeof(dtype));        \
    (xs)->capacity = (new_capacity);                                           \
  } while (0)

#define da_reset_with_capacity(xs, dtype, capacity)                            \
  da_reset(xs, dtype, capacity)

#define da_reset_simple(xs, dtype)                                             \
  da_reset_with_capacity(xs, dtype, (xs)->capacity)

int main() {
  Numbers numbers = {0};
  for (int i = 0; i < 10; ++i) {
    da_append(&numbers, i);
  }
  for (size_t i = 0; i < numbers.counts; i++) {
    printf("%d ", numbers.items[i]);
  }
  da_reset_simple(&numbers, double);
  for (int i = 0; i < 10; ++i) {
    da_append(&numbers, i);
  }
  for (size_t i = 0; i < numbers.counts; i++) {
    printf("%d ", numbers.items[i]);
  }

  return 0;
}
