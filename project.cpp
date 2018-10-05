#include <iostream>
#include <string.h>
#include <stack>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <queue>
#include <utility>
#include <string>
#include <set>
#include <map>
#define rep(i,n) for(int i = 0;i<(n);i++)
#define repf(i,a,b) for(int i = (a);i<=(b);i++)
#define repd(i,a,b) for(int i = (a);i>=(b);i--)
#define clr0(x) memset(x,0,sizeof(x))
#define clrf(x) memset(x,-1,sizeof(x))
#define clr1(x) memset(x,1,sizeof(x))
#define debug(x) cout << "\n--------\nDebug :\n"<<#x<<" : "<<x<<" \n--------\n"
#define Sz(x) (int)x.size()
#define Len(x) (int)x.length()
#define ADD_TYPE 1//链接
#define OR_TYPE 2//选择
#define CL_TYPE 3//闭包
#define START_TYPE 1//起始点
#define END_TYPE 2//终止点
#define DEBUG 
using namespace std;

//----------------全局变量-------------------
map<char,int>op_num_map;//字符映射关系，保证从1开始，0为串
map<int,char>op_num_rmap;//反向映射，调试用
int op_num_tot = 0;//终结字符数
int status_tot = 0;

void init()
{
	op_num_map.clear();
	op_num_rmap.clear();
	op_num_tot = 0;
	status_tot = 0;
	op_num_rmap[0] = '0';
}


//------------------------------------------- 



bool is_op_num(char now)//用于判断单个字符是操作运算符还是操作数
{
	if(now == '(' || now == ')' || now == '|' || now == '*' || now == '.')return false;
	return true;
}

int get_prov(char now)//返回每种字符的优先级 
{
	if(now == '.')return 1;
	if(now == '*')return 2;
	if(now == ')' || now == '(') return 3;
	if(now == '|')return 0;
} 
string add_c(string in_re)//给源字符串添加连接运算符顺便统计正则中的终结运算符 
{
	string ret = "";
	ret += in_re[0]; 
	int len = Len(in_re);
	int last_pos = 0;
	rep(i,len)if(is_op_num(in_re[i]) && (!op_num_map.count(in_re[i]))){
		op_num_map[in_re[i]] = ++op_num_tot;
		op_num_rmap[op_num_tot] = in_re[i];
	}
	repf(i,1,len-1){
		
		if(in_re[i] != '|' && in_re[i] != '*'){
			if(is_op_num(in_re[i]) && is_op_num(ret[last_pos])){
				ret += '.';
				last_pos ++; 
			}else if(in_re[i] == '(' && (ret[last_pos] == ')' || is_op_num(ret[last_pos]))){
				ret += '.';
				last_pos ++;
			}
			else if(is_op_num(in_re[i]) && (ret[last_pos] == ')' || ret[last_pos] == '*')){
				ret += '.';
				last_pos ++;
			}
		}
		ret += in_re[i];
		last_pos ++;
	}

	return ret;
	
}
string to_suffix(string in_re)
{
	string ret = "";
	//stack<char>op_num;
	stack<char>op_code;
	in_re = add_c(in_re);
	int len = Len(in_re);
	rep(i,len){
		if(is_op_num(in_re[i])){
			ret += in_re[i];
		}else if(in_re[i] != ')'){
			while(Sz(op_code) != 0 ){
				if(get_prov(op_code.top()) >= get_prov(in_re[i]) && op_code.top() != '(' ){
					char u = op_code.top();
					op_code.pop();
					ret += u;
				}else break;
			}
			op_code.push(in_re[i]);			
		}else if(in_re[i] == ')'){
			while(Sz(op_code) != 0){
				char u = op_code.top();
				op_code.pop();
				if(u == '(')break;
				ret += u;
			}
		}
	}
	while(!op_code.empty()){
		ret += op_code.top();
		op_code.pop();
	}
	return ret;
}

class node
{
	public:
		node(int type,int id){
			switch(type){
				case START_TYPE:
					is_start = true;
					is_end = false;
					break;
				case END_TYPE:
					is_start = false;
					is_end = true;
					break;
				default:
					is_start = false;
					is_end = false;
					break;
			}
			status_id = id;
		}
		~node(){
			int sz = Sz(nxt);
			rep(i,sz){
				delete nxt[i].first;
			}
			nxt.clear();
		}
		vector<pair<node *,int>>nxt;
		bool is_end;
		bool is_start;
		int status_id;
};

