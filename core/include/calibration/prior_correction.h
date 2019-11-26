//
// Created by AIYOJ on 2019-03-02.
//

#ifndef ALGORITHM_COMPONENTS_PRIOR_CORRECTION_H
#define ALGORITHM_COMPONENTS_PRIOR_CORRECTION_H

#include "calibration/calibration.h"
#include <vector>
#include <memory>
#include <boost/optional.hpp>
#include <spdlog/logger.h>


namespace algocomp {
    class PriorCorrection : public Calibration {
    public:
        std::shared_ptr<spdlog::logger> _logger;

        PriorCorrection();

        virtual ~PriorCorrection();

        float _neg_smp_r = 1;

        void update(float neg_smp_r);

        std::vector<float> calibrate(const std::vector<float> &scores) override;

        float calibrate(float score) override;

        static std::unique_ptr<PriorCorrection> load(const std::string &path);
    };
}

#endif //ALGORITHM_COMPONENTS_PRIOR_CORRECTION_H
