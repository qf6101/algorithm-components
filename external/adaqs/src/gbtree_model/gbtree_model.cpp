//
// Created by qfeng on 17-9-29.
//

#include "gbtree_model/gbtree_model.h"
#include "gbtree_model/leaf_node.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


using namespace std;

namespace quickscorer {

    GBTreeModel::GBTreeModel() {
        _logger = spdlog::get(QSLog::_logger_name);
    }

    vector<float> GBTreeModel::predict(const vector<Eigen::SparseVector<float>> &instances, bool output_margin) {
        vector<float> results;
        for(int i = 0; i < instances.size(); ++i) {
            auto score = predict(instances[i], output_margin);
            results.push_back(score);
        }
        return results;
    }

    float GBTreeModel::predict(const Eigen::SparseVector<float> &features, bool output_margin) {
        float margin = 0;
        try {
            for (int i = 0; i < _trees.size(); ++i) {
                margin += _tree_weights[i] * _trees[i]->predict(features);
            }
        } catch (exception &ex) {
            _logger->error("Fail to predict features of size {}, detail: {}", features.size(), ex.what());
        }
        if(output_margin) return margin;
        else return 1.0 / (1.0 + exp(-margin));
    }

    unique_ptr<GBTreeModel> GBTreeModel::from_xgb_txt(const string &model_path, int num_trees) {
        auto logger = spdlog::get(QSLog::_logger_name);
        logger->info("Load gbtree model from {}", model_path);
        try {
            int tree_count = 0;
            ifstream model_file;
            model_file.open(model_path);
            if (model_file.is_open()) {
                string line;
                auto model = unique_ptr<GBTreeModel>(new GBTreeModel());
                uint64_t feature_size = 0;
                google::dense_hash_set<uint64_t> all_tree_features;
                all_tree_features.set_empty_key(std::numeric_limits<uint64_t>::max());
                unique_ptr<InnerNode> tree = nullptr;
                google::dense_hash_map<int, InnerNode *> recent_depth_nodes;
                recent_depth_nodes.set_empty_key(std::numeric_limits<int>::max());
                unique_ptr<google::dense_hash_set<uint64_t>> tree_features = nullptr;
                while (!model_file.eof()) {
                    getline(model_file, line);
                    if (boost::algorithm::starts_with(line, "booster") ||
                        boost::algorithm::trim_copy(line).length() == 0) {
                        // start parsing a new tree or finish parsing all the trees
                        if (tree != nullptr) {
                            model->_trees.push_back(std::move(tree));
                            model->_tree_weights.push_back(1);
                            recent_depth_nodes.clear();
                            all_tree_features.insert(tree_features->begin(), tree_features->end());
                            model->_tree_features.push_back(std::move(tree_features));
                        }
                        // check the tree numbers
                        if (num_trees > 0) {
                            if (tree_count++ >= num_trees) break;
                        }
                        // create a new tree instance
                        if (boost::algorithm::starts_with(line, "booster")) {
                            // create a new tree
                            tree = unique_ptr<InnerNode>(new InnerNode(0));
                            tree_features = unique_ptr<google::dense_hash_set<uint64_t>>(
                                    new google::dense_hash_set<uint64_t>());
                            tree_features->set_empty_key(std::numeric_limits<uint64_t>::max());
                        }
                    } else {
                        // parse a line of tree node
                        auto f_idx = parse_line(line, recent_depth_nodes, *tree);
                        if (f_idx > 0) {
                            if (feature_size < f_idx) feature_size = f_idx;
                            tree_features->insert(f_idx);
                        }
                    }
                }
                model->_feature_size = feature_size + 1;
                if (model_file.is_open()) model_file.close();
                // log statistical information
                logger->info("GBTree stat: feature size {}", model->_feature_size);
                logger->info("GBTree stat: distinct features {}", all_tree_features.size());
#ifdef _DEBUG
                vector<pair<uint64_t, int>> feature_counts;
                for (auto it = all_tree_features.begin(); it != all_tree_features.end(); ++it) {
                    int count = 0;
                    for (int i = 0; i < model->_tree_features.size(); i++) {
                        auto found = model->_tree_features[i]->find(*it);
                        if (found != model->_tree_features[i]->end()) ++count;
                    }
                    feature_counts.emplace_back(*it, count);
                }
                std::sort(feature_counts.begin(), feature_counts.end(),
                          [](pair<uint64_t, int> &lhs, pair<uint64_t, int> &rhs) {
                              return lhs.second > rhs.second;
                          });
                ostringstream ss;
                for (int i = 0; i < feature_counts.size(); ++i) {
                    ss << feature_counts[i].first << ":" << feature_counts[i].second << " ";
                }
                logger->info("GBTree stat: feature histogram {}", ss.str());
#endif
                // return loaded model
                return model;
            } else
                logger->error("Fail to open xgboost model file {}.", model_path);
        } catch (exception &ex) {
            logger->error("Fail to load xgboost model from {}, detail: {}.", model_path, ex.what());
        }
        return nullptr;
    }

