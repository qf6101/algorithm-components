//
// Created by qfeng on 17-9-29.
//

#include "gbtree_model/leaf_node.h"

namespace quickscorer {

    LeafNode::LeafNode(uint32_t depth, float value) : TreeNode(depth) {
        _value = value;
    }

    bool LeafNode::is_leaf() const {
        return true;
    }

    float LeafNode::predict(const Eigen::SparseVector<float> &features) {
        return _value;
    }
}
