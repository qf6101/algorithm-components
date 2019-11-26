//
// Created by qfeng on 17-11-3.
//

#include <log/algocomp_log.h>
#include <iostream>
#include "model/adaqs_model.h"

namespace algocomp {

    AdaqsModel::AdaqsModel() {
        _logger = spdlog::get(ACLog::_logger_name);
        _logger->info("Initialize adaptive quickscorer.");
    }

    AdaqsModel::~AdaqsModel() {
        _logger->info("Leave adaptive quickscorer.");
    }

    uint64_t AdaqsModel::weight_size() {
        return 0;
    }

    float AdaqsModel::score(const Eigen::SparseVector<float> &sample) {
        return _adaqs->predict(sample, true);
    }

    unique_ptr<AdaqsModel> AdaqsModel::load_model(const string model_path, const int block_size) {
        unique_ptr<AdaqsModel> output_model = unique_ptr<AdaqsModel>(new AdaqsModel());
        output_model->_adaqs = quickscorer::QSModel::from_xgb_txt(model_path, block_size);
        return output_model;
    }

}