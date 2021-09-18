#include "node.h"

#include <iostream>
#include <memory>

Node::Node(int data): data_{data}, next_{nullptr} {}
Node::Node(): next_{nullptr} {}

void insertAtEnd(Node **head_ref, int data) {
    Node* new_node { new Node{data}};
    if(!(*head_ref)) {
        (*head_ref) = new_node;
        return;
    }
    Node* last_node {(*head_ref)};
    while(last_node->next_)
        last_node=last_node->next_;
    last_node->next_ = new_node;
}

void insertAtBeginning(Node** head_ref, int data) {
    Node* new_node { new Node{data}};
    new_node->next_ = (*head_ref);
    (*head_ref) = new_node;
}

void deleteNodeWithKey(Node** head_ref, int key) {
   if(!(*head_ref))
       return;
   if(((*head_ref)->data_) == key) {
        Node* node_to_delete {(*head_ref)};
        (*head_ref) = node_to_delete->next_;
        delete node_to_delete;
        return;
   }
   Node* prev_node {nullptr}, *node_to_delete {(*head_ref)};
   while(node_to_delete->data_ != key && node_to_delete) {
        prev_node = node_to_delete;
        node_to_delete = node_to_delete->next_;
   }
   if(!node_to_delete) return;
   prev_node->next_= node_to_delete->next_;
   delete node_to_delete;
}

void deleteNodeAtPosition(Node** head_ref, int position) {
    
   if(!(*head_ref))
       return;
   else if(position == 0) {
        Node* node_to_delete {(*head_ref)};
        (*head_ref) = node_to_delete->next_;
        delete node_to_delete;
        return;
   }
   Node* prev_node {nullptr}, *node_to_delete {(*head_ref)};
   for(int i{0}; i < position - 1 && node_to_delete; ++i) {
        prev_node = node_to_delete;
        node_to_delete = node_to_delete->next_;
   }
   if(!node_to_delete) return;
   prev_node->next_= node_to_delete->next_;
   delete node_to_delete;
}

void printLinkedList(Node* head) {
    while(head) {
        std::cout << head->data_ << ' ';
        head = head->next_;
    }
    std::cout << '\n';

}

