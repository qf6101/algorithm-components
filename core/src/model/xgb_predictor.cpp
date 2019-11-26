//
// Created by qfeng on 17-10-31.
//

#include <log/algocomp_log.h>
#include "model/xgb_predictor.h"

namespace algocomp {

    XgbPredictor::XgbPredictor() {
        _logger = spdlog::get(ACLog::_logger_name);
        _logger->info("Initialize xgboost predictor.");
    }

    XgbPredictor::~XgbPredictor() {
        _logger->info("Leave xgboost predictor.");
    }

    uint64_t XgbPredictor::weight_size() {
        return 0;
    }

    float XgbPredictor::score(const Eigen::SparseVector<float> &sample) {
        return _xgb_predictor->Predict(sample, true, 0);
    }

    unique_ptr<XgbPredictor> XgbPredictor::load_model(const string model_path) {
        unique_ptr<XgbPredictor> output_model = unique_ptr<XgbPredictor>(new XgbPredictor());
        output_model->_xgb_predictor = unique_ptr<xgboost::Predictor>(new xgboost::Predictor());
        output_model->_xgb_predictor->Load(model_path);
        return output_model;
    }

}