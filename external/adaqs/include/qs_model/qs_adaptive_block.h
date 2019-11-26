//
// Created by qfeng on 17-10-13.
//

#ifndef QUICKSCORER_ADAQS_MODEL_BLOCK_H
#define QUICKSCORER_ADAQS_MODEL_BLOCK_H

#include <Eigen/Sparse>
#include <gbtree_model/gbtree_model.h>
#include "qs_model_block.h"
#include "qs_node.h"
#include "qs_condition.h"

namespace quickscorer {


    class QSAdaBlock : public QSModelBlock {
    public:
        QSConditions _less_conditions;
        QSConditions _more_conditions;
        vector<float> _leaf_values;
        vector<float> _tree_weights;

        // calculate the margin
        float raw_predict(const Eigen::SparseVector<float> &features);

        static unique_ptr<QSAdaBlock>
        from_gbtree_model(const GBTreeModel &gbtree_model, uint32_t start_tree, uint32_t end_tree);

        static void traverse_tree(TreeNode *node, vector<QSNode> &less_nodes, vector<QSNode> &more_nodes,
                                  int &leaf_idx, vector<float> &leaf_values, int tree_id);

        static void build_conditions(QSConditions &conditions, vector<QSNode> &nodes, bool ascending);
    };

}

#endif //QUICKSCORER_ADAQS_MODEL_BLOCK_H