class Graph
{
	public:
		Graph(){}
		Graph(int u){
			//单个字符状态的转移
			start_node = new node(START_TYPE,++status_tot);
			node *e_n = new node(END_TYPE,++status_tot);
			start_node->nxt.push_back(make_pair(e_n,u));
			end_node.push_back(e_n);
		}
		Graph& g_cont(Graph *b){
			int sz = Sz(end_node);
			b->start_node->is_start = false;
			rep(i,sz){
				end_node[i]->is_end = false;
				end_node[i]->nxt.push_back(make_pair(b->start_node,0));
			}
			end_node = b->end_node;
			return *this;
		}
		Graph & g_or(Graph *b){
			node *new_start = new node(START_TYPE,++status_tot);
			node *new_end = new node(END_TYPE,++status_tot);
			start_node->is_start = false;
			b->start_node->is_start = false;
			new_start->nxt.push_back(make_pair(start_node,0));
			new_start->nxt.push_back(make_pair(b->start_node,0));
			int sz1 = Sz(b->end_node);
			int sz2 = Sz(end_node);
			rep(i,sz1){
				b->end_node[i]->is_end = false;
				b->end_node[i]->nxt.push_back(make_pair(new_end,0));
			}
			rep(i,sz2){
				end_node[i]->is_end = false;
				end_node[i]->nxt.push_back(make_pair(new_end,0));
			}
			start_node = new_start;
			end_node.clear();
			end_node.push_back(new_end);
			return *this;
		}
        Graph & g_cl(){
			node *new_start = new node(START_TYPE,++status_tot);
			node *new_end = new node(END_TYPE,++status_tot);
			start_node->is_start = false;
			int sz = Sz(end_node);
			rep(i,sz){
				end_node[i]->is_end = false;
				end_node[i]->nxt.push_back(make_pair(start_node,0));
				end_node[i]->nxt.push_back(make_pair(new_end,0));
			}
			new_start->nxt.push_back(make_pair(start_node,0));
			start_node = new_start;
			end_node.clear();
			end_node.push_back(new_end);
			return *this;
		}

		void debug_print(){
			queue<node *>Q;
			set<int>vis;
			node * now;
			Q.push(start_node);
			while(!Q.empty()){
				now = Q.front();
				Q.pop();
				
				cout << "当前节点编号 : "<<now->status_id<<endl;
				if(now->is_start)cout << "当前节点为起始节点"<<endl;
				if(now->is_end)cout << "当前节点为终止节点"<<endl;
				int sz = Sz(now->nxt);
				rep(i,sz){
					printf("now : %2d , next : %2d , edge_weight : %2c\n",now->status_id,now->nxt[i].first->status_id,op_num_rmap[now->nxt[i].second]);
					
					if(!vis.count(now->nxt[i].first->status_id)){
						vis.insert(now->nxt[i].first->status_id);
						Q.push(now->nxt[i].first);
					}
				}	
				cout << endl;
			}
		}
		
		node *start_node;
		vector<node*>end_node;
};

Graph * build_nfa(string suffix_re)
{
	stack<Graph*>op_num;
	int len = Len(suffix_re);
	rep(i,len){
		if(is_op_num(suffix_re[i])){
			
			op_num.push(new Graph(op_num_map[suffix_re[i]]));
		}else if(suffix_re[i] == '*'){
			if(op_num.size()<1)throw "Wrong RE!\n";
			Graph * tmp = op_num.top();
			op_num.pop();
			tmp->g_cl();
			//cout << "----------------------\n";
			//tmp->debug_print();
			op_num.push(tmp);		
		}else if(suffix_re[i] == '.'){
			if(op_num.size()<2)throw "Wrong RE!\n";
			Graph * tmp2 = op_num.top();op_num.pop();
			Graph * tmp1 = op_num.top();op_num.pop();
			//cout << "----------------------\n";
			//tmp1->debug_print();
			//cout << "----------------------\n";
			//tmp2->debug_print();
			tmp1->g_cont(tmp2);
			//cout << "----------------------\n";
			//tmp1->debug_print();
			op_num.push(tmp1);
		}else if(suffix_re[i] == '|'){
			if(op_num.size()<2)throw "Wrong RE!\n";
			Graph * tmp2 = op_num.top();op_num.pop();
			Graph * tmp1 = op_num.top();op_num.pop();
			tmp1->g_or(tmp2);
			//cout << "----------------------\n";
			//tmp1->debug_print();
			op_num.push(tmp1);
		}
	}
	if(op_num.size()>1)throw "Wrong RE\n";
	return op_num.top();
}
int main()
{
	#ifdef DEBUG
	//Graph *a = new Graph(1);
	//Graph *b = new Graph(2);
	////a->debug_print();
	////b->debug_print();	
	//a->g_or(b);
	//a->debug_print();
	#endif
	while(true)
	{
		init();
		string in_string;//输入字符串
		string in_re;//输入正则表达式
		cout << "输入字符串" << endl;
		cin >> in_string;
		cout << "请输入匹配模式" << endl; 
		cin >> in_re;
		try{
			string suffix_re = to_suffix(in_re);//将正则表达式专为后缀表达式
			#ifdef DEBUG
			cout << "原始正则："+in_re<<endl;
			cout << "后缀表达式："+suffix_re<<endl;
			#endif	
			
			//更具后缀表达式建立NFA
			int len = Len(suffix_re);
			Graph *suffix_nfa = build_nfa(suffix_re);
			#ifdef DEBUG
			suffix_nfa->debug_print();
			#endif

		}catch(const char *msg){
			cerr << msg << endl;
		}
	}
	

	
}