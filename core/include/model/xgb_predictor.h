//
// Created by qfeng on 17-10-31.
//

#ifndef ALGOCOMP_GBDT_MODEL_H
#define ALGOCOMP_GBDT_MODEL_H


#include <spdlog/logger.h>
#include <memory>
#include <predictor.h>
#include "model.h"

using namespace std;

namespace algocomp {

    /**
     * XGBoost Predictor
     *
     * Define wrap of XGBoost predictor
     */
    class XgbPredictor : public Model {
    public:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Model
        unique_ptr<xgboost::Predictor> _xgb_predictor;

        // Constructor
        XgbPredictor();

        // Destructor
        ~XgbPredictor();

        // Get the size of weights (coefficients) which is not supported by tree based model
        uint64_t weight_size() override;

        // score the input encoded feature data
        float score(const Eigen::SparseVector<float> &sample) override;

        // Load model from file
        static unique_ptr<XgbPredictor> load_model(const string model_path);
    };

}


#endif //ALGOCOMP_GBDT_MODEL_H
