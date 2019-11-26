//
// Created by qfeng on 17-5-22.
//

#include <cstdint>
#include "log/algocomp_log.h"
#include "encode/feature_hash.h"
#include "constant.h"
#include "hash.h"

namespace algocomp {

    FeatureHash::FeatureHash(uint64_t size) {
        _size = size;
        _logger = spdlog::get(ACLog::_logger_name);
        _logger->info("Initialize feature hash with size of {}", _size);
    }

    void FeatureHash::update(uint64_t size) {
        if (size > 0) {
            auto old_size = _size;
            _size = size;
            _logger->info("Finish resizing feature hash from {} to {}", old_size, _size);
        } else _logger->info("Feature hash size is unchanged ({}) due to zero input size.", _size);
    }

    uint64_t FeatureHash::encode_constant() const {
        return FeatureHash::hash_constant(_size);
    }

    uint64_t FeatureHash::hash_group(const string &gname) const {
        if (gname.size() > 0) {
            return hash_string(gname, hash_base);
        } else return 0;
    }

    uint64_t FeatureHash::encode_single_feature(const string &ns, const string &feature) const {
        if (feature.size() > 0) {
            uint64_t ns_hash = hash_group(ns);
            return hash_string(feature, ns_hash) % (_size);
        } else return 0;
    }

    uint64_t FeatureHash::encode_quadratic_features(const string &lhs_gname,
                                                    const string &lhs_fname,
                                                    const string &rhs_gname,
                                                    const string &rhs_fname) const {
        uint64_t lhs_hash = encode_single_feature(lhs_gname, lhs_fname);
        uint64_t rhs_hash = encode_single_feature(rhs_gname, rhs_fname);
        return ((lhs_hash * FNV_prime) ^ rhs_hash) % (_size);
    }

    uint64_t FeatureHash::encode_quadratic_features(const uint64_t l_hash, const uint64_t r_hash) const {
        return ((l_hash * FNV_prime) ^ r_hash) % (_size);
    }

    uint64_t FeatureHash::hash_string(const string &s, uint64_t h) const {
        auto iter = s.begin();
        //trim leading whitespace but not UTF-8
        for (; iter < s.end() && *iter <= 0x20 && (int) *iter >= 0; iter++);
        iter = s.end();
        //trim trailing white space but not UTF-8
        for (; iter > s.begin() && *(iter - 1) <= 0x20 && (int) *(iter - 1) >= 0; iter--);

        size_t ret = 0;
        iter = s.begin();
        while (iter != s.end())
            if (*iter >= '0' && *iter <= '9')
                ret = 10 * ret + *(iter++) - '0';
            else
                return uniform_hash((unsigned char *) s.c_str(), s.length(), h);

        return ret + h;
    }

    uint64_t FeatureHash::hash_constant(uint64_t size) {
        return bias_constant % (size);
    }

}
