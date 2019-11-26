//
// Created by qfeng on 17-9-29.
//

#include "gbtree_model/tree_node.h"

namespace quickscorer {

    TreeNode::TreeNode(uint32_t depth) {
        // initialize depth of node in the tree
        this->_depth = depth;
        // initialize bitvector and mask with 11...11
        _mask = _bitvector = ~((BANDWITH_TYPE) 0);
    }

}