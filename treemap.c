#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
  Pair *pair;
  TreeNode *left;
  TreeNode *right;
  TreeNode *parent;
};

struct TreeMap {
  TreeNode *root;
  TreeNode *current;
  int (*lower_than)(void *key1, void *key2);
};

int is_equal(TreeMap *tree, void *key1, void *key2) {
  if (tree->lower_than(key1, key2) == 0 && tree->lower_than(key2, key1) == 0)
    return 1;
  else
    return 0;
}

TreeNode *createTreeNode(void *key, void *value) {
  TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
  if (new == NULL)
    return NULL;
  new->pair = (Pair *)malloc(sizeof(Pair));
  new->pair->key = key;
  new->pair->value = value;
  new->parent = new->left = new->right = NULL;
  return new;
}

// new->lower_than = lower_than;
TreeMap *createTreeMap(int (*lower_than)(void *key1, void *key2)) {
  TreeMap *map = (TreeMap *)malloc(sizeof(TreeMap));
  if (map == NULL) {
    return NULL;
  }

  map->root = NULL;
  map->current = NULL;
  map->lower_than = lower_than;

  return map;
}

void insertTreeMap(TreeMap *tree, void *key, void *value) {
  if (tree == NULL || key == NULL || value == NULL) {
    return;
  }

  TreeNode *nNode = createTreeNode(key, value);

  if (nNode == NULL) {
    return;
  }

  TreeNode *current = tree->root;
  TreeNode *parent = NULL;

  while (current != NULL) {
    parent = current;

    int comp = tree->lower_than(key, current->pair->key);

    if (comp == 0) {

      free(nNode->pair);
      free(nNode);
    } else {

      parent = current;

      if (comp < 0) {
        current = current->left;
      } else {

        current = current->right;
      }
    }
  }

  nNode->parent = parent;

  if (parent == NULL) {
    tree->root = nNode;
  } else if (tree->lower_than(key, parent->pair->key) < 0) {
    parent->left = nNode;
  } else {

    parent->right = nNode;
  }

  tree->current = nNode;
}

TreeNode *minimum(TreeNode *x) { return NULL; }

void removeNode(TreeMap *tree, TreeNode *node) {}

void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key) {

  if (tree == NULL || tree->root == NULL) {
    return NULL;
  }

  TreeNode *current = tree->root;

  while (current != NULL) {

    int comp = tree->lower_than(key, current->pair->key);

    if (comp == 0) {
      tree->current = current;
      return current->pair;
    } else if (comp < 0) {
      current = current->left;
    } else {
      current = current->right;
    }
  }

  tree->current = NULL;
  return NULL;
}

Pair *upperBound(TreeMap *tree, void *key) { return NULL; }

Pair *firstTreeMap(TreeMap *tree) { return NULL; }

Pair *nextTreeMap(TreeMap *tree) { return NULL; }
