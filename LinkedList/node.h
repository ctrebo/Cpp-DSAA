#ifndef LINKEDLIST_H
#define LINKEDLIST_H

class Node {
public:
    Node(int data);
    Node();
    int data_;
    Node* next_;
};

void insertAtBeginning(Node **head_ref, int data);
void insertAtEnd(Node **head_ref, int data);

void deleteNodeWithKey(Node** head_ref, int key);
void deleteNodeAtPosition(Node** head_ref, int position);

void printLinkedList(Node* head);
#endif // LINKEDLIST_H
