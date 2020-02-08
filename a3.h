// a3.h

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : Chamodi Basnayake
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////


#include "cmpt_error.h"
#include <iostream>
#include <string>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
// Your task for this assignment is to implement all of the methods of the BST
// class given below, subject to a few important constraints.
//
// Here are the things your implementation **cannot** do:
//
// - You **cannot** use recursion anywhere in any of your code.
//
// - You **cannot** add new variables to the BST class, public or private. You
//   cannot add any global variables.
//
// - You **cannot** #include any other files. If you borrow code or ideas from
//   any other source, be sure to cite the source (e.g. as a comment in your
//   code).
//
// - You **cannot** use arrays or vectors or any other pre-made data
//   structure. As mentioned below, you may only use data structures that you
//   implement yourself.
//
// Here are the things your implementation **can** do:
//
// - You **can** add helper methods, either public or private, to BST. You may
//   also add helper functions outside of BST. Of course, any extra
//   methods/functions you add must follow the "cannot" rules above.
//
// - You **can** add helper classes, either in BST or outside of BST. For
//   instance, you could implement your own stack class.
//
// Put all the code necessary to implement BST correct inside a3.h, and then
// submit just this file when you are done. Put your testing code in a3.cpp.
//
// In the specification of the methods for BST, these variables are used:
//
// - n is the number of nodes in the BST
// - h is the height of the BST
// - T refers to the tree immediately before a method is called
// - T' refers to the tree immediately after the method is finished
//
///////////////////////////////////////////////////////////////////////////////

class BST {
private:
    struct Node {
        string value;
        Node* left;
        Node* right;
    };

    Node* root;

    class List_stack {
    private:
        struct stackNode {
            Node* node;
            stackNode* next;
        };

        stackNode* head;

    public:
        List_stack() :  head(nullptr) {}

        ~List_stack() {
            while (!is_empty()) pop();
        }   

        bool is_empty()const {
            return head == nullptr;
        }

        void push(Node* x) {
            head = new stackNode{x, head};
        }

        Node* pop() {
            if (is_empty()) cmpt::error("can't pop an empty stack");
            Node* result = head->node;
            stackNode* p = head->next;
            delete head;
            head = p;
            return result;
        }

        Node* top() const {
        if (is_empty()) cmpt::error("can't pop an empty stack");
        return head->node;       
        }

    };

    class List_queue {
    private:
        struct queueNode {
            Node* node;
            queueNode* next;
        };

        queueNode* cursor;
        int sz;


    public:
        List_queue() {
            cursor = nullptr;
            sz = 0;
        }

        ~List_queue() {
            while (!is_empty()) remove();
        }   

        bool is_empty() const {
            return cursor == nullptr;
        }

        int size() const {
            return sz;
        }

        void enqueue(Node* x) {
            queueNode* v = new queueNode{nullptr, nullptr};
            v->node = x;

            if (is_empty()) {
                v->next = v;
                cursor = v;
            } else {
                v->next = cursor->next;
                cursor->next = v;
            }

            cursor = cursor->next;
            sz++;
        }
        
        void remove() {
            if (is_empty()) cmpt::error("remove: list is empty");
            queueNode* old = cursor->next;
            if (old == cursor) {
                cursor = nullptr;
            } else {
                cursor->next = old->next;
            }
            delete old;
        }

        Node* dequeue() {
            if (is_empty()) cmpt::error ("dequeue: sequence empty");
            Node* result = front();
            remove();
            sz--;

            return result;
        }

        Node* front() const {
            if (is_empty()) cmpt::error ("front:queue is empty");
            return cursor->next->node;
        }

    };

public:
    // Pre-condition:
    //    none
    // Post-condition:
    //    constructs a new empty BST
    // Constraints:
    //    O(1) performance
    BST(){
        root = nullptr;
    }

