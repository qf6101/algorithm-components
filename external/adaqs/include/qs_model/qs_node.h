//
// Created by qfeng on 17-10-7.
//

#ifndef QUICKSCORER_QS_NODE_H
#define QUICKSCORER_QS_NODE_H


#include <cstdint>
#include "conf/config.h"

namespace quickscorer {

    class QSNode {
    public:
        uint64_t _feature;
        float _threshold = -1;
        uint32_t _tree_id;
        BANDWITH_TYPE _bitvector;

        QSNode(uint64_t feature, float threshold, uint32_t tree_id, BANDWITH_TYPE bitvector);
    };

}


#endif //QUICKSCORER_QS_NODE_H
