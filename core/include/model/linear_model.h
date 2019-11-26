//
// Created by qfeng on 17-5-25.
//

#ifndef ALGOCOMP_LINEAR_MODEL_H
#define ALGOCOMP_LINEAR_MODEL_H

#include "model/model.h"
#include <Eigen/Sparse>
#include <memory>
#include <boost/optional.hpp>
#include <spdlog/logger.h>

using boost::optional;

using namespace std;

namespace algocomp {

    /**
     * Linear Model
     *
     * Define content of linear model (e.g., intercept and weights)
     */
    class LinearModel : public Model {
    public:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Intercept
        float _bias = 0;

        // Weights (coefficients)
        Eigen::SparseVector<float> _weights;

        // Constructor
        LinearModel(uint64_t weight_size);

        // Destructor
        virtual ~LinearModel();

        // Get the size of weights (coefficients)
        uint64_t weight_size() override;

        // Get the weight of index position
        float get(uint64_t index);

        // Update the weight of index position
        void update_weight(uint64_t index, float value);

        // Score the input encoded feature data
        float score(const Eigen::SparseVector<float> &sample) override;

        // Load vowpal wabbit model
        static unique_ptr<LinearModel> load_vw_model(const string &model_path);

        // Load sk-learn linear model
        static unique_ptr<LinearModel> load_skl_model(const string &model_path);
    };

}


#endif //ALGOCOMP_LINEAR_MODEL_H
