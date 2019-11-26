//
// Created by qfeng on 17-11-3.
//

#include "model/raw_gbdt_model.h"
#include <log/algocomp_log.h>
#include <iostream>

namespace algocomp {

    RawGbdtModel::RawGbdtModel() {
        _logger = spdlog::get(ACLog::_logger_name);
        _logger->info("Initialize raw gbdt model.");
    }

    RawGbdtModel::~RawGbdtModel() {
        _logger->info("Leave raw gbdt model.");
    }

    uint64_t RawGbdtModel::weight_size() {
        return 0;
    }

    float RawGbdtModel::score(const Eigen::SparseVector<float> &sample) {
        return _gbtree->predict(sample, true);
    }

    unique_ptr<RawGbdtModel> RawGbdtModel::load_model(const string model_path) {
        unique_ptr<RawGbdtModel> output_model = unique_ptr<RawGbdtModel>(new RawGbdtModel());
        output_model->_gbtree = quickscorer::GBTreeModel::from_xgb_txt(model_path);
        return output_model;
    }

}