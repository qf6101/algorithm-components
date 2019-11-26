//
// Created by AIYOJ on 2019-03-02.
//

#ifndef ALGORITHM_COMPONENTS_ISOTONIC_REGRESSION_H
#define ALGORITHM_COMPONENTS_ISOTONIC_REGRESSION_H

#include "calibration/calibration.h"
#include <vector>
#include <memory>
#include <boost/optional.hpp>
#include <spdlog/logger.h>


namespace algocomp {
    class IsotonicRegression : public Calibration {
    public:
        enum class SearchType {
            UPPERBOUND = 0,
            LOWERBOUND = 1
        };

        SearchType _search_type = SearchType::UPPERBOUND;

        std::shared_ptr<spdlog::logger> _logger;

        std::vector<std::pair<float, float>> _bins;

        IsotonicRegression();

        virtual ~IsotonicRegression();

        void update(const std::vector<std::pair<float, float>> &bins);

        float get(float bin, const SearchType search_type = SearchType::UPPERBOUND);

        std::vector<float> calibrate(const std::vector<float> &scores) override;

        float calibrate(float score) override;

        static std::unique_ptr<IsotonicRegression> load(const std::string &path);

    };
}

#endif //ALGORITHM_COMPONENTS_ISOTONIC_REGRESSION_H
