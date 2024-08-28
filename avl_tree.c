// avl_tree.c
#include "avl_tree.h"
#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(AVLNode* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

AVLNode* avl_new_node(int key) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* avl_insert(AVLNode* root, int key) {
    if (root == NULL) {
        return avl_new_node(key);
    }

    if (key < root->key) {
        root->left = avl_insert(root->left, key);
    } else if (key > root->key) {
        root->right = avl_insert(root->right, key);
    } else {
        // Key already exists, do nothing
        return root;
    }

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = height(root->left) - height(root->right);

    if (balance > 1) {
        if (key < root->left->key) {
            return avl_right_rotate(root);
        } else {
            root->left = avl_left_rotate(root->left);
            return avl_right_rotate(root);
        }
    }

    if (balance < -1) {
        if (key > root->right->key) {
            return avl_left_rotate(root);
        } else {
            root->right = avl_right_rotate(root->right);
            return avl_left_rotate(root);
        }
    }

    return root;
}

AVLNode* avl_left_rotate(AVLNode* z) {
    AVLNode* y = z->right;
    AVLNode* T2 = y->left;

    y->left = z;
    z->right = T2;

    z->height = max(height(z->left), height(z->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

AVLNode* avl_right_rotate(AVLNode* z) {
    AVLNode* y = z->left;
    AVLNode* T3 = y->right;

    y->right = z;
    z->left = T3;

    z->height = max(height(z->left), height(z->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

AVLNode* avl_delete(AVLNode* root, int key) {
    if (root == NULL) {
        return root;
    }

    if (key < root->key) {
        root->left = avl_delete(root->left, key);
    } else if (key > root->key) {
        root->right = avl_delete(root->right, key);
    } else {
        if (root->left == NULL) {
            AVLNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            AVLNode* temp = root->left;
            free(root);
            return temp;
        }

        AVLNode* temp = avl_min_value_node(root->right);
        root->key = temp->key;
        root->right = avl_delete(root->right, temp->key);
    }

    if (root == NULL) {
        return root;
    }

    root->height = max(height(root->left), height(root->right)) + 1;

    int balance = height(root->left) - height(root->right);

    if (balance > 1) {
        if (height(root->left->left) >= height(root->left->right            )) {
            return avl_right_rotate(root);
        } else {
            root->left = avl_left_rotate(root->left);
            return avl_right_rotate(root);
        }
    }

    if (balance < -1) {
        if (height(root->right->right) >= height(root->right->left)) {
            return avl_left_rotate(root);
        } else {
            root->right = avl_right_rotate(root->right);
            return avl_left_rotate(root);
        }
    }

    return root;
}

AVLNode* avl_min_value_node(AVLNode* node) {
    AVLNode* current = node;

    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}

AVLNode* avl_search(AVLNode* root, int key) {
    if (root == NULL || root->key == key) {
        return root;
    }

    if (root->key < key) {
        return avl_search(root->right, key);
    }

    return avl_search(root->left, key);
}

void avl_inorder(AVLNode* root) {
    if (root != NULL) {
        avl_inorder(root->left);
        printf("%d ", root->key);
        avl_inorder(root->right);
    }
}