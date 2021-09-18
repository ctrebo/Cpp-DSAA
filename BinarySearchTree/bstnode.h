#ifndef NODE_BINARY_S_TREE_H
#define NODE_BINARY_S_TREE_H

// Making class only available for ints
class BstNode {
public:
    BstNode(); 
    BstNode(int data); 

    int data_ {};
    BstNode* left_ {}; 
    BstNode* right_ {};

};

void insertNodeBst(BstNode** root, int data);
BstNode* getNewNode(int data);
bool searchInBst(BstNode* root, int data);
int minBst(BstNode* root);
int maxBst(BstNode* root);
int heightBst(BstNode* root);
void levelOrder(BstNode* root);
void preOrder(BstNode* root);
void inOrder(BstNode* root);
void postOrder(BstNode* root);
// Function and subfunctions for isBst()
bool isSubtreeLesser(BstNode* root, int value);
bool isSubtreGreater(BstNode* root, int value);
bool isBst(BstNode* root);
// Functions and subfunctions for deleteBstNode
BstNode* findMin(BstNode* root);
void deleteBstNode(BstNode** root, int key);
BstNode* getSuccesor(BstNode* root, BstNode* current);
#endif //NODE_BINARY_S_TREE_H
