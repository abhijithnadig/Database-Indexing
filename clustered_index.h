#ifndef CLUSTERED_INDEX_H
#define CLUSTERED_INDEX_H

typedef struct Cluster {
    int key;
    struct Cluster* left;
    struct Cluster* right;
    int height;
} Cluster;

int height(Cluster* node);
Cluster* cluster_insert(Cluster* root, int key);
Cluster* cluster_search(Cluster* root, int key);
void cluster_inorder(Cluster* root);

#endif