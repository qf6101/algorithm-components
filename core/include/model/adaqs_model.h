//
// Created by qfeng on 17-11-3.
//

#ifndef PCTR_ADAQS_MODEL_H
#define PCTR_ADAQS_MODEL_H


#include <spdlog/logger.h>
#include <Eigen/Sparse>
#include <string>
#include "qs_model/qs_model.h"
#include "model.h"

using namespace std;

namespace algocomp {

    /**
     * AdaQS Model
     *
     * Define wrap of AdaQS model
     */
    class AdaqsModel : public Model {
    public:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Model
        unique_ptr<quickscorer::QSModel> _adaqs;

        // Constructor
        AdaqsModel();

        // Destructor
        ~AdaqsModel();

        // Get the size of weights (coefficients) which is not supported by tree based model
        uint64_t weight_size() override;

        // score the input encoded feature data
        float score(const Eigen::SparseVector<float> &sample) override;

        // Load model from file
        static unique_ptr<AdaqsModel> load_model(const string model_path, const int block_size = 1000);
    };

}


#endif //PCTR_ADAQS_MODEL_H
