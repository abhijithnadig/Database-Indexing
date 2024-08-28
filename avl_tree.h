
#ifndef AVL_TREE_H
#define AVL_TREE_H

typedef struct AVLNode {
    int key;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

AVLNode* avl_insert(AVLNode* root, int key);
AVLNode* avl_delete(AVLNode* root, int key);
AVLNode* avl_search(AVLNode* root, int key);
void avl_inorder(AVLNode* root);

#endif