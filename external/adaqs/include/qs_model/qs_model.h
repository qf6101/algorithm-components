//
// Created by qfeng on 17-9-26.
//

#ifndef QUICKSCORER_QS_MODEL_H
#define QUICKSCORER_QS_MODEL_H

#include <string>
#include "gbtree_model/gbtree_model.h"
#include "qs_model_block.h"

using namespace std;

namespace quickscorer {

    class QSModel {
    public:
        shared_ptr<spdlog::logger> _logger;
        uint64_t _feature_size = 0;
        // block-wise variant of QS
        vector<unique_ptr<QSModelBlock>> _model_blocks;

        QSModel();

        vector<float> predict(const vector<Eigen::SparseVector<float>> &instances, bool output_margin = false);

        float predict(const Eigen::SparseVector<float> &features, bool output_margin = false);

        static unique_ptr<QSModel>
        from_xgb_txt(string model_path, uint32_t block_size, bool adaptive = true, int num_trees = 0);
    };

}


#endif //QUICKSCORER_QS_MODEL_H
