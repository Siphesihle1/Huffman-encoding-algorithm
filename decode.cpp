#include <iostream>
#include <queue>
#include<wchar.h>
#include<stdio.h>
#include <string>
#include <unordered_map>
#include <stack>
#include<locale>
#include <codecvt>
#include <sstream>
#include <algorithm>

using namespace std;

class TreeNode{
public:
	TreeNode *left = nullptr;
    	TreeNode *right = nullptr;

    	char value = '*';
    	int frequency = -1;
    	
	bool operator<(const TreeNode &other){
        	if(frequency < other.frequency){
            		return true;
        	}else if(frequency > other.frequency){
            		return false;
        	}else if(value < other.value){
           		return true;
		}else if (left && other.left){
			return *left < *other.left;
		}else {
            		return false;
        	}
    	}

    	void preorder(){
        	cout << value << "[" << frequency << "] ";
        	if(left) left->preorder();
        	if(right) right->preorder();
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
	
    	//Assuming that *t1 has a lower frequencey than *t2
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

string r_space(string line) {
	
	string n_line = "";
	for (int i = 0; i < line.size(); i+=2) {
		n_line += line[i];
	}
	return n_line;

}

vector<int> c_freq(string line) {
	stringstream ss(line);
	int freq;
	vector<int> freqs;

	while(ss >> freq) {
		freqs.push_back(freq);
	}

	return freqs;

}

string decode(TreeNode * root , string encoding) {

	//Decode the string
	TreeNode * subtree = root;
	string d_sent = "";
	for (char i : encoding) {
		if (i == '0' && subtree->left) {
			subtree = subtree->left;
			if (!subtree->left && !subtree->right) {
				d_sent += subtree->value;
				subtree = root;
			}
		} else if (i == '1' && subtree->right) {
			subtree = subtree->right;
			if (!subtree->left && !subtree->right) {
				d_sent += subtree->value;
				subtree = root;
			}
		}
	}

	return d_sent;


}

int main()
{
	//Getting input from stdin
	string line1, line2, line3;
    	getline(cin, line1);
	getline(cin, line2);
	getline(cin, line3);
	

	//Prepare the input
	line1 = r_space(line1); 
	vector<int> freqs = c_freq(line2);	
	
	//Creating the table	
	unordered_map<char, int> freq_table;

	for(int i = 0; i < line1.size();i++) {
		freq_table[line1[i]] = freqs[i];
	}

	//Initialising the priority queue
	priority_queue<TreeNode* , vector<TreeNode*>, LessByFreq> nodes;
	
	for (auto row : freq_table) {
		TreeNode * node = new TreeNode();
		node->value = row.first;
		node->frequency = row.second;
		nodes.push(node);
	}

	//Creating the tree
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
	
	//Decoding the string
	TreeNode * root = nodes.top();
	cout << decode(root, line3) << endl;	
	//nodes.top()->preorder(); cout << endl;
	//cout << d_sent <<endl;
	
	return 0;
}
