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

  Pair *aux = searchTreeMap(tree, key);

  if (aux != NULL) {
    return;
  }

  int RorL = 1;

  tree->current = tree->root;
  TreeNode *search = tree->current;

  while (search != NULL) {
    int res = tree->lower_than(tree->current->pair->key, key);
    if (res == 1) {
      if (tree->current->right == NULL) {
        search = NULL;
        RorL = 1;
      } else {

        tree->current = tree->current->right;
      }
    } else if (res == 0) {

      if (tree->current->left == NULL) {
        search = NULL;
        RorL = 0;
      } else {

        tree->current = tree->current->left;
      }
    }
  }

  TreeNode *new = createTreeNode(key, value);

  new->parent = tree->current;

  if (RorL == 1) {
    tree->current->right = new;
  } else {

    tree->current->left = new;
  }
  tree->current = new;
}

TreeNode *minimum(TreeNode *x) {
  while (x->left != NULL) {
    x = x->left;
  }
  return x;
}

void removeNode(TreeMap *tree, TreeNode *node) {
  // sin hijos

  if (node->left == NULL && node->right == NULL) {
    if (tree->lower_than(node->parent->pair->key, node->pair->key) == 1) {
      node->parent->right = NULL;
    } else {
      node->parent->left = NULL;
    }

    return;
  }

  // con hijo

  if (node->left == NULL && node->right != NULL) {
    if (tree->lower_than(node->parent->pair->key, node->pair->key) == 1) {
      node->parent->right = node->right;
      node->right->parent = node->parent;
    }
    return;
  } else if (node->left != NULL && node->right == NULL) {
    if (tree->lower_than(node->parent->pair->key, node->pair->key) == 1) {
      node->parent->right = node->left;
      node->left->parent = node->parent;
    } else {
      node->parent->left = node->left;
      node->left->parent = node->parent;
    }

    return;
  }

  if (node->left != NULL && node->right != NULL) {
    TreeNode *aux = minimum(node->right);
    node->pair->key = aux->pair->key;
    node->pair->value = aux->pair->value;
    removeNode(tree, aux);
  }
}

void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL && tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key) {

  int comp = is_equal(tree, tree->root->pair->key, key);
  if (comp == 1) {
    tree->current = tree->root;
    return tree->root->pair;
  } else {

    tree->current = tree->root;
  }

  while (comp < 1) {

    int res = tree->lower_than(tree->current->pair->key, key);

    if (res == 1) {

      if (tree->current->right == NULL) {
        return NULL;
      }

      tree->current = tree->current->right;
    } else if (res == 0) {

      if (tree->current->left == NULL) {

        return NULL;
      }

      tree->current = tree->current->left;
    }

    comp = is_equal(tree, tree->current->pair->key, key);
  }

  return tree->current->pair;
}

Pair *upperBound(TreeMap *tree, void *key) {
  tree->current = tree->root;
  TreeNode *uNode = NULL;

  while (1) {
    if (tree->lower_than(tree->current->pair->key, key) == 1) {
      if (tree->current->right == NULL) {
        break;
      }

      tree->current = tree->current->right;
      continue;
    }

    if (tree->lower_than(tree->current->pair->key, key) == 0) {
      if (tree->current->left == NULL) {
        break;
      }

      tree->current = tree->current->left;
      uNode = tree->current->parent;
      continue;
    }
  }

  if (tree->lower_than(tree->current->pair->key, key) == 1 && uNode != NULL) {
    return uNode->pair;
  }

  if (uNode == NULL) {
    return NULL;
  }

  return tree->current->pair;
}

Pair *firstTreeMap(TreeMap *tree) {
  TreeNode *x = tree->root;
  tree->current = minimum(x);
  if (tree->current == NULL) {
    return NULL;
  }

  return tree->current->pair;
}

Pair *nextTreeMap(TreeMap *tree) { return NULL; }