    // Pre-condition:
    //    none
    // Post-condition:
    //    deletes all the nodes in this BST
    // Constraints:
    //    O(n) performance
    ~BST(){
        if (root == nullptr) {
            return;
        }
        //Use level order traversal to delete nodes from the tree
        List_queue queue;
        //Insert root into the queue
        queue.enqueue(root);

        //while queue is not empty, pop the front element, and push in the children of the popped element
        while (!queue.is_empty()) {
            Node* treeNode = queue.front();
            queue.dequeue();

            if (treeNode->left != nullptr) {
                queue.enqueue(treeNode->left);
            }
            if (treeNode->right != nullptr) {
                queue.enqueue(treeNode->right);
            }
            //delete the popped node
            delete treeNode;
        }
    }

    // Pre-condition:
    //    none
    // Post-condition:
    //    returns the number of string in this tree (i.e. n)
    // Constraints:
    //    worst-case O(n) performance; does not use recursion!
    int size() const {
        if (root == nullptr) {
            return 0;
        }

        //Create an object of queue type
        List_queue queue;

        queue.enqueue(root);
        Node* current = nullptr;
        int size = 0;

        while (!queue.is_empty()) {
            //Pop elements from the queue and increase size as each is poppe
            current = queue.front();
            queue.dequeue();
            size++;

            //Store the children of the popped element in the queue
            if (current->left !=nullptr) {
                queue.enqueue(current->left);
            }
            
            if (current->right != nullptr) {
                queue.enqueue(current->right);
            }
        }
        return size;
    }

    // Pre-condition:
    //    none
    // Post-condition:
    //    returns the height of this tree
    // Constraints:
    //    worst-case O(n) performance; does not use recursion!
    // Note:
    //    The height of a BST is the number of links in the longest
    //    root-to-leaf path in the tree. An empty tree is height 0,
    //    and a tree with a single node is also height 0.
    int height() const {
        if (root == nullptr) {
            return 0;
        }

        List_queue queue;

        queue.enqueue(root);
        Node* current = nullptr;
        int height = 0;

        while (!queue.is_empty()) {
            int sz = queue.size();

            //Run the while loop until the size of the quque is zero
            while (sz--) {
                current = queue.front();
                queue.dequeue();

                //Store the children of the popped element in the quque
                if(current->left != nullptr) {
                    queue.enqueue(current->left);
                }
                
                if(current->right != nullptr) {
                    queue.enqueue(current->right);
                }
            }
            height ++;   
        }
        //Since height is numbr of levels-1, decrease 1 from height calculated
        return height-1;
    }

    // Pre-condition:
    //    none
    // Post-condition:
    //    returns true if s is in this tree, and false otherwise
    // Constraints:
    //    worst-case O(h) performance; does not use recursion!
    bool contains(const string& s) const {
        Node* p = root;

        //Iterate through the tree until the node is found based on value of the keys
        while (p != nullptr) {
            if (s == p->value) {
                return true;
            } else if (s < p->value) {
                p = p->left;
            } else if (s > p->value) {
                p = p->right;
            }
        }
        //Returns false when node isn't found
        return false;
    }

    // Pre-condition:
    //    none
    // Post-condition:
    //    T'.contains(s)
    // Constraints:
    //    worst-case O(h) performance; does not use recursion!
    // Note:
    //    If s is already in T, then insert(s) does nothing.
    void insert(const string& s) {
        Node* p = root;

        //Don't do anything if the key already exists
        if (contains(s)) {
            return;
        }
        
        //Create a new node and place as root if the tree is empty
        if (p == nullptr) {
            Node* newNode = new Node{s, nullptr, nullptr};
            root = newNode;
        //Else, compare value of the key and use property of binary search tree to place it as a leaf    
        } else {
            while (p != nullptr) {
                if (s < p->value) {
                    
                    if (p->left == nullptr) {
                        Node* h = new Node{s, nullptr,nullptr};
                        p->left = h;
                        p = p->left->left; 
                    } else {
                        p = p->left;
                    }

                } else if (s > p->value) {
                    
                    if (p->right == nullptr) {
                        Node* h = new Node{s, nullptr,nullptr};
                        p->right = h;
                        p = p->right->right;
                        
                    } else {
                        p = p->right;
                    }
                }
            }
        }
    }
    


    // Pre-condition:
    //    none
    // Post-condition:
    //    !T'.contains(s)
    // Constraints:
    //    worst-case O(h) performance; does not use recursion!
    // Note:
    //    If s is not in T, then remove(s) does nothing.
    void remove(const string& s){
        if (root == nullptr){
            cout << "Root is null" << endl;
            return;
        }

        //Create Node* p to iterate through the tree, create a parent node to keep track of the parents
        Node* p = root;
        Node* parent = nullptr;

        while (p != nullptr) {
            if (s == p->value) {
                //If node has no children, proceed
                if (p->left == nullptr && p->right == nullptr) {
                    //If the node was the root, proceed
                    if (parent == nullptr) {
                        delete root;
                        root = nullptr;
                        break;
                    } else {
                        //Use parent node to delete node from the tree
                        if (parent->value > s) {
                            parent->left = nullptr;
                            delete p;
                            p = nullptr;
                        } else if(parent->value < s) {
                            parent->right = nullptr;
                            delete p;
                            p = nullptr;
                        }
                    }
                //If the node only has a left child, proceed
                } else if (p->left != nullptr && p->right == nullptr) {
                    //Check if the node is the root
                    if (parent == nullptr) {
                        Node* temp = p->left;
                        root = temp;
                        delete p;
                        break;
                    //Node isn't the root    
                    } else {
                        //Use parent node to remove node from the tree
                        if (p->value < parent->value) {
                            Node* temp = p->left;
                            parent->left = temp;
                        } else {
                            Node* temp = p->left;
                            parent->right = temp;
                        }
                        delete p;
                        break;
                    }
                //If the Node has a right child
                } else if (p->left == nullptr && p->right != nullptr) {
                    //Check if the node is the root
                    if (parent == nullptr) {
                        Node*temp = p->right;
                        root = temp;
                        delete p;
                        break;
                    //Else use parent node to delete th node from the tree
                    } else {
                        if (p->value < parent->value) {
                            Node* temp = p->right;
                            parent->left = temp;

                        } else {
                            Node* temp = p->right;
                            parent->right = temp;
                        }
                        delete p;
                        break;
                    }
                
                //If the node has both right and left children
                } else if (p->left != nullptr && p->right != nullptr) {
                    //Create node temp, that's pointing to left child of p, in order to find max of left subtree
                    Node* temp = p->left;

                    //If node has no grandchildren
                    if (temp->right == nullptr && temp->left == nullptr) {
                        p->value = temp->value;
                        delete p->left;
                        p->left = nullptr;

                    //If p has a left grandchild
                    } else if (temp->left != nullptr && temp->right == nullptr) {
                        p->value = temp->value;
                        p->left = temp->left;
                        delete temp;
                        break;
                    
                    //If p has a right grandchild
                    }else if (temp->right != nullptr){
                        //Set p to be parent to start off
                        parent = p;
                        //Iterate through the left subtree to find the max value, set temp to point to it
                        while(temp->right != nullptr){
                            parent = temp;
                            temp = temp->right;
                        }

                        //Replace value of node p to be the value at temp (max of left subtree)
                        p->value = temp->value;

                        //Check if temp has a left child, if so replace temp value by temp->left value and delete temp->left
                        if(temp->left != nullptr){
                            temp->value = temp->left->value;
                            delete temp->left;
                            temp->left = nullptr;
                        //If not, delete what temp points to   
                        }else{
                            delete parent->right;
                            parent->right = nullptr; 
                        }   
                    } 
                }
            //If s is smaller than p, then go to left subtree. Tag parent with each iteration    
            } else if (s < p->value) {
                parent = p;
                p = p->left;
            
            //If s is larger than p, then go to right subtree. Tag parent with each iteration    
            } else if (s > p->value) {
                parent = p;
                p = p->right;
            }
        }
    }


