#include <iostream>
#include <string.h>
#include <stack>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <algorithm>
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
#define START_TPYE 1//起始点
#define END_TYPE 2//终止点
//#define DEBUG 
using namespace std;

//----------------全局变量-------------------
map<char,int>op_num_map;//字符映射关系，保证从1开始，0为串
int op_num_tot = 0;//终结字符数
int status_tot = 0;




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
	repf(i,1,len-1){
		if(is_op_num(in_re[i]) && (!op_num_map.count(in_re[i])))op_num_map[in_re[i]] = ++op_num_tot;
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
				case START_TPYE:
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
		Graph(node *now,int u){
			//单个字符状态的转移
			start_node = now;
			node *e_n = new node(END_TYPE,++status_tot);
			now->nxt.push_back(make_pair(e_n,u));
		}
		Graph& g_cont(Graph *b){
			int sz = Sz(end_node);
			b->start_node->is_start = false;
			rep(i,sz){
				end_node[i]->is_end = false;
				end_node[i]->nxt.push_back(make_pair(b->start_node,0));
			}
			return *this;
		}
		Graph & g_or(Graph *b){
			node *new_start = new node(START_TPYE,++status_tot);
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
		
		node *start_node;
		vector<node*>end_node;
};

int main()
{
	while(true)
	{
		string in_string;//输入字符串
		string in_re;//输入正则表达式
		cout << "输入字符串" << endl;
		cin >> in_string;
		cout << "请输入匹配模式" << endl; 
		cin >> in_re;
		try{
			string suffix_re = to_suffix(in_re);//将正则表达式专为后缀表达式
			#ifdef DEBUG
			debug(in_re);
			debug(suffix_re);
			#endif	
		}catch(const char *msg){
			cerr << msg << endl;
		}
	}
	

	
}

