//
// Created by qfeng on 17-10-13.
//

#include <log/qs_log.h>
#include "qs_model/qs_model_block.h"

namespace quickscorer {

    QSModelBlock::QSModelBlock() {
        _logger = spdlog::get(QSLog::_logger_name);
    }

    float QSModelBlock::predict(const Eigen::SparseVector<float> &features) {
        auto margin = raw_predict(features);
        return 1.0 / (1.0 + exp(-margin));
    }

    vector<float> QSModelBlock::raw_predict(const vector<Eigen::SparseVector<float>> &instances) {
        vector<float> results(instances.size());
        for (int i = 0; i < instances.size(); ++i) {
            try {
                auto score = raw_predict(instances[i]);
                results[i] += score;
            } catch (exception &ex) {
                _logger->error("Fail to predict instances of size {}, detail: {}", instances.size(), ex.what());
            }
        }
        return results;
    }

    uint32_t QSModelBlock::highest_one_bit_index(BANDWITH_TYPE n) {
        // adopted from https://rosettacode.org/wiki/Find_first_and_last_set_bit_of_a_long_integer#C
        uint32_t b = 1;
        if (!n) assert("Try to find the highest one bit in zero.");

#define step(x) if (n >= ((BANDWITH_TYPE)1) << x) b += x, n >>= x

#ifdef TWO_BYTES_BANDWITH
            step(8);
#elif defined(THREE_BYTES_BANDWITH)
            step(16);
            step(8);
#elif defined(FOUR_BYTES_BANDWITH)
        step(32);
        step(16);
        step(8);
#endif
        step(4);
        step(2);
        step(1);
#undef step

        return BANDWITH_BITS - b;
    }

}