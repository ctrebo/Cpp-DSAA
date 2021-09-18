#include "bstnode.h"
#include <iostream> 
#include <queue>

BstNode::BstNode(): left_{nullptr}, right_ {nullptr}{

} 

BstNode::BstNode (int data): data_ {data}, left_ {nullptr}, right_ {nullptr} {

}

BstNode* getNewNode(int data) {
    BstNode* new_node { new BstNode{data}};
    return new_node;
}
void insertNodeBst(BstNode** root, int data) {
   // If root is nullptr, place where to place new node is found
   if(!(*root)) {
        (*root) = getNewNode(data);
        return;
   } else if(data <= (*root)->data_ ) {
        insertNodeBst(&((*root)->left_), data);
   } else {
        insertNodeBst(&((*root)->right_), data);
   }
}

bool searchInBst(BstNode* root, int data) {
    if(!root) return false;
    else if(root->data_ == data) {
        return true;
    } else if(data <= root->data_) {
       return searchInBst(root->left_, data);
    } else {
       return searchInBst(root->right_, data);
    }
}

int minBst(BstNode* root) {
    while(root->left_) {
        root = root->left_;
    }
    return root->data_;
}

int maxBst(BstNode* root) {
    while(root->right_) {
        root = root->right_;
    }
    return root->data_;
}
   
int heightBst(BstNode* root) {
    if(!root) return -1;
    int leftHeight {heightBst(root->left_)};
    int rightHeight {heightBst(root->right_)};

    return ((leftHeight > rightHeight) ? leftHeight : rightHeight) +1;
}
   
void inorderTraversal(BstNode* root) {
    std::cout << root->data_;
}

void levelOrder(BstNode* root) {
    if(!root) return;
    std::queue<BstNode*> qu;
    qu.push(root);
    while(!qu.empty()) {
        BstNode* current {qu.front()};
        std::cout << current->data_ << ' ';
        if(current->left_) qu.push(current->left_);
        if(current->right_) qu.push(current->right_);
        qu.pop();
            
    }
}

void preOrder(BstNode* root) {
    if(!root)return;
    std::cout << root->data_ << ' ';
    preOrder(root->left_);
    preOrder(root->right_);
}

void inOrder(BstNode* root) {
    if(!root)return;
    inOrder(root->left_);
    std::cout << root->data_ << ' ';
    inOrder(root->right_);
}

void postOrder(BstNode* root) {
    if(!root)return;
    postOrder(root->left_);
    postOrder(root->right_);
    std::cout << root->data_ << ' ';
}

bool isSubtreLesser(BstNode* root, int value){
    if(!root) return true;
    if(root->data_ <= value
    && isSubtreLesser(root->left_, value)
    && isSubtreLesser(root->right_, value)) {
        return true;    
    } else {
        return false;
    }
}

bool isSubtreGreater(BstNode* root, int value){
    if(!root) return true;
    if(root->data_ >  value
    && isSubtreGreater(root->left_, value)
    && isSubtreGreater(root->right_, value)) {
        return true;    
    } else {
        return false;
    }
}

bool isBst(BstNode* root) {
    if(!root) return true;
    if(isSubtreLesser(root->left_, root->data_)
    && isSubtreGreater(root->right_, root->data_)
    && isBst(root->left_)
    && isBst(root->right_)){
        return true;
    } else {
        return false;
    }
}

BstNode* findMin(BstNode* root) {
    while(root->left_) {
        root = root->left_;
    }
    return root;
}

void deleteBstNode(BstNode** root, int key) {
    if(!(*root)) return;
    else if(key < (*root)->data_) {
        deleteBstNode(&(*root)->left_, key);
    } else if(key > (*root)->data_) {
        deleteBstNode(&(*root)->right_, key);
    } else {
        // Case 1: No child
        if(!((*root)->left_) && !((*root)->right_)) {
            delete (*root);
            (*root) = nullptr;
        }
        // Case 2: 1 child
        else if(!((*root)->left_)) {
            BstNode* temp {(*root)};
            (*root) = (*root)->right_;
            delete temp;
        }
        else if(!((*root)->right_)) {
            BstNode* temp {(*root)};
            (*root) = (*root)->left_;
            delete temp;
        }
        // Case 3: 2 childs
        else {
            BstNode* temp {findMin((*root)->right_)};
            (*root)->data_ = temp->data_;
            deleteBstNode(&((*root)->right_), temp->data_);
        }
    }
    return;
}

BstNode* getSuccesor(BstNode* root, BstNode* current) {
    if(!current) return nullptr;
    // Case 1: BstNode has right subtree
    if(current->right_) {
        BstNode* temp {current->right_};
        while(temp->left_)
            temp = temp->left_;
        return temp;
    } 
    // Case 2: No right Subtree
    else {
        BstNode* successor {nullptr}; 
        BstNode* ancestor {root}; 
        while(ancestor != successor) {
            if(current->data_ < ancestor->data_) {
                successor = ancestor;
                ancestor = ancestor->left_;
            } else {
                ancestor = ancestor->right_;
            }
        } 
        return successor;
    }
}
