int main() {
  /* A dangling pointer points to memory that has been freed or deallocated.
   * Dereferencing such pointers leads to undefined behavior. */
  /* 悬空指针指向已释放的内存。解引用这样的指针会导致未定义的行为。 */
  cfree(ptr);
  ptr = NULL;

  /* 解引用NULL指针会导致程序崩溃。总是初始化指针，并在解引用前检查是否为NULL。
   */
  cint *ptr = NULL;
  if (ptr != NULL) {
    // Safe to dereference
    int value = *ptr;
  } else {
    // Handle null pointer
  }
  /* 总是在声明时初始化指针 */
  cint *ptr = NULL; // Initialize to NULL
  // 指针运算不当或内存分配不足可能导致缓冲区溢出，
  // 从而影响程序的稳定性和安全性
  // 始终确保指针引用有效的内存区域。
  /* 使用限制写入字节数的函数(例如，用strncpy代替strcpy)。 */
}
