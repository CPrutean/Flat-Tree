#include "LinearTree.h"

#include <stdint.h>

// Reallocates a new buffer and returns the location of the old node in the new
// buffer

struct __tree_internals {
  Node *nodeBuffer;
  size_t capacity;
  size_t size;
};

static Node *reallocBuffer(Tree *tr, Node *n);
static int getInd(Tree *parent, Node *n);
static bool insertVal(Tree *tr, Node *n, DATA_T val);

Tree *treeCreate() {
  Tree *t = (Tree *)malloc(sizeof(Tree));
  t->tr = (__tree_internals *)malloc(sizeof(__tree_internals));
  size_t nodeBuffSize = sizeof(Node) * 3;
  t->tr->nodeBuffer = (Node *)malloc(nodeBuffSize);
  t->tr->capacity = nodeBuffSize;
  t->tr->size = 0;
  t->root = NULL;
  return t;
}

int getInd(Tree *parent, Node *n) {
  uintptr_t zero = (uintptr_t)parent->tr->nodeBuffer;
  uintptr_t curr = (uintptr_t)n;
  return (curr - zero) / sizeof(Node);
}

Node *reallocBuffer(Tree *tr, Node *n) {
  int ind = getInd(tr, n);
  uint32_t numNodes = tr->tr->capacity / DATA_T_SIZE;
  uint32_t newNodes = (numNodes / 2) * 2 + 2 + numNodes;
  size_t newSize = sizeof(Node) * newNodes;
  Node *newBuffer = (Node *)malloc(newSize);
  memcpy(newBuffer, tr->tr->nodeBuffer, tr->tr->capacity);
  free(tr->tr->nodeBuffer);
  tr->tr->nodeBuffer = newBuffer;
  tr->tr->capacity = newSize;
  return &tr->tr->nodeBuffer[ind];
}

Node *left(Tree *parent, Node *currNode) {
  if (currNode->left == 0) {
    return NULL;
  }
  int ind = getInd(parent, currNode);
  int newInd = ind * 2 + 1;
  return &parent->tr->nodeBuffer[newInd];
}

Node *right(Tree *parent, Node *currNode) {
  if (currNode->right == 0) {
    return NULL;
  }
  int ind = getInd(parent, currNode);
  int newInd = ind * 2 + 2;
  return &parent->tr->nodeBuffer[newInd];
}

bool insertValue(Tree *tr, DATA_T val) {
  if (tr == NULL) {
    return false;
  } else if (tr->root == NULL) {
    Node n;
    n.val = val;
    n.left = 0;
    n.right = 0;
    tr->tr->nodeBuffer[0] = n;
    tr->root = &tr->tr->nodeBuffer[0];
    return true;
  } else {
    return insertVal(tr, tr->root, val);
  }
}

bool insertVal(Tree *parent, Node *n, DATA_T val) {
  if (n->val == val) {
    return false;
  } else if (n == NULL) {
    return false;
  }
  int ind = getInd(parent, n);
  Node *next;
  if (val < n->val) {
    next = left(parent, n);
    ind = ind * 2 + 1;
  } else {
    next = right(parent, n);
    ind = ind * 2 + 2;
  }

  if (next == NULL) {
    size_t numNodes = parent->tr->capacity / DATA_T_SIZE;

    if (ind > numNodes) {
      Node *ptr = reallocBuffer(parent, n);
      n = ptr;
    }
    Node newN;
    newN.val = val;
    newN.left = 0;
    newN.right = 0;
    parent->tr->nodeBuffer[ind] = newN;

    if (val < n->val) {
      n->left = 1;
    } else {
      n->right = 1;
    }

    return true;
  } else {
    return insertVal(parent, next, val);
  }
}
