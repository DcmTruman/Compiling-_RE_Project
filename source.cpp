#include "Node.h"
#include "Graph_Fa.h"
#include "Init_Head.h"

 
int main()
{

	while(true)
	{
		init();
		string in_string;//�����ַ���
		string in_re;//����������ʽ
		cout << "�����ַ���" << endl;
		cin >> in_string;
		cout << "������ƥ��ģʽ" << endl; 
		cin >> in_re;
		try{
			string suffix_re = to_suffix(in_re);//��������ʽרΪ��׺���ʽ
			#ifdef DEBUG
			cout << "ԭʼ����"+in_re<<endl;
			cout << "��׺���ʽ��"+suffix_re<<endl;
			#endif	
			
			//���ߺ�׺���ʽ����NFA
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
				cout << "�����ַ�����������\n"; 
			}else{
				cout << "�����ַ�������������\n"; 
			}
		}catch(const char *msg){
			cerr << msg << endl;
		}
	}
	
}
