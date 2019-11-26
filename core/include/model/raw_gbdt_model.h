//
// Created by qfeng on 17-11-3.
//

#ifndef PCTR_RAW_GBDT_MODEL_H
#define PCTR_RAW_GBDT_MODEL_H


#include <spdlog/logger.h>
#include <Eigen/Sparse>
#include <string>
#include "qs_model/qs_model.h"
#include "model.h"

using namespace std;

namespace algocomp {

    /**
     * Raw GBDT Model
     *
     * Define wrap of raw GBDT model
     */
    class RawGbdtModel : public Model {
    public:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Model
        unique_ptr<quickscorer::GBTreeModel> _gbtree;

        // Constructor
        RawGbdtModel();

        // Destructor
        ~RawGbdtModel();

        // Get the size of weights (coefficients) which is not supported by tree based model
        uint64_t weight_size() override;

        // score the input encoded feature data
        float score(const Eigen::SparseVector<float> &sample) override;

        // Load model from file
        static unique_ptr<RawGbdtModel> load_model(const string model_path);
    };

}


#endif //PCTR_RAW_GBDT_MODEL_H