    uint64_t
    GBTreeModel::parse_line(const string &line, google::dense_hash_map<int, InnerNode *> &recent_depth_nodes,
                            InnerNode &root) {
        // parse depth based on number of tabs
        int i = 0;
        while (line[i] == '\t') ++i;
        int depth = i;
        // parse feature
        while (line[i] != ':') ++i;
        if (line[i + 1] == '[') {
            // inner node and parse feature
            int f_idx_st = i + 3;
            while (line[i] != '<') ++i;
            int f_idx_end = i;
            auto f_idx = boost::lexical_cast<uint64_t>(line.substr(f_idx_st, f_idx_end - f_idx_st));
            // parse condition value
            int f_val_st = i + 1;
            while (line[i] != ']') ++i;
            int f_val_end = i;
            auto f_val = boost::lexical_cast<float>(line.substr(f_val_st, f_val_end - f_val_st));
            // parse left child
            while (line[i] != '=') ++i;
            int left_child_st = i + 1;
            while (line[i] != ',') ++i;
            int left_child_end = i;
            auto left_child = line.substr(left_child_st, left_child_end - left_child_st);
            // skip right child
            while (line[i] != '=') ++i;
            // parse default directory
            ++i;
            while (line[i] != '=') ++i;
            ++i;
            auto missing = line.substr(i);
            // append node to its parent
            if (depth > 0) {
                // initialize non-root node
                auto node = unique_ptr<InnerNode>(new InnerNode(depth));
                node->_split_feature = f_idx;
                node->_split_value = f_val;
                node->_default_to_left = (missing == left_child);
                // append node to its parent
                auto parent = recent_depth_nodes[depth - 1];
                if (parent->_left_child == nullptr) {
                    parent->_left_child = std::move(node);
                    recent_depth_nodes[depth] = dynamic_cast<InnerNode *>(parent->_left_child.get());
                } else {
                    parent->_right_child = std::move(node);
                    recent_depth_nodes[depth] = dynamic_cast<InnerNode *>(parent->_right_child.get());
                }
            } else {
                // initialize root node
                root._split_feature = f_idx;
                root._split_value = f_val;
                root._default_to_left = (missing == left_child);
                recent_depth_nodes[0] = &root;
            }

            return f_idx;
        } else {
            // leaf node and parse value
            while (line[i] != '=') ++i;
            ++i;
            auto leaf_val = boost::lexical_cast<float>(line.substr(i));
            auto &parent = recent_depth_nodes[depth - 1];
            auto leaf_node = unique_ptr<LeafNode>(new LeafNode(depth, leaf_val));
            if (parent->_left_child == nullptr) parent->_left_child = std::move(leaf_node);
            else parent->_right_child = std::move(leaf_node);
            return 0;
        }
    }
}
