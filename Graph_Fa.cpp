#include "Graph_Fa.h"
#include "Node.h"
#include "Init_Head.h"

//------------全局变量----------------------- 
map<char,int>op_num_map;//字符映射关系，保证从1开始，0为串
map<int,char>op_num_rmap;//反向映射，调试用 
int op_num_tot = 0;//终结字符数
int status_tot = 0;//用来节点标识ID 
vector<int>op_num_vec;//记录所有终结字符 
void init()
{
	op_num_map.clear();
	op_num_rmap.clear();
	op_num_vec.clear();
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
		op_num_vec.push_back(op_num_tot);
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

Graph::Graph(){}
Graph::Graph(node *u)
{
	start_node = u;
}
Graph::Graph(int u)
{
	//单个字符状态的转移
	start_node = new node(START_TYPE,++status_tot);
	node *e_n = new node(END_TYPE,++status_tot);
	start_node->nxt.push_back(make_pair(e_n,u));
	end_node.push_back(e_n);
}
Graph& Graph::g_cont(Graph *b)
{
	int sz = Sz(end_node);
	b->start_node->is_start = false;
	rep(i,sz){
		end_node[i]->is_end = false;
		end_node[i]->nxt.push_back(make_pair(b->start_node,0));
	}
	end_node = b->end_node;
	return *this;
}

Graph & Graph::g_or(Graph *b)
{
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

Graph & Graph::g_cl()
{
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

void Graph::debug_print()
{
	queue<node *>Q;
	set<int>vis;
	node * now;
	Q.push(start_node);
	vis.insert(start_node->status_id);
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
        
Graph & Graph::to_dfa()
{
	//BFS求e_c(0);
	set<node *>e_c0;
	map<set<node *> , node* >set_to_node;//用于对set进行映射，建立新的节点用 
	map<int,set<node *> > int_to_set;//int 对 set 进行映射 
	//map<set <node *> , int>set_to_int;
	queue<node *>Q;
	Q.push(start_node);
	e_c0.insert(start_node);
	//先用BFS求所有只经过空串不消耗字符就能到达的节点
	while(!Q.empty()){
		node *now = Q.front();
		Q.pop();
		int sz = Sz(now->nxt);
		rep(i,sz){
			if(now->nxt[i].second == 0 && (!e_c0.count(now->nxt[i].first))){
				e_c0.insert(now->nxt[i].first);
				Q.push(now->nxt[i].first);
			}
		}
	}
	node *new_start = new node(START_TYPE,++status_tot);
	vector<node*>new_end_node;
	set_to_node[e_c0] = new_start;
	int_to_set[status_tot] = e_c0;
	queue<int>Qi;
	Qi.push(status_tot);
	bool flag = false;
	while(!Qi.empty()){
	//	cout << "fuck"<< endl;
		int u = Qi.front();
		Qi.pop();
		//set<node *>next;
		node *now_node = set_to_node[int_to_set[u]];
		int sz = Sz(op_num_vec);
		rep(i,sz){
			set<node *>next_set;
		//	set<node *>vis_n;
			flag = false;
			//cout << endl;
			for(set<node*>::iterator it = int_to_set[u].begin();it != int_to_set[u].end();it++){
				//*it是一个node指针
				//cout << (*it)->status_id << " "; 
				//int szz = Sz((*it)->nxt);
				//flag = false;
			
				queue<pair<node*,int> >move_q;
			//	next_set.insert((*it));
				move_q.push(make_pair((*it),1));
				set<node *>vis_n;
				while(!move_q.empty())
				{
					//cout << "fuck" << endl;
					node * move_node = move_q.front().first;
					int time = move_q.front().second;
					int szz = Sz(move_node->nxt);
					move_q.pop();
					rep(j,szz){
						if(move_node->nxt[j].second == op_num_vec[i] && time == 1 ){
							next_set.insert(move_node->nxt[j].first);
							vis_n.insert(move_node->nxt[j].first);
							if(move_node->nxt[j].first->is_end)flag = true;
							move_q.push(make_pair(move_node->nxt[j].first,0));
						}
						else if(move_node->nxt[j].second == 0 && (!vis_n.count(move_node->nxt[j].first))){
							vis_n.insert(move_node->nxt[j].first);
							if(time == 0){
								//此时已经消耗类一个匹配字母，并根据空字母匹配到达此状态 
								next_set.insert(move_node->nxt[j].first);
								if(move_node->nxt[j].first->is_end)flag = true;
							}
							move_q.push(make_pair(move_node->nxt[j].first,time));	
						}
					}
				}
			}
			if(!set_to_node.count(next_set)){
				node *temp  = new node();
				if(!flag)temp = new node(0,++status_tot);
				else {
				//	cout << "\nfuck\n";
					temp = new node(END_TYPE,++status_tot);
					new_end_node.push_back(temp);
				}
				now_node->nxt.push_back(make_pair(temp,op_num_vec[i]));
				set_to_node[next_set] = temp;
				int_to_set[status_tot] = next_set;
			//	set_to_int[next_set] = status_tot;
				Qi.push(status_tot);
			}else{
				node *temp = set_to_node[next_set];
				now_node->nxt.push_back(make_pair(temp,op_num_vec[i]));
			}
			#ifdef DEBUG
//					cout << "当前节点："<<now_node->status_id<<endl;
//					cout << "当前节点集合" << endl;
//					for(set<node*>::iterator it = int_to_set[now_node->status_id].begin();it != int_to_set[now_node->status_id].end();it++){
//						cout << (*it)->status_id<<" ";
//					}
//					cout << endl;
//					cout << "转移边："<<op_num_rmap[op_num_vec[i]]<<endl;
//					cout << "转移集合节点：";
//					for(set<node*>::iterator it = next_set.begin();it != next_set.end();it++){
//						cout << (*it)->status_id<<" ";
//					}
//					cout << endl;
			#endif
		}
	//d	cout << "fuck" << endl;
		
	}
	start_node = new_start;
	end_node = new_end_node;
	return *this;		 
}

int Graph::get_all_node(vector<node*> & all_node)
{
	all_node.clear();
	queue<node *>Q;
	set<int>vis;
	node * now;
	Q.push(start_node);
	all_node.push_back(start_node);
	vis.insert(start_node->status_id);
	while(!Q.empty()){
		now = Q.front();
		Q.pop();
		int sz = Sz(now->nxt);
		rep(i,sz){
			if(!vis.count(now->nxt[i].first->status_id)){
				all_node.push_back(now->nxt[i].first);
				vis.insert(now->nxt[i].first->status_id);
				Q.push(now->nxt[i].first);
			}
		}	
	}
	return Sz(all_node);
}

set<set<node*> >::iterator  Graph::get_node_in_set(set<set<node*> > & all_set , node *now_node , int edge_type)
{
	int sz = Sz(now_node->nxt);
	rep(i,sz){
		if(now_node->nxt[i].second == edge_type){
			node *nxt_node = now_node->nxt[i].first;
			for(set<set<node*> >::iterator it = all_set.begin();it != all_set.end();it++){
				if((*it).count(nxt_node)){
					return it;
				}
			}
			//break;
		}
	}
}
Graph &  Graph::simplify_dfa()
{
	
	vector< node * >all_node;
	set< set<node*> >all_set;
	int sz = get_all_node(all_node);
	set<node *>end_set;
	set<node *>not_end_set;
	rep(i,sz){
		if(all_node[i]->is_end)end_set.insert(all_node[i]);
		else not_end_set.insert(all_node[i]);
	}
	all_set.insert(end_set);
	all_set.insert(not_end_set);
	bool same_node = false;//用来标记集合中的点经过转移之后所属于的集合是否等价
	int set_size = -1;
	//cout << "fuck"<<all_set.size();
	while(true){
		 if(set_size == all_set.size())break;
		 else set_size = all_set.size();
		 vector<set<node *> >set_vec;//用来存储当前set可以被划分成哪些set 
		 set<node*>vis;//记忆化降低复杂度至O(n) 
		 for(set< set<node*> >::iterator it_set = all_set.begin();it_set != all_set.end();it_set++){
		 	
		 	for(set<node*>::iterator i_node = (*it_set).begin();i_node != (*it_set).end();i_node++){
		 		if(vis.count((*i_node)))continue;
		 		vis.insert((*i_node));
		 		int set_pos = Sz(set_vec);
		 		set<node *>temp_set;
		 		temp_set.insert((*i_node));
		 		set_vec.push_back(temp_set);
		 		for(set<node*>::iterator j_node = (*it_set).begin();j_node != (*it_set).end();j_node++){
		 			if(i_node == j_node)continue;
		 			same_node = true;
		 			rep(i,Sz(op_num_vec)){
		 				if(get_node_in_set(all_set,(*i_node),op_num_vec[i]) != get_node_in_set(all_set,(*j_node),op_num_vec[i]))
						 {
		 					same_node = false;
		 					break;
						 }
					}
					if(same_node){
						vis.insert((*j_node));
						set_vec[set_pos].insert((*j_node));
					}
				}
			}
		 }
		 all_set.clear();
		 //cout << set_vec.size() << "fuck"<<endl;
		 rep(i,Sz(set_vec)){
		 	all_set.insert(set_vec[i]);
		 }
	}
	
	#ifdef DEBUG
//			cout << "size : " << Sz(all_set) << endl;
//			for(set< set<node*> >::iterator it_set = all_set.begin();it_set != all_set.end();it_set++){
//				for(set<node*>::iterator i_node = (*it_set).begin();i_node != (*it_set).end();i_node++){
//					cout << (*i_node)->status_id << " ";
//				}
//				cout << endl;
//			}	
	#endif
	
	map<node *,set<node*> >node_to_set;//记录每个点对应的集合
	map<set<node*>,node *>set_to_newnode;
	node *new_start = new node(START_TYPE,++status_tot);
	vector<node*>new_end_node;
	for(set< set<node*> >::iterator it_set = all_set.begin();it_set != all_set.end();it_set++){
		if((*it_set).count(start_node)){
			set_to_newnode[*it_set] = new_start;
		}else if((*((*it_set).begin()))->is_end ){
			node *temp_node = new node(END_TYPE,++status_tot);
			new_end_node.push_back(temp_node);
			set_to_newnode[*it_set] = temp_node;
		}else{
			node *temp_node = new node(0,++status_tot);
			set_to_newnode[*it_set] = temp_node;
		}
		
		for(set<node*>::iterator i_node = (*it_set).begin();i_node != (*it_set).end();i_node++){
			node_to_set[(*i_node)] = *it_set;
		}
	}
	
	rep(i,sz){
		rep(j,Sz(all_node[i]->nxt)){
			node *nxt = set_to_newnode[node_to_set[all_node[i]->nxt[j].first]];
			int type = all_node[i]->nxt[j].second;
			bool flag = true;
			//此处防止重边 

			if(find(set_to_newnode[node_to_set[all_node[i]]]->nxt.begin(),set_to_newnode[node_to_set[all_node[i]]]->nxt.end(),make_pair(nxt,type))==set_to_newnode[node_to_set[all_node[i]]]->nxt.end())
			{
				set_to_newnode[node_to_set[all_node[i]]]->nxt.push_back(make_pair(nxt,type));
			}
			
		}
	}
	start_node = new_start;
	end_node = new_end_node;

	return *this;
}

bool Graph::try_re(string in_string)
{
	int len = Len(in_string);
	node *now = start_node;
	rep(i,len){
		if(!op_num_map.count(in_string[i]))return false;//匹配字符串中出现了正则中未出现的终结字符 
		rep(j,Sz(now->nxt)){
			if(now->nxt[j].second == op_num_map[in_string[i]]){
				now = now->nxt[j].first;
				break;
			}
		}
	}
	if(now->is_end)return true;
	else return false;
}


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
		}else{
			throw "Wrong RE!\n";
		}
	}
	if(op_num.size()!=1)throw "Wrong RE\n";
	return op_num.top();
}

