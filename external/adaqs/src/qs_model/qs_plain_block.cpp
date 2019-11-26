//
// Created by qfeng on 17-9-25.
//

#include "qs_model/qs_plain_block.h"
#include "gbtree_model/leaf_node.h"
#include <dense_hash_set>
#include <iostream>

namespace quickscorer {

    float QSPlainBlock::raw_predict(const Eigen::SparseVector<float> &features) {
        // initialize result bitvectors, one per each tree
        vector<BANDWITH_TYPE> v(_tree_weights.size());
        for_each(v.begin(), v.end(), [](BANDWITH_TYPE &v_h) {
            // initialize result bitvector with 11...11
            v_h = ~((BANDWITH_TYPE) 0);
        });
        // update result bitvectors by visible features one by one
        google::dense_hash_set<uint64_t> feature_set;
        feature_set.set_empty_key(std::numeric_limits<uint64_t>::max());
        for (Eigen::SparseVector<float>::InnerIterator it(features); it; ++it) {
            auto f_idx = it.index();
            auto f_val = it.value();
            feature_set.emplace(f_idx);

            auto found = _offsets.find(f_idx);
            if (found != _offsets.end()) {
                // the feature is adopted by tree nodes
                int start = found->second.first;
                int end = found->second.second;
                if (_thresholds[start] > f_val) {
                    //skip search
                } else {
                    // search false nodes by step size of delta
                    int i = start + DELTA;
                    for (; i < end && _thresholds[i] <= f_val; i += DELTA);
                    int j = std::min(i - 1, end - 1);
                    for (; j >= start && _thresholds[j] > f_val; --j);
                    // jth one is the greatest threshold smaller than the feature value
                    for (int k = start; k <= j; ++k) {
                        auto h = _tree_ids[k];
                        v[h] = v[h] & _bitvectors[k];
                    }
                }
            }
        }
        // update result bitvectors by invisible features wrt xgboost's default directions
        for (int h = 0; h < _tree_default_encodings.size(); ++h) {
            auto encodings = _tree_default_encodings[h];
            for (auto it = encodings.begin(); it != encodings.end(); ++it) {
                auto f_idx = it->first;
                auto f_encoding = it->second;
                if (feature_set.find(f_idx) == feature_set.end()) v[h] = v[h] & f_encoding;
            }
        }
        // calculate the margin
        float margin = 0;
        for (int h = 0; h < v.size(); ++h) {
            // get the index of leftmost bit one
            auto index = highest_one_bit_index(v[h]);
            margin += _tree_weights[h] * _leaf_values[h * BANDWITH_BITS + index];
        }
        return margin;
    }

    unique_ptr<QSPlainBlock> QSPlainBlock::from_gbtree_model(const GBTreeModel &gbtree_model, uint32_t start_tree,
                                                             uint32_t end_tree) {
        // initialize quickscorer model block
        unique_ptr<QSPlainBlock> qs_block = unique_ptr<QSPlainBlock>(new QSPlainBlock());
        qs_block->_tree_weights.resize(end_tree - start_tree);
        vector<QSNode> all_inner_nodes;
        // traverse trees one by one
        for (int i = start_tree; i < end_tree; ++i) {
            // initialize tree index
            int tree_idx = i - start_tree;
            // set tree weight
            qs_block->_tree_weights[tree_idx] = gbtree_model._tree_weights[i];
            // initialize default feature encodings
            DefaultEncodingMap default_encodings;
            auto all_ones = ~((BANDWITH_TYPE) 0);
            for (auto it = gbtree_model._tree_features[i]->begin(); it != gbtree_model._tree_features[i]->end(); ++it)
                default_encodings[*it] = all_ones;
            // traverse tree nodes
            int leaf_idx = BANDWITH_BITS - 1;
            traverse_tree(gbtree_model._trees[i].get(), all_inner_nodes, leaf_idx, qs_block->_leaf_values,
                          default_encodings, tree_idx);
            // remove features with all the default directions to the left
            for (auto it = default_encodings.begin(); it != default_encodings.end();) {
                if (it->second == all_ones) it = default_encodings.erase(it);
                else ++it;
            }
            qs_block->_tree_default_encodings.emplace_back(default_encodings);
            // complement leaf values
            for (int j = leaf_idx; j >= 0; --j) qs_block->_leaf_values.emplace_back(0);
        }
        // sort the inner nodes of all trees by feature indices and values in ascending order
        std::stable_sort(all_inner_nodes.begin(), all_inner_nodes.end(), [](const QSNode &lhs, const QSNode &rhs) {
            if (lhs._feature < rhs._feature) return true;
            else if (lhs._feature == rhs._feature) return lhs._threshold < rhs._threshold;
            else return false;
        });
        // set the content of model block using the traversed results
        uint64_t last_feature;
        bool first_node = true;
        for (int i = 0; i < all_inner_nodes.size(); ++i) {
            auto &qs_node = all_inner_nodes[i];
            qs_block->_thresholds.emplace_back(qs_node._threshold);
            qs_block->_bitvectors.emplace_back(qs_node._bitvector);
            qs_block->_tree_ids.emplace_back(qs_node._tree_id);
            if (first_node) {
                qs_block->_offsets[qs_node._feature] = make_pair(0, 0);
                last_feature = qs_node._feature;
                first_node = false;
            } else if (last_feature < qs_node._feature) {
                qs_block->_offsets[last_feature].second = i;
                qs_block->_offsets[qs_node._feature] = make_pair(i, i);
                last_feature = qs_node._feature;
            }
        }
        qs_block->_offsets[last_feature].second = all_inner_nodes.size();
        // return result
        return qs_block;
    }

    void QSPlainBlock::traverse_tree(TreeNode *node, vector<QSNode> &complete_inner_nodes, int &leaf_idx,
                                     vector<float> &leaf_values, DefaultEncodingMap &default_encodings, int tree_id) {
        if (!node->is_leaf()) {
            // traverse inner node
            auto inner_node = dynamic_cast<InnerNode *>(node);
            traverse_tree(inner_node->_left_child.get(), complete_inner_nodes, leaf_idx, leaf_values,
                          default_encodings, tree_id);
            inner_node->_bitvector = inner_node->_bitvector & inner_node->_left_child->_mask;
            traverse_tree(inner_node->_right_child.get(), complete_inner_nodes, leaf_idx, leaf_values,
                          default_encodings, tree_id);
            inner_node->_mask = inner_node->_left_child->_mask & inner_node->_right_child->_mask;
            complete_inner_nodes.emplace_back(inner_node->_split_feature, inner_node->_split_value, tree_id,
                                              inner_node->_bitvector);
            // pre-calculate the default encodings of invisible features wrt xgboost's default directions
            if (!inner_node->_default_to_left) {
                default_encodings[inner_node->_split_feature] =
                        default_encodings[inner_node->_split_feature] & inner_node->_bitvector;
            }
        } else {
            // traverse leaf node
            auto leaf_node = dynamic_cast<LeafNode *>(node);
            leaf_node->_mask = leaf_node->_mask & (~(((BANDWITH_TYPE) 1) << leaf_idx));
            leaf_idx--;
            leaf_values.emplace_back(leaf_node->_value);
        }
    }

} //namespace