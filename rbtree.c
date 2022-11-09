//
// Created by v on 27/10/22.
//

#include "rbtree.h"

#include "stdlib.h"

void rb_init(struct rb_root* root, void * data){

	root->rb_node = malloc(sizeof (struct rb_node));

	root->rb_node->node_type = BLACK;

	root->rb_node->data = data;

}

void rb_insert(struct rb_root root, u32 key, void* data){

	if(root.rb_node == NULL){ //First time initialization
	}
}