//
// Created by v on 27/10/22.
//

#ifndef VOS__RBTREE_H_
#define VOS__RBTREE_H_
#include "pcb.h"

enum NodeType{
	BLACK,
	RED
};
struct rb_node{
	u32 key; //vruntime as our key
	void* data;

	struct rb_node* parent;
	struct rb_node* left_node;
	struct rb_node* right_node;

	enum NodeType node_type;

};

struct rb_root{
	struct rb_node* rb_node;
};

extern void rb_init(struct rb_root* rb_root, void *data);

void* rb_lookup(struct rb_node node, u32 key);

void rb_insert(struct rb_root root, u32 key, void* data);
void rb_delete(struct rb_root root, u32 key);


#endif //VOS__RBTREE_H_
