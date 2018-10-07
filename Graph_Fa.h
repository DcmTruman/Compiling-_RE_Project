#ifndef _GRAPH_FA_H_
#define _GRAPH_FA_H_
#include "Init_Head.h"
#include "Node.h"
void init();
bool is_op_num(char );
int get_prov(char);
string add_c(string);
string to_suffix(string);
class Graph
{
	public:
		Graph();
		Graph(node *u);
		Graph(int u);
		Graph& g_cont(Graph *b);
		Graph & g_or(Graph *b);
        Graph & g_cl();
		void debug_print();
		Graph & to_dfa(); 
		int get_all_node(vector<node*> & all_node);
		
		//判断某个点根据某个状态转移之后属于那个集合 
		set<set<node*> >::iterator  get_node_in_set(set<set<node*> > & all_set , node *now_node , int edge_type);
		Graph &  simplify_dfa();
		bool try_re(string in_string);
		node *start_node;
		vector<node*>end_node;
};
Graph * build_nfa(string suffix_re);
#endif

