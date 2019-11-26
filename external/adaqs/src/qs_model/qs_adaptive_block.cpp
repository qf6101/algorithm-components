//
// Created by qfeng on 17-10-13.
//

#include <gbtree_model/leaf_node.h>
#include "qs_model/qs_adaptive_block.h"

namespace quickscorer {

    float QSAdaBlock::raw_predict(const Eigen::SparseVector<float> &features) {
        // initialize result bitvectors, one per each tree
        vector<BANDWITH_TYPE> v(_tree_weights.size());
        for_each(v.begin(), v.end(), [](BANDWITH_TYPE &v_h) {
            // initialize result bitvector with 11...11
            v_h = ~((BANDWITH_TYPE) 0);
        });
        // update result bitvectors by visible features one by one
        for (Eigen::SparseVector<float>::InnerIterator it(features); it; ++it) {
            auto f_idx = it.index();
            auto f_val = it.value();
            // find false nodes in less conditions
            auto found = _less_conditions._offsets.find(f_idx);
            if (found != _less_conditions._offsets.end()) {
                // the feature is adopted by tree nodes
                int start = found->second.first;
                int end = found->second.second;
                if (_less_conditions._thresholds[start] > f_val) {
                    // skip search
                } else {
                    // search false nodes by step size of delta
                    int i = start + DELTA;
                    for (; i < end && _less_conditions._thresholds[i] <= f_val; i += DELTA);
                    int j = std::min(i - 1, end - 1);
                    for (; j >= start && _less_conditions._thresholds[j] > f_val; --j);
                    // jth one is the greatest threshold smaller than the feature value
                    for (int k = start; k <= j; ++k) {
                        auto h = _less_conditions._tree_ids[k];
                        v[h] = v[h] & _less_conditions._bitvectors[k];
                    }
                }
            }
            // find false nodes in more conditions
            found = _more_conditions._offsets.find(f_idx);
            if (found != _more_conditions._offsets.end()) {
                // the feature is adopted by tree nodes
                int start = found->second.first;
                int end = found->second.second;
                if (_more_conditions._thresholds[start] <= f_val) {
                    //skip search
                } else {
                    // search false nodes by step size of delta
                    int i = start + DELTA;
                    for (; i < end && _more_conditions._thresholds[i] > f_val; i += DELTA);
                    int j = std::min(i - 1, end - 1);
                    for (; j >= start && _more_conditions._thresholds[j] <= f_val; --j);
                    // jth one is the smallest threshold greater than or equal to the feature value
                    for (int k = start; k <= j; ++k) {
                        auto h = _more_conditions._tree_ids[k];
                        v[h] = v[h] & _more_conditions._bitvectors[k];
                    }
                }
            }
        }
        float margin = 0;
        for (int h = 0; h < v.size(); ++h) {
            // get the index of leftmost bit one
            auto index = highest_one_bit_index(v[h]);
            margin += _tree_weights[h] * _leaf_values[h * BANDWITH_BITS + index];
        }
        return margin;
    }

    unique_ptr<QSAdaBlock> QSAdaBlock::from_gbtree_model(const GBTreeModel &gbtree_model, uint32_t start_tree,
                                                         uint32_t end_tree) {
        // initialize quickscorer model block
        unique_ptr<QSAdaBlock> qs_block = unique_ptr<QSAdaBlock>(new QSAdaBlock());
        qs_block->_tree_weights.resize(end_tree - start_tree);
        vector<QSNode> less_nodes;
        vector<QSNode> more_nodes;
        // traverse trees one by one
        for (int i = start_tree; i < end_tree; ++i) {
            // initialize tree index
            int tree_idx = i - start_tree;
            // set tree weight
            qs_block->_tree_weights[tree_idx] = gbtree_model._tree_weights[i];
            // traverse tree nodes
            int leaf_idx = BANDWITH_BITS - 1;
            traverse_tree(gbtree_model._trees[i].get(), less_nodes, more_nodes, leaf_idx, qs_block->_leaf_values,
                          tree_idx);
            // complement leaf values
            for (int j = leaf_idx; j >= 0; --j) qs_block->_leaf_values.emplace_back(0);
        }
        // build less and more conditions individually
        build_conditions(qs_block->_less_conditions, less_nodes, true);
        build_conditions(qs_block->_more_conditions, more_nodes, false);
        // return result
        return qs_block;
    }

    void QSAdaBlock::traverse_tree(TreeNode *node, vector<QSNode> &less_nodes, vector<QSNode> &more_nodes,
                                   int &leaf_idx, vector<float> &leaf_values, int tree_id) {
        if (!node->is_leaf()) {
            // traverse inner node
            auto inner_node = dynamic_cast<InnerNode *>(node);

            if (inner_node->_default_to_left) {
                traverse_tree(inner_node->_left_child.get(), less_nodes, more_nodes, leaf_idx, leaf_values, tree_id);
                inner_node->_bitvector = inner_node->_bitvector & inner_node->_left_child->_mask;
                traverse_tree(inner_node->_right_child.get(), less_nodes, more_nodes, leaf_idx, leaf_values, tree_id);
                inner_node->_mask = inner_node->_left_child->_mask & inner_node->_right_child->_mask;
                less_nodes.emplace_back(inner_node->_split_feature, inner_node->_split_value, tree_id,
                                        inner_node->_bitvector);
            } else {
                traverse_tree(inner_node->_right_child.get(), less_nodes, more_nodes, leaf_idx, leaf_values, tree_id);
                inner_node->_bitvector = inner_node->_bitvector & inner_node->_right_child->_mask;
                traverse_tree(inner_node->_left_child.get(), less_nodes, more_nodes, leaf_idx, leaf_values, tree_id);
                inner_node->_mask = inner_node->_left_child->_mask & inner_node->_right_child->_mask;
                more_nodes.emplace_back(inner_node->_split_feature, inner_node->_split_value, tree_id,
                                        inner_node->_bitvector);
            }
        } else {
            // traverse leaf node
            auto leaf_node = dynamic_cast<LeafNode *>(node);
            leaf_node->_mask = leaf_node->_mask & (~(((BANDWITH_TYPE) 1) << leaf_idx));
            leaf_idx--;
            leaf_values.emplace_back(leaf_node->_value);
        }
    }

    void QSAdaBlock::build_conditions(QSConditions &conditions, vector<QSNode> &nodes, bool ascending) {
        // sort the inner nodes of all trees by feature indices and values in ascending order
        std::stable_sort(nodes.begin(), nodes.end(), [&ascending](const QSNode &lhs, const QSNode &rhs) {
            if (lhs._feature < rhs._feature) return true;
            else if (lhs._feature == rhs._feature) {
                if (ascending) return lhs._threshold < rhs._threshold;
                else return rhs._threshold < lhs._threshold;
            } else return false;
        });
        // set the conditions of model block using the traversed results
        uint64_t last_feature;
        bool first_node = true;
        for (int i = 0; i < nodes.size(); ++i) {
            auto &qs_node = nodes[i];
            conditions._thresholds.emplace_back(qs_node._threshold);
            conditions._bitvectors.emplace_back(qs_node._bitvector);
            conditions._tree_ids.emplace_back(qs_node._tree_id);
            if (first_node) {
                conditions._offsets[qs_node._feature] = make_pair(0, 0);
                last_feature = qs_node._feature;
                first_node = false;
            } else if (last_feature < qs_node._feature) {
                conditions._offsets[last_feature].second = i;
                conditions._offsets[qs_node._feature] = make_pair(i, i);
                last_feature = qs_node._feature;
            }
        }
        conditions._offsets[last_feature].second = nodes.size();
    }

}
