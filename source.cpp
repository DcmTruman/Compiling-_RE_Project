#include "Node.h"
#include "Graph_Fa.h"
#include "Init_Head.h"

 
int main()
{

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
			Graph *suffix_fa = build_nfa(suffix_re);
			
			#ifdef DEBUG
			cout << "\n-------\nNFA:\n";
			suffix_fa->debug_print();
			#endif
			
			suffix_fa->to_dfa();
			
			#ifdef DEBUG
			cout << "\n-------\nDFA:\n";
			suffix_fa->debug_print();
			#endif
			
			suffix_fa->simplify_dfa();
			
			#ifdef DEBUG
			cout << "\n-------\nSimplified_DFA:\n";
			suffix_fa->debug_print();
			#endif
			
			if(suffix_fa->try_re(in_string)){
				cout << "输入字符串符合正则\n"; 
			}else{
				cout << "输入字符串不符合正则\n"; 
			}
		}catch(const char *msg){
			cerr << msg << endl;
		}
	}
	
}
