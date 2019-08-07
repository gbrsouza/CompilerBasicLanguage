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
using std::deque;

struct Node{
	Node *father;
	vector<Node*> children;
	symbol sym;
	string lexeme;
};

struct TikzNodeDef{
	//								 nodeType			nodeId			 	positioning			  nodeContent
	// sample node definition: \node[roundnode]        (uppercircle)       [above=of maintopic]  {     1     };
	string nodeType;
	string nodeId;
	string positioning;
	string nodeContent;
	TikzNodeDef(string nt, string nid, string p, string nc) : 
		nodeType(nt), 
		nodeId(nid), 
		positioning(p), 
		nodeContent(nc)
	{
	}
};

struct TikzEdgeDef{
	//									  idFirst     regionFirst	   idSecond  regionSecond
	// sample edge definition: \draw[->] (uppercircle.south)   --     (maintopic.north);
	string idFirst;
	string regionFirst;
	string idSecond;
	string regionSecond;
	TikzEdgeDef(string i1, string r1, string i2, string r2) : 
		idFirst(i1), 
		regionFirst(r1), 
		idSecond(i2), 
		regionSecond(r2)
	{
	}
};

string symbol_to_string(symbol s){
	return "TODO";
}

string astToTikz(Node *root){
	string ret = "\\begin{tikzpicture}[\nterminalNode/.style={circle, draw=black!60, fill=black!5, very thick, minimum size=7mm},\nnonterminalNode/.style={rectangle, draw=blue!60, fill=blue!5, very thick, minimum size=5mm},\n]";

	vector<TikzNodeDef> nodeDefs;
	vector<TikzEdgeDef> edgeDefs;

	q.push_back(make_pair(std::to_string(nodeDefs.size()), root));

	nodeDefs.push_back(TikzNodeDef(
							root->children.empty()? "terminalNode" : "nonterminalNode", 
							"0", 
							"", 
							root->lexeme == "" ? symbol_to_string(root->sym) : root->lexeme));

	deque<pair<string, Node*> > q;

	while(!q.empty()){
		Node *u = q.front().second;
		string fatherId = q.front().first;

		q.pop_back();

		for(auto v : u->children){
			q.push_back(make_pair(std::to_string(nodeDefs.size()), v));

			TikzNodeDef def = TikzNodeDef(
						u->children.empty() ? "terminalNode" : "nonterminalNode", 
						std::to_string(nodeDefs.size()), 
						"below=of " + fatherId, 
						root->lexeme == "" ? symbol_to_string(root->sym) : root->lexeme
					);
			
			
			edgeDefs.push_back(TikzEdgeDef(
				fatherId,
				"south",
				std::to_string(nodeDefs.size()),
				"north"
			));

			nodeDefs.push_back(def);
		}
	}
	
	ret += "%Nodes\n";

	for(auto ndef : nodeDefs){
		ret += "\\node[" + ndef.nodeType + "]\t\t(" + ndef.nodeId + ")\t\t[" + ndef.positioning + "]\t\t{"+ ndef.nodeContent +"};";
	}

	ret += "%Edges\n";

	for(auto edef : nodeDefs){
		ret += "\\draw[->] (" + edef.idFirst + "." + edef.regionFirst + ")\t\t--\t\t(" + edef.idSecond + "." + edef.regionSecond + ");";
	}

	ret += "\\end{tikzpicture}\n";
}

int main(){
	
}
