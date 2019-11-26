//
// Created by qfeng on 17-9-29.
//

#ifndef QUICKSCORER_GBTREE_MODEL_H
#define QUICKSCORER_GBTREE_MODEL_H

#include "inner_node.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <log/qs_log.h>
#include <dense_hash_map>
#include <dense_hash_set>

using namespace std;

namespace quickscorer {

    class GBTreeModel {
    public:
        shared_ptr<spdlog::logger> _logger;
        vector<unique_ptr<InnerNode>> _trees;
        vector<float> _tree_weights;
        // distinct features of each tree
        vector<unique_ptr<google::dense_hash_set<uint64_t>>> _tree_features;
        // feature size across all tress used to initialize new instances
        uint64_t _feature_size = 0;

        GBTreeModel();

        vector<float> predict(const vector<Eigen::SparseVector<float>> &instances, bool output_margin = false);

        float predict(const Eigen::SparseVector<float> &features, bool output_margin = false);

        static unique_ptr<GBTreeModel> from_xgb_txt(const string &model_path, int num_trees = 0);

        static uint64_t
        parse_line(const string &line, google::dense_hash_map<int, InnerNode *> &recent_depth_nodes, InnerNode &root);
    };

}


#endif //QUICKSCORER_GBTREE_MODEL_H
