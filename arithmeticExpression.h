#pragma once

#include <iostream>
#include <cstdlib>
#include <string>

using std::string;
using std::ofstream;

struct TreeNode {
    char data;
    char key;
    TreeNode* left;
    TreeNode* right;
    TreeNode(char data, char key): data(data), key(key), left(nullptr), right(nullptr) {}
};

class arithmeticExpression {
    private:
        string infixExpression;
        TreeNode* root;

        /* Helper function that returns an integer according to
       the priority of the given operator. */
        int priority(char key);

        /* Helper function that returns the postfix notation equivalent
        to the given infix expression */
        string infix_to_postfix();

        /* Helper function that outputs the infix notation of the arithmetic expression tree
        by performing the inorder traversal of the tree.
        An opening and closing parenthesis must be added at the 
        beginning and ending of each expression. */
        void infix(TreeNode* root);

        /* Helper function that outputs the prefix notation of the arithmetic expression tree
        by performing the preorder traversal of the tree. */
        void prefix(TreeNode* root);

        /* Helper function that outputs the postfix notation of the arithmetic expression tree
        by performing the postorder traversal of the tree. */
        void postfix(TreeNode* root);

        /* Helper function for generating the dotty file. This is a recursive function. */
        void visualizeTree(ofstream& outFS, TreeNode* root);

    public:
        /* Initializes an empty tree and sets the infixExpression
        to the value of parameter passed in. */
        arithmeticExpression(const string & value){
            infixExpression = value;
            root = nullptr;
        }

        //destructor
        ~arithmeticExpression(){
            destruct(root);
        }

        //recursively delete each node
        void destruct(TreeNode* node){
            if (!node){
                return;
            }
            destruct(node->left);
            destruct(node->right);
            delete node;
        }
        
        //rule of three
        arithmeticExpression(const arithmeticExpression&) = delete;
        arithmeticExpression& operator=(const arithmeticExpression&) = delete;

        /* Converts the infixExpression to its equivalent postfix string
        and then generates the tree and assigns the root node to the 
        root data field. The key for the first node is 'a', 'b' for the second node and so on. */
        void buildTree();

        /* Calls the recursive infix function. */
        void infix() { infix(root); }

        /* Calls the recursive prefix function. */
        void prefix() { prefix(root); }

        /* Calls the recursive postfix function. */
        void postfix() { postfix(root); }

        /* Calls the recursive visualizeTree function and generates the .png file using system call. */
        void visualizeTree(const string& outputFileName);
};