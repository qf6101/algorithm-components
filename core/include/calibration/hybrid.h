//
// Created by AIYOJ on 2019-03-04.
//

#ifndef ALGORITHM_COMPONENTS_HYBRID_H
#define ALGORITHM_COMPONENTS_HYBRID_H

#include "calibration/calibration.h"
#include "log/algocomp_log.h"


namespace algocomp {
    class Hybrid : public Calibration {
    public:
        enum class SearchType {
            UPPERBOUND = 0,
            LOWERBOUND = 1
        };

        SearchType _search_type = SearchType::UPPERBOUND;

        std::shared_ptr<spdlog::logger> _logger;

        std::vector<std::pair<float, float>> _bins;

        float _neg_smp_r = 1.0;

        Hybrid();

        virtual ~Hybrid();

        void update(const std::vector<std::pair<float, float>> &bins);

        void update(float neg_smp_r);

        float get(float bin, const SearchType search_type = SearchType::UPPERBOUND);

        std::vector<float> calibrate(const std::vector<float> &scores) override;

        float calibrate(float score) override;

        static std::unique_ptr<Hybrid> load(const std::string &path1, const std::string &path2);

    };
}

#endif //ALGORITHM_COMPONENTS_HYBRID_H
