//
// Created by qfeng on 17-10-13.
//

#ifndef QUICKSCORER_QS_MODEL_BLOCK_H
#define QUICKSCORER_QS_MODEL_BLOCK_H

#include "conf/config.h"
#include <Eigen/Sparse>
#include <vector>

using namespace std;

namespace quickscorer {

    class QSModelBlock {
    public:
        shared_ptr<spdlog::logger> _logger;

        QSModelBlock();

        // calculate the score wrt logistic loss
        float predict(const Eigen::SparseVector<float> &features);

        // calculate the batch scores wrt logistic loss
        vector<float> raw_predict(const vector<Eigen::SparseVector<float>> &instances);

        // calculate the margin
        virtual float raw_predict(const Eigen::SparseVector<float> &features) = 0;

        // calculate the index of highest one bit
        uint32_t highest_one_bit_index(BANDWITH_TYPE n);
    };

}

#endif //QUICKSCORER_QS_MODEL_BLOCK_H
