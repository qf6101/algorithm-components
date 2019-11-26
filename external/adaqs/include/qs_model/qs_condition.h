//
// Created by qfeng on 17-10-13.
//

#ifndef QUICKSCORER_QS_CONTENT_H
#define QUICKSCORER_QS_CONTENT_H

#include <dense_hash_map>

namespace quickscorer {

    class QSConditions {
    public:
        vector<float> _thresholds;
        vector<uint32_t> _tree_ids;
        vector<BANDWITH_TYPE> _bitvectors;
        google::dense_hash_map<uint64_t, pair<uint32_t, uint32_t>> _offsets;

        QSConditions() {
            _offsets.set_empty_key(std::numeric_limits<uint64_t>::max());
        }
    };

}


#endif //QUICKSCORER_QS_CONTENT_H
