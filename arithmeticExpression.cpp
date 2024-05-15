#include "arithmeticExpression.h"
#include <stack>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

//determine priority of operator
int arithmeticExpression::priority(char op){
    int priority = 0;
    if(op == '('){
        priority =  3;
    }
    else if(op == '*' || op == '/'){
        priority = 2;
    }
    else if(op == '+' || op == '-'){
        priority = 1;
    }
    return priority;
}

//infix expression to postfix
string arithmeticExpression::infix_to_postfix(){
    if (infixExpression.empty()){
        throw runtime_error("Infix expression is empty.");
    }
    stack<char> s;
    ostringstream oss;
    char c;
    for (unsigned i = 0; i< infixExpression.size();++i){
        c = infixExpression.at(i);
        if(c == ' '){
            continue;
        }
        if(c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')'){ //c is an operator
            if( c == '('){
                s.push(c);
            }
            else if(c == ')'){
                while(s.top() != '('){
                    oss << s.top();
                    s.pop(); //remove (
                }
                s.pop();
            }
            else{
                while(!s.empty() && priority(c) <= priority(s.top())){
                    if(s.top() == '('){
                        break;
                    }
                    oss << s.top();
                    s.pop();
                }
                s.push(c);
            }
        }
        else{ //c is an operand
            oss << c;
        }
    }
    //pop remaining operators
    while(!s.empty()){
        oss << s.top();
        s.pop();
    }
    return oss.str();
}

//build expression tree from postfix expression
void arithmeticExpression::buildTree(){
    infixExpression = infix_to_postfix();

    stack<TreeNode*> nodeStack;
    char key = 'a';

    for (char token : infixExpression) {
        if (isalnum(token)) { // Operand
            TreeNode* newNode = new TreeNode(token, key++);
            nodeStack.push(newNode);
        } 
        else { // Operator
            TreeNode* newNode = new TreeNode(token, key++);

            // Pop two operands from the stack
            TreeNode* rightOperand = nodeStack.top();
            nodeStack.pop();
            TreeNode* leftOperand = nodeStack.top();
            nodeStack.pop();

            // Set left and right children of the new node
            newNode->left = leftOperand;
            newNode->right = rightOperand;

            // Push the new node back onto the stack
            nodeStack.push(newNode);
        }
    }

    if (nodeStack.size() != 1){
        throw runtime_error("Invalid size.");
    }

    // The final node remaining on the stack is the root of the expression tree
    root = nodeStack.top();
}

//infix notation print
void arithmeticExpression::infix(TreeNode* node){
    if (!node){
        return;
    }

    bool needParentheses = false;

    // Check if parentheses are needed
    if (node->left != nullptr || node->right != nullptr) {
        needParentheses = true;
    }

    // Output opening parenthesis if needed
    if (needParentheses) {
        cout << "(";
    }

    infix(node->left);
    cout << node->data;
    infix(node->right);

    // Output closing parenthesis if needed
    if (needParentheses) {
        cout << ")";
    }
}

//prefix notation print
void arithmeticExpression::prefix(TreeNode* node){
    if (!node){
        return;
    }

    cout << node->data;
    prefix(node->left);
    prefix(node->right);
}

//postfix notation print
void arithmeticExpression::postfix(TreeNode* node){
    if (!node){
        return;
    }

    postfix(node->left);
    postfix(node->right);
    cout << node->data;
}

//generate .dot file and .jpg image of tree
void arithmeticExpression::visualizeTree(const string &outputFilename){
    ofstream outFS(outputFilename.c_str());

    if (!outFS.is_open()){
        cout << "Error opening " << outputFilename <<endl;
        return;
    }

    outFS << "digraph G {" << endl;
    visualizeTree(outFS, root);
    outFS << "}";
    outFS.close();
    string jpgFilename = outputFilename.substr(0, outputFilename.size() - 4) + ".jpg";
    string command = "dot -Tjpg " + outputFilename + " -o " + jpgFilename;
    system(command.c_str());
}

//recursively output tree 
void arithmeticExpression::visualizeTree(ofstream& outFS, TreeNode* root){
    if (root == nullptr) {
        return;
    }

    outFS << root->key << " [label=\"" << root->data << "\"];" << endl;

    if (root->left != nullptr) {
        outFS << root->key << " -> " << root->left->key << ";" << endl;
        visualizeTree(outFS, root->left);
    }
    if (root->right != nullptr) {
        outFS << root->key << " -> " << root->right->key << ";" << endl;
        visualizeTree(outFS, root->right);
    }
}