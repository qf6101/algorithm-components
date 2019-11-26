//
// Created by qfeng on 17-9-29.
//

#ifndef QUICKSCORER_LEAF_NODE_H
#define QUICKSCORER_LEAF_NODE_H

#include "tree_node.h"

namespace quickscorer {

    class LeafNode : public TreeNode {
    public:
        float _value = 0;

        LeafNode(uint32_t depth, float value);

        bool is_leaf() const;

        float predict(const Eigen::SparseVector<float> &features);
    };

}


#endif //QUICKSCORER_LEAF_NODE_H
