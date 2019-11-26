//
// Created by qfeng on 17-10-7.
//

#include "qs_model/qs_node.h"

namespace quickscorer {

    QSNode::QSNode(uint64_t feature, float threshold, uint32_t tree_id, BANDWITH_TYPE bitvector) {
        this->_feature = feature;
        this->_threshold = threshold;
        this->_tree_id = tree_id;
        this->_bitvector = bitvector;
    }

}
