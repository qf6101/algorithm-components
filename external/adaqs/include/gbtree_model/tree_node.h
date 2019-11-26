//
// Created by qfeng on 17-9-29.
//

#ifndef QUICKSCORER_TREE_NODE_H
#define QUICKSCORER_TREE_NODE_H

#include <memory>
#include <Eigen/Sparse>
#include "conf/config.h"

using namespace std;

namespace quickscorer {

    class TreeNode {
    public:
        uint32_t _depth;
        // bitvector built from the left children
        BANDWITH_TYPE _bitvector;
        // mask for building the bitvectors of parents
        BANDWITH_TYPE _mask;

        TreeNode(uint32_t depth);

        virtual bool is_leaf() const = 0;

        virtual float predict(const Eigen::SparseVector<float> &features) = 0;
    };

}

#endif //QUICKSCORER_TREE_NODE_H
