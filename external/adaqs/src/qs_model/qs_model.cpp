//
// Created by qfeng on 17-9-26.
//

#include "qs_model/qs_model.h"
#include "qs_model/qs_plain_block.h"
#include <iostream>
#include <fstream>
#include <qs_model/qs_adaptive_block.h>

using namespace std;

namespace quickscorer {

    QSModel::QSModel() {
        _logger = spdlog::get(QSLog::_logger_name);
    }

    vector<float> QSModel::predict(const vector<Eigen::SparseVector<float>> &instances, bool output_margin) {
        // calculate margins
        vector<float> margins(instances.size());
        try {
            for (int i = 0; i < _model_blocks.size(); ++i) {
                auto scores = _model_blocks[i]->raw_predict(instances);
                for(int j = 0; j < scores.size(); ++j) margins[j] += scores[j];
            }
        } catch (exception &ex) {
            _logger->error("Fail to predict instances of size {}, detail: {}", instances.size(), ex.what());
        }
        // calculate logistic scores
        vector<float> results;
        for(int i = 0; i < margins.size(); ++i) {
            if(output_margin) results.push_back(margins[i]);
            else results.push_back(1.0 / (1.0 + exp(-margins[i])));
        }
        return results;
    }

    float QSModel::predict(const Eigen::SparseVector<float> &features, bool output_margin) {
        float margin = 0;
        try {
            for (int i = 0; i < _model_blocks.size(); ++i) {
                margin += _model_blocks[i]->raw_predict(features);
            }
        } catch (exception &ex) {
            _logger->error("Fail to predict features of size {}, detail: {}", features.size(), ex.what());
        }
        if(output_margin) return margin;
        else return 1.0 / (1.0 + exp(-margin));
    }

    unique_ptr<QSModel> QSModel::from_xgb_txt(string model_path, uint32_t block_size, bool adaptive, int num_trees) {
        auto logger = spdlog::get(QSLog::_logger_name);
        logger->info("Create quickscorer model with block size {} and adaptive {} from {}", block_size,
                     adaptive, model_path);

        auto gbtree_model = GBTreeModel::from_xgb_txt(model_path, num_trees);
        if (gbtree_model != nullptr) {
            try {
                unique_ptr<QSModel> qs_model = unique_ptr<QSModel>(new QSModel());
                qs_model->_feature_size = gbtree_model->_feature_size;
                // create model blocks with the input block size
                uint32_t start_tree = 0;
                for (int i = 0; i < gbtree_model->_trees.size();) {
                    if (++i % block_size == 0) {
                        if (adaptive) {
                            auto ada_block = QSAdaBlock::from_gbtree_model(*gbtree_model.get(), start_tree, i);
                            qs_model->_model_blocks.emplace_back(std::move(ada_block));
                        } else {
                            auto plain_block = QSPlainBlock::from_gbtree_model(*gbtree_model.get(), start_tree, i);
                            qs_model->_model_blocks.emplace_back(std::move(plain_block));
                        }
                        start_tree = i;
                    }
                }
                // create the last model block
                if (start_tree < gbtree_model->_trees.size()) {
                    if (adaptive) {
                        auto ada_block = QSAdaBlock::from_gbtree_model(*gbtree_model.get(), start_tree,
                                                                       gbtree_model->_trees.size());
                        qs_model->_model_blocks.emplace_back(std::move(ada_block));
                    } else {
                        auto plain_block = QSPlainBlock::from_gbtree_model(*gbtree_model.get(), start_tree,
                                                                           gbtree_model->_trees.size());
                        qs_model->_model_blocks.emplace_back(std::move(plain_block));
                    }

                }
                return qs_model;
            } catch (exception &ex) {
                logger->error("Fail to load QS model from {}, detail: {}.", model_path, ex.what());
            }
        }
        return nullptr;
    }

}