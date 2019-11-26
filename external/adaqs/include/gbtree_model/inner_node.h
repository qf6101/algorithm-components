//
// Created by qfeng on 17-9-29.
//

#ifndef QUICKSCORER_NONLEAF_NODE_H
#define QUICKSCORER_NONLEAF_NODE_H

#include "tree_node.h"
#include <Eigen/Sparse>

namespace quickscorer {

    class InnerNode: public TreeNode {
    public:
        unique_ptr<TreeNode> _left_child = nullptr;
        unique_ptr<TreeNode> _right_child = nullptr;
        uint64_t _split_feature;
        float _split_value;
        bool _default_to_left = true;

        InnerNode(uint32_t depth);

        bool is_leaf() const;

        float predict(const Eigen::SparseVector<float> &features);
    };

}


#endif //QUICKSCORER_NONLEAF_NODE_H
