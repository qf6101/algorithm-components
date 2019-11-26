//
// Created by qfeng on 17-9-25.
//

#ifndef QUICKSCORER_QS_PLAIN_BLOCK_H
#define QUICKSCORER_QS_PLAIN_BLOCK_H

#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <dense_hash_map>
#include "gbtree_model/gbtree_model.h"
#include "conf/config.h"
#include "qs_node.h"
#include "qs_model_block.h"


using namespace std;

namespace quickscorer {

    // vector of default encodings of missing features, one per each tree
    typedef unordered_map<uint64_t, BANDWITH_TYPE> DefaultEncodingMap;

    class QSPlainBlock: public QSModelBlock {
    public:
        vector<float> _thresholds;
        vector<uint32_t> _tree_ids;
        vector<BANDWITH_TYPE> _bitvectors;
        google::dense_hash_map<uint64_t, pair<uint32_t, uint32_t>> _offsets;
        vector<float> _leaf_values;
        vector<DefaultEncodingMap> _tree_default_encodings;
        vector<float> _tree_weights;

        QSPlainBlock() {
            _offsets.set_empty_key(std::numeric_limits<uint64_t>::max());
        }

        // calculate the margin
        float raw_predict(const Eigen::SparseVector<float> &features);

        static unique_ptr<QSPlainBlock>
        from_gbtree_model(const GBTreeModel &gbtree_model, uint32_t start_tree, uint32_t end_tree);

        static void traverse_tree(TreeNode *node, vector<QSNode> &complete_inner_nodes, int &leaf_idx,
                                  vector<float> &leaf_values, DefaultEncodingMap & default_encodings, int tree_id);
    };

}

#endif //QUICKSCORER_QS_PLAIN_BLOCK_H


