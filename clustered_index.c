#include "clustered_index.h"
#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
    return (a > b)? a : b;
}

int height(Cluster* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

Cluster* cluster_insert(Cluster* root, int key) {
    if (root == NULL) {
        root = (Cluster*) malloc(sizeof(Cluster));
        root->key = key;
        root->left = NULL;
        root->right = NULL;
        root->height = 1;
        return root;
    }

    if (key < root->key) {
        root->left = cluster_insert(root->left, key);
    } else if (key > root->key) {
        root->right = cluster_insert(root->right, key);
    } else {
        // Key already exists, do nothing
        return root;
    }

    root->height = 1 + max(height(root->left), height(root->right));

    // Balance the tree
    int balance = height(root->left) - height(root->right);

    if (balance > 1) {
        if (height(root->left->left) >= height(root->left->right)) {
            return cluster_right_rotate(root);
        } else {
            root->left = cluster_left_rotate(root->left);
            return cluster_right_rotate(root);
        }
    }

    if (balance < -1) {
        if (height(root->right->right) >= height(root->right->left)) {
            return cluster_left_rotate(root);
        } else {
            root->right = cluster_right_rotate(root->right);
            return cluster_left_rotate(root);
        }
    }

    return root;
}

Cluster* cluster_search(Cluster* root, int key) {
    if (root == NULL || root->key == key) {
        return root;
    }

    if (root->key < key) {
        return cluster_search(root->right, key);
    }

    return cluster_search(root->left, key);
}

void cluster_inorder(Cluster* root) {
    if (root != NULL) {
        cluster_inorder(root->left);
        printf("%d ", root->key);
        cluster_inorder(root->right);
    }
}

Cluster* cluster_left_rotate(Cluster* z) {
    Cluster* y = z->right;
    Cluster* T2 = y->left;

    y->left = z;
    z->right = T2;

    z->height = max(height(z->left), height(z->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

Cluster* cluster_right_rotate(Cluster* z) {
    Cluster* y = z->left;
    Cluster* T3 = y->right;

    y->right = z;
    z->left = T3;

    z->height = max(height(z->left), height(z->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}