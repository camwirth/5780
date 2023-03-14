#ifndef PRIORITY_HPP
#define PRIORITY_HPP

#include <iostream>
#include "node.hpp"
#include <string>
#include <stdlib.h>

template<class T>
class Priority {
    public:

        // create empty priority queue - set root to null
        Priority() {
            root = NULL;
        }

        // add a node to the priority queue
        void enqueue(T task) {
            // create a pointer to a new node
            Node<T>* newNode = new Node<T>(task, NULL, NULL);

            // check if the queue is empty
            if(root == NULL) {
                root = newNode;
            } else {
                // if queue is not empty, merge new node with the rest of the queue 
                root = merge(root, newNode);
            }
        }

        // remove the min priority from the priority queue 
        T dequeue() {
            // check if the queue is not empty
            if(!isEmpty()) {

                // create a temprorary pointer 
                Node<T>* temp = root;

                // get the task from the top of the priority queue
                T minTask = root->task;

                // merge the left and right trees together to remove the root node
                root = merge(root->left, root->right);

                // delete the memory for root node
                delete temp;

                //return the task from the root node 
                return minTask;
            }
            else {
                // print error if queue is empty 
                std::cout << "ERROR!! Cannot remove from an empty queue" << std::endl;
                exit(1);
            }
        }

        // return true if queue is empty 
        bool isEmpty() {
            return root == NULL;
        }

        // print queue for debugging purposes
        void printQueue() {
            printQueue(root, "");
            std::cout << "------------------------" << std::endl;
        }

    private:
        // node pointer is the root of the tree (most prioritized task)
        Node<T>* root;

        // function recursively prints the priority queue
        void printQueue(Node<T>* node, std::string spaces) {
            // if node is null, end of branch has been reached 
            if(node == NULL) {
                return;
            }

            // traverse through right side of tree (top)
            printQueue(node->right, spaces);
            // print info for current node
            std::cout << spaces << node->task << std::endl;
            // traverse through left side of tree (bottom)
            printQueue(node->left, spaces);

            // increment spaces to indicate heirarchy in tree
            spaces += "  ";
        }

        // get the null path length of a given node
        int getNpl(Node<T>* node) {
            // if node is null, there is no null path length (return -1)
            if(node == NULL) {
                return -1;
            }

            // return the null path length
            return node->npl;
        }

        // set the proper null path length for a given node
        void setNullPathLength(Node<T>* node) {
            // if either the left or right nodes are null the null path length is zero
            if(node->left == NULL || node->right == NULL) {
                node->npl = 0;
            } else {
                // otherwise the null path length is the right null path length plus 1
                node->npl = node->right->npl + 1;
            }
        }

        // swap children to keep shortest null path length on the right side of tree
        void swapChildren(Node<T>* node) {
            // create temp pointer and set to left side of tree
            Node<T>* temp;
            temp = node->left;

            // swap left to hold right tree
            node->left = node->right;
            // swap right to hold left tree
            node->right = temp;
        }

        // merge two priority trees together
        Node<T>* merge(Node<T>* t1, Node<T>* t2) {
            Node<T>* small;

            if(t1 == NULL) {
                return t2;
            }
            if(t2 == NULL) {
                return t1;
            }

            // overload < operator
            if(t1->task < t2->task) {
                t1->right = merge(t1->right, t2);
                small = t1;
            } else {
                t2->right = merge(t2->right, t1);
                small = t2;
            }

            if (getNpl(small->left) < getNpl(small->right)) {
                swapChildren(small);
            }

            setNullPathLength(small);

            return small;
        }

};

#endif