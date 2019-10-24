#include <iostream>
#include <queue>
#include <wchar.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <map>
#include <stack>
#include <algorithm>

using namespace std;

class TreeNode{
public:
	TreeNode *left = nullptr;
    	TreeNode *right = nullptr;

    	char value = '*';
    	int frequency = -1;
	string code = "";

    	bool operator<(const TreeNode &other){
        	if(frequency < other.frequency){
            		return true;
        	}else if(frequency > other.frequency){
            		return false;
        	}else if(value < other.value){
           		return true;
        	}else if (left && other.left){
			return *left < *(other.left);
		} else {
			return false;
		}
    	}

    	void preorder(){
        	cout << value << "[" << frequency << "] ";
        	if(left) left->preorder();
        	if(right) right->preorder();
    	}

	void inorder() {
		if (left) left->inorder();
		cout << value <<  "[" << frequency << "] ";
		if (right) right->inorder();
	}
	
	void postorder() {
		if (left) left->postorder();
		if (right) right->postorder();
		cout << value << "[" << frequency << "] ";
	
	}
	~TreeNode() {
		delete left;
		left = nullptr;
		delete right;
		right  = nullptr;
	
	}
};

TreeNode *merge(TreeNode* t1, TreeNode* t2){
    	// Return a new node with t1 and t2 as children
    	TreeNode* output = new TreeNode();
	
    	output->frequency = t1->frequency + t2->frequency;
    	output->left = t1;
   	output->right = t2;

    	return output;
}

struct Cols {
	int freq;
	string code;
	int bits;
};

struct LessByFreq {
	
	bool operator() (TreeNode * t1, TreeNode * t2) const {
		return !(*t1 < *t2);
	
	}

};

void encode(TreeNode * root, unordered_map<char, Cols> &table) {

	//Encode the string
	queue<TreeNode *> subtrees;

	subtrees.push(root);

	while(!subtrees.empty()) {
		TreeNode * curr = subtrees.front();
		subtrees.pop();
	
		if (curr->left) {
			curr->left->code = curr->code + "0";
			subtrees.push(curr->left);
		}

		if (curr->right) {
			curr->right->code = curr->code + "1";
			subtrees.push(curr->right);
		}

		if (!curr->left && !curr->right) {
			table[curr->value].code = curr->code;
		}
	}

}

int main()
{
	string line;
    	getline(cin, line);

    	// Build frequency table
   	unordered_map<char, Cols> table;
	
	//Add each character to the freq table and update freq
	for (char c : line) {
		table[c].freq += 1;
	}

   	// Create the tree
	//Creating priority queue
	priority_queue<TreeNode* , vector<TreeNode*>, LessByFreq> nodes;

	//Copy from map to queue
	for (auto row : table) {
		TreeNode* node = new TreeNode();
		node->value = row.first;
		node->frequency = row.second.freq;
		nodes.push(node);
	}
	
	//Creating tree
	TreeNode * t1 = nullptr;
	TreeNode * t2 = nullptr;
	
	while(nodes.size() > 1) {
		t1 = nodes.top();
			
		nodes.pop();

		t2 = nodes.top();
			
		nodes.pop();
		//cout << t1->value << " : " << t1->frequency << " -- " << t2->value << " : " << t2->frequency << endl; 

		nodes.push(merge(t1, t2));
	}	
	
	//Encode string
	TreeNode * root = nodes.top();
	encode(root, table);

	//Updating the table
	int nt_bits = 0;
	int ot_bits = 0;
	string encoding = "";

	for (auto row : table) {
		row.second.bits = row.second.code.size();
		nt_bits += row.second.bits * row.second.freq;
		ot_bits += 8 * row.second.freq;	
	}

	for (char i : line) {
		encoding += table[i].code;
	
	}
	
	
	
	//Output the encoding
	for (auto row : table) {
		cout << row.first << " ";
	}

	cout << endl;

	for (auto row : table) {
		cout << row.second.freq << " ";
	}

	cout << endl;	

	cout << encoding << endl;

	cout << "Total Bits (Original):" << ot_bits << endl;
	cout << "Total Bits (Coded):" << nt_bits << endl;
	//nodes.top()->preorder(); cout << endl;
	return 0;
}
