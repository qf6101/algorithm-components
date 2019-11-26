//
// Created by qfeng on 17-9-19.
//

#ifndef ALGOCOMP_FEATURE_MAP_H
#define ALGOCOMP_FEATURE_MAP_H

#include <string>
#include <spdlog/logger.h>
#include "feature_encoder.h"
#include "dense_hash_map"

using namespace std;

namespace algocomp {

    /**
     * Feature map
     *
     * Define context of feature map (one kind of feature encoder)
     */
    class FeatureMap : public FeatureEncoder {
    public:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Path of feature map file
        string _fmap_path;

        // Feature map
        google::dense_hash_map<string, int> _kv;

        // Constructor
        FeatureMap(const string fmap_path);

        // Update the feature size
        void update(uint64_t size);

        // Constant feature encode with predefined feature size (especially for intercept)
        uint64_t encode_constant() const;

        // Encode single feature
        uint64_t encode_single_feature(const string &gname, const string &fname) const;

        // Encode quadratic features with raw strings
        uint64_t encode_quadratic_features(const string &lhs_gname, const string &lhs_fname,
                                           const string &rhs_gname, const string &rhs_fname) const;

        // Encode quadratic features with previous encoded single features (do not support in feature map)
        uint64_t encode_quadratic_features(const uint64_t l_hash, const uint64_t r_hash) const;

        // Load feature map from file
        static tuple<google::dense_hash_map<string, int>, uint64_t> load(const string fmap_path);
    };

}

#endif //ALGOCOMP_FEATURE_MAP_H