    // Pre-condition:
    //    none
    // Post-condition:
    //    prints all the strings in T to cout in pre-order order
    // Constraints:
    //    worst-case O(n) performance; does not use recursion!
    void print_preorder() const{
        if (root == nullptr) {
            cout << "tree is empty" << endl;
            return;
        }
        //Create a stack and push root inside
        List_stack stack;
        stack.push(root);

        //While the stack is not empty, pop the last element in, and cout. 
        //Proceed to push it's children into stack and iterate
        while (!stack.is_empty()) {
            Node* n = stack.pop();
            cout << n->value << " ";

            if (n->right != nullptr) {
                stack.push(n->right);
            }
            if (n->left != nullptr) {
                stack.push(n->left);
            }
        }
        cout << endl;
    }

    // Pre-condition:
    //    none
    // Post-condition:
    //    prints all the strings in T to cout in in-order order
    //    (i.e. alphabetical order)
    // Constraints:
    //    worst-case O(n) performance; does not use recursion!
    void print_inorder() const{
        if(root == nullptr){
            cout << "tree is empty" << endl;
            return;
        }

        //Create a stack
        List_stack stack;
        Node*current = root;
        
        //Run while both stack isn't empty and the current node it's nullptr
        while ((!stack.is_empty()) || current != nullptr) {
            //Until current reaches nullptr, keep pushing left tree's elements in
            if (current != nullptr) {
                stack.push(current);
                current = current->left;
            
            //When current reaches nullptr, set current to element on top of stack and set current to next right    
            } else {
                current = stack.top();
                stack.pop();
                cout << current->value << " ";
                current = current->right;
            }
        }
        cout << endl;
    }

    // Pre-condition:
    //    none
    // Post-condition:
    //    prints all the strings in T to cout in post-order order
    // Constraints:
    //    worst-case O(n) performance; does not use recursion!
    void print_postorder() const{
        if(root == nullptr){
            cout << "tree is empty" << endl;
            return;
        }

        //Create two stacks. 
        //StackOne: push root in, and pops the top element (this element stored in StackTwo).
        //StackOne stores children of the popped element. 
        List_stack stackOne;
        List_stack stackTwo;
        stackOne.push(root);

        while (!stackOne.is_empty()) {
            Node* current = stackOne.top();
            stackTwo.push(current);
            stackOne.pop();

            if (current->left !=nullptr) {
                stackOne.push(current->left);
            }
            if (current->right != nullptr) {
                stackOne.push(current->right);
            }
        }

        //StackTwo consists of popped elements from stackOne. Pop them to cout in post-order.
        while (!stackTwo.is_empty()) {
            cout << stackTwo.top()->value << " ";
            stackTwo.pop();
        }
        cout << endl;
    }
}; // class BST



////////////////////////////////CITATIONS//////////////////////////////
/*
 - Height of the tree: Mainly referred to psedo code provided by the professor in class. Directly took the idea of implementing while (sz--) loop from following website:
    http://www.techiedelight.com/calculate-height-binary-tree-iterative-recursive/
    https://www.geeksforgeeks.org/iterative-method-to-find-height-of-binary-tree/

 - Size of the tree: Mainly used class notes provided in order to implement. Pseudo code was provided by professor


 - BST Destructor: Got the idea to use a traversal to create the destructor. I used level order traversal in the end.
    https://stackoverflow.com/questions/16258213/non-recursive-destructor-for-binary-search-tree-by-implementing-stack
    https://www.geeksforgeeks.org/print-level-order-traversal-line-line/


 - Preorder Traversal: Mainly used class notes (professor wrote pseudo code). Referred to following.
    https://www.geeksforgeeks.org/iterative-preorder-traversal/
    https://www.ideserve.co.in/learn/iterative-preorder-traversal-of-binary-tree

 - Inorder Traversal: referred to following websites on how to implement this. Used class notes as reference
    http://www.techiedelight.com/inorder-tree-traversal-iterative-recursive/
    https://www.geeksforgeeks.org/iterative-preorder-traversal/

 - Postorder Traversal: got the idea to use two stacks from the following website. Used class notes as reference
        https://www.geeksforgeeks.org/iterative-postorder-traversal/
        https://www.geeksforgeeks.org/iterative-postorder-traversal-using-stack/
        Got the idea on how to code it using the class notes provided by the professor

 -Stack and queue classes - Directly taken from professor's notes posted on the website 

*/

