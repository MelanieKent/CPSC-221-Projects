#include "chain.h"
#include "chain_given.cpp"
#include <cmath>
#include <iostream>

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain()
{
    clear();
} 

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block &ndata)
{
    Node *curr = head_;

    while (curr->next != head_) {
        curr = curr->next;
    }
    curr->next = new Node{ndata};
    curr->next->next = head_;
    length_++;
}


/**
 * Swaps the two nodes at the indexes "node1" and "node2".
 * The indexes of the nodes are 1 based.
 * assumes i and j are valid (in {1,length_} inclusive)
 * 
 */
void Chain::swap(int i, int j)
{
    Node *iPre = head_;
    Node *jPre = head_;
    Node *iTarget = walk(head_, i);
    Node *jTarget = walk(head_, j);
    Block iData = walk(head_,i)->data;
    Block jData = walk(head_,j)->data;

    while (iPre->next != iTarget) {
        iPre = iPre->next;
    }
    Node *iRest = iTarget->next;
    delete iPre->next;
    iPre->next = new Node(jData);
    iPre->next->next = iRest;

    while (jPre->next != jTarget) {
        jPre = jPre->next;
    }
    Node *jRest = jTarget->next;
    delete jPre->next;
    jPre->next = new Node(iData);
    jPre->next->next = jRest;
}

/**
 * Reverses the chain
 */
void Chain::reverse()
{
    if (head_->next == head_) {
        return;
    }

    if (head_->next->next == head_) {
        return;
    }

    for (int i = 1; i <= length_/2; i++) {
        swap(i, (length_+1-i));
    }
}

/*
* Modifies the current chain by "rotating" every k nodes by one position.
* In every k node sub-chain, remove the first node, and place it in the last 
* position of the sub-chain. If the last sub-chain has length less than k,
* then don't change it at all. 
* Some examples with the chain a b c d e:
*   k = 1: a b c d e
*   k = 2: b a d c e
*   k = 3: b c a d e
*   k = 4: b c d a e
*/
void Chain::rotate(int k)
{
    if (k == 1) {
        return;
    }
    if (k == length_) {
        swap(1,k);
        return;
    }

    int oldK = k;
    int count = 1;
    
    while (k <= length_) {
        for (int i = count; i < k; i++) {
            swap(i,i+1);
            count++;
        }
        count++;
        k += oldK;
    }
}

/**
 * Destroys ALL dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear()
{
    Node *curr = head_->next;

    while (curr != head_) {
        Node *next = curr->next;
        delete curr;
        curr = next;
    }
    delete head_;
    head_ = NULL;
    
}

/* makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other)
{
    head_ = new Node();
    head_->next = head_;
    width_ = other.width_;
    height_ = other.height_;
    length_ = other.length_;

    Node *oCurr = other.head_->next;
    Node *curr = head_;

    while (oCurr != other.head_) {
        Block oData = oCurr->data;
        curr->next = new Node(oData);
        curr->next->next = head_;
        oCurr = oCurr->next;
        curr = curr->next;
    }
}
