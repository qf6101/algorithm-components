//
// Created by qfeng on 17-5-22.
//

#ifndef ALGOCOMP_FEATURE_HASHING_H
#define ALGOCOMP_FEATURE_HASHING_H

#include <cstdint>
#include <string>
#include <Eigen/Sparse>
#include <memory>
#include "feature_encoder.h"
#include <boost/optional.hpp>
#include "log/algocomp_log.h"

using namespace std;
using boost::optional;

namespace algocomp {

    /**
     * Feature hash
     *
     * Define context of feature hash (one kind of feature encoder)
     *
     */
    class FeatureHash : public FeatureEncoder {
    public:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Constructor
        FeatureHash(uint64_t size);

        // Update the feature size
        void update(uint64_t size) override;

        // Internal function for murmurhash3
        uint64_t hash_string(const string &s, uint64_t h) const;

        // Constant feature encode with predefined feature size (especially for intercept)
        uint64_t encode_constant() const override;

        // Hash feature group
        uint64_t hash_group(const string &gname) const;

        // Encode single feature
        uint64_t encode_single_feature(const string &ns, const string &feature) const;

        // Encode quadratic features with raw strings
        uint64_t encode_quadratic_features(const string &lhs_gname, const string &lhs_fname,
                                           const string &rhs_gname, const string &rhs_fname) const;

        // Encode quadratic features with previous encoded single features
        uint64_t encode_quadratic_features(const uint64_t l_hash, const uint64_t r_hash) const;

        // Constant feature encode with specified feature size (especially for intercept)
        static uint64_t hash_constant(uint64_t size);
    };

}

#endif //ALGOCOMP_FEATURE_HASHING_H
