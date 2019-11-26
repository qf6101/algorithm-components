//
// Created by AIYOJ on 2019-03-02.
//

#ifndef ALGORITHM_COMPONENTS_CALIBRATION_H
#define ALGORITHM_COMPONENTS_CALIBRATION_H

#include <vector>

namespace algocomp {
    class Calibration {
    public:
        virtual std::vector<float> calibrate(const std::vector<float> &scores) = 0;

        virtual float calibrate(float score) = 0;

        virtual ~Calibration() = default;
    };
}


#endif //ALGORITHM_COMPONENTS_CALIBRATION_H
