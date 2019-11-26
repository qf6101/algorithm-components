//
// Created by qfeng on 17-11-2.
//

#ifndef ALGOCOMP_FEATURE_ENCODER_H
#define ALGOCOMP_FEATURE_ENCODER_H

#include <string>

using namespace std;

namespace algocomp {

    /**
     * Feature Encoder
     *
     * Define abstract functions of feature encoder (concrete classes are FeatureHash and FeatureMap).
     */
    class FeatureEncoder {
    protected:
        // Feature size (e.g., 2^30)
        uint64_t _size = 0;
    public:
        // Feature size to outer
        uint64_t size() const { return _size; };

        // Update the feature size
        virtual void update(uint64_t size) = 0;

        // Constant feature encode (especially for intercept)
        virtual uint64_t encode_constant() const = 0;

        // Encode single feature
        virtual uint64_t encode_single_feature(const string &ns, const string &feat_name) const = 0;

        // Encode quadratic features with raw strings
        virtual uint64_t encode_quadratic_features(const string &lhs_gname, const string &lhs_fname,
                                                   const string &rhs_gname, const string &rhs_fname) const = 0;

        // Encode quadratic features with previous encoded single features
        virtual uint64_t encode_quadratic_features(const uint64_t l_hash, const uint64_t r_hash) const = 0;

        // Destructor
        virtual ~FeatureEncoder() = default;
    };

}

#endif //ALGOCOMP_FEATURE_ENCODER_H
