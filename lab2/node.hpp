#ifndef NODE_HPP
#define NODE_HPP

// class for a node for the priority queue

template<class T>
class Node {
    public:
        Node(T t, Node* l, Node* r) {
            task = t;
            left = l;
            right = r;
            npl = 0;
        }
        T task;
        Node* left;
        Node* right;
        int npl;

};
#endif