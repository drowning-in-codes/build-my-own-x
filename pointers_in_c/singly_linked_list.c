#include <complex.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#define DEBUG_PRINT(message, ...) fprintf(stderr, message, ##__VA_ARGS__)
#define LOG(level, message, ...) printf("[%s] " message, level, ##__VA_ARGS__)
#define WARNING(message, ...)                                                  \
  fprintf(stderr, "Warning: " message, ##__VA_ARGS__)
typedef struct Node {
  int data;
  struct Node *next;
} Node;

Node *createNode(int data) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (!newNode) {
    perror("Failed to allocate memory");
    exit(EXIT_FAILURE);
  }
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

void append(Node **head, int data) {
  Node *newNode = createNode(data);
  if (*head == NULL) {
    *head = newNode;
    return;
  }
  Node *temp = *head;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = newNode;
}
void printList(Node *head) {
  while (head != NULL) {
    printf("%d-> ", head->data);
    head = head->next;
  }
  printf("NULL\n");
}
void freeList(Node *head) {
  Node *temp;
  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp);
  }
}
void func(int num, ...) {
  va_list valist;
  va_start(valist, num);
  for (int i = 0; i < num; i++) {
    printf("%d ", va_arg(valist, int));
  }
  va_end(valist);
}
int main() {
  Node *head = NULL;
  append(&head, 10);
  append(&head, 20);
  append(&head, 30);
  append(&head, 40);
  append(&head, 50);
  printList(head);
  freeList(head);
  head = NULL;
  return 0;
}
