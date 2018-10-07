#include ".\Node.h"
#include ".\Init_Head.h"

node::node(){}
node::node(int type,int id)
{
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
node::~node()
{
	int sz = Sz(nxt);
	rep(i,sz){
		delete nxt[i].first;
	}
	nxt.clear();
}




