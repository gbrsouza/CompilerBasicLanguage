#include <iostream>
#include <string>
#include <vector>
#include <deque>

#include "parser.h"
#include "symbol.h"
#include "token.h"

using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::deque

using namespace std;

struct Node{
	Node *father;
	vector<Node*> children;
	symbol sym;
	string lexeme;
};

struct TikzNodeDef{
	string nodeType;
	string nodeId;
	string positioning;
	string nodeContent;
}

string astToTikz(Node *root){
	string ret = "\\begin{tikzpicture}[\nroundnode/.style={circle, draw=green!60, fill=green!5, very thick, minimum size=7mm},\nsquarednode/.style={rectangle, draw=red!60, fill=red!5, very thick, minimum size=5mm},\n]";

	vector<TikzNodeDef> nodeDefs;
	vector<TikzEdgeDef> edgeDefs;

	deque<Node*> q;
	q.push_back(root);

	while(!q.empty()){
		Node *u = q.front();
		q.pop_back();
		for(auto v : u->children){

		}
	}
	
	ret += "\\end{tikzpicture}";
}

int main(){
	cout << "\e\n";
}
