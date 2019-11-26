//
// Created by qfeng on 17-9-29.
//

#include "gbtree_model/inner_node.h"

namespace quickscorer {

    InnerNode::InnerNode(uint32_t depth) : TreeNode(depth) {}

    bool InnerNode::is_leaf() const {
        return false;
    }

    float InnerNode::predict(const Eigen::SparseVector<float> &features) {
        auto f_val = features.data().at(_split_feature, std::nanf("1"));
        if (!std::isnan(f_val)) {
            if (f_val < _split_value) return _left_child->predict(features);
            else return _right_child->predict(features);
        } else {
            if (_default_to_left) return _left_child->predict(features);
            else return _right_child->predict(features);
        }
    }

}
