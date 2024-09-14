#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int data;
  struct Node *left, *right;
} Node;

Node *createNode(int data) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (!newNode) {
    perror("Failed to allocate memory");
    exit(EXIT_FAILURE);
  }
  newNode->data = data;
  newNode->left = newNode->right = NULL;
  return newNode;
}

Node *insert(Node *root, int data) {
  if (root == NULL)
    return createNode(data);
  if (data < root->data)
    root->left = insert(root->left, data);
  else if (data > root->data)
    root->right = insert(root->right, data);
  return root;
}

void inorder(Node *root) {
  if (root != NULL) {
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
  }
}

void freeTree(Node *root) {
  if (root != NULL) {
    freeTree(root->left);
    freeTree(root->right);
    free(root);
  }
}

int main(int argc, char *argv[]) {
  Node *root = NULL;
  int values[] = {50, 30, 70, 20, 40, 60, 80};
  int n = sizeof(values) / sizeof(values[0]);

  for (int i = 0; i < n; i++) {
    root = insert(root, values[i]);
  }

  printf("In-order traversal: ");
  inorder(root);
  printf("\n");

  freeTree(root);
  return 0;
}
