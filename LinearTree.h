#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// #define _NO_DEFAULT_TREE_IMPL

#ifndef TREE_MALLOC
#include <stdlib.h>
#define TREE_MALLOC malloc
#endif

#ifndef DATAT_T
#define DATA_T int
#endif

#define DATA_T_SIZE sizeof(DATA_T) + (8 - sizeof(DATA_T) % 8)

// Ensure that it has 8 bytes of padding to allow for faster blocks of
// memory

typedef struct Node {
  DATA_T val;
  // Padding and just to
  uint32_t left;
  uint32_t right;
} Node;

// Abstract some stuff away
typedef struct __tree_internals __tree_internals;

typedef struct Tree {
  Node *root;
  // Where i shamefully hide all the details that makes my buffer work
  __tree_internals *tr;
} Tree;

// The whole point of this project is to avoid memory fragmentation
// so to actually avoid heap allocations instead i will be returning an
// expensive struct
bool insertValue(Tree *tr, DATA_T val);

Node *left(Tree *parent, Node *currNode);
Node *right(Tree *parent, Node *currNode);

Tree *treeCreate();
