#ifndef _NODE_H_
#define _NODE_H_
#include ".\Init_Head.h"

class node
{
	public:
		node();
		node(int type,int id);
		~node();
		vector<pair<node *,int> >nxt;
		bool is_end;
		bool is_start;
		int status_id;
};
#endif

