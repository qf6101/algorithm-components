//
// Created by qfeng on 17-9-19.
//

#include "encode/feature_map.h"
#include "log/algocomp_log.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace algocomp {

    FeatureMap::FeatureMap(const string fmap_path) {
        _logger = spdlog::get(ACLog::_logger_name);
        _fmap_path = fmap_path;
        _kv.set_empty_key("");
        _logger->info("Initialize feature map with path of {}", _fmap_path);
    }

    void FeatureMap::update(uint64_t size) {
        auto created = FeatureMap::load(_fmap_path);
        _kv.clear();
        _kv = std::get<0>(created);
        _size = std::get<1>(created);
        if (size > _size) _size = size;
        _logger->info("Load feature map from {} with size of {} (input size {})", _fmap_path, _size, size);
    }

    tuple<google::dense_hash_map<string, int>, uint64_t> FeatureMap::load(const string fmap_path) {
        auto logger = spdlog::get(ACLog::_logger_name);
        google::dense_hash_map<string, int> kv;
        kv.set_empty_key("");
        try {
            ifstream fmap_file;
            fmap_file.open(fmap_path);
            if (fmap_file.is_open()) {
                string line;
                uint64_t max = 0;
                while (!fmap_file.eof()) {
                    getline(fmap_file, line);
                    if (line.length() > 0) {
                        vector<string> splits;
                        boost::algorithm::split(splits, line, boost::algorithm::is_any_of("\t"));
                        if (splits.size() < 2) continue;
                        int encoding = boost::lexical_cast<int>(splits[0]);
                        string ns_and_fname = splits[1];
                        kv[ns_and_fname] = encoding;
                        if (max < encoding) max = encoding;
                    }
                }
                if (fmap_file.is_open()) fmap_file.close();
                if (kv.size() == 0) logger->error("Empty feature map.");
                logger->info("Finish reload feature map with size of {}", max);
                return make_tuple(kv, max + 1);
            } else logger->error("Fail to open feature map file when loading from {}.", fmap_path);
        } catch (exception &ex) {
            logger->error("Fail to load feature map from {}, detail: {}.", fmap_path, ex.what());
        }
        return make_tuple(kv, 0);
    }

    uint64_t FeatureMap::encode_constant() const {
        return UINT64_MAX;
    }

    uint64_t FeatureMap::encode_single_feature(const string &gname, const string &fname) const {
        auto key = gname + "^" + fname;
        const auto &iter = _kv.find(key);
        if (iter != _kv.end()) return iter->second;
        else return 0;
    }

    uint64_t
    FeatureMap::encode_quadratic_features(const string &lhs_gname, const string &lhs_fname, const string &rhs_gname,
                                          const string &rhs_fname) const {
        auto key = lhs_gname + "^" + lhs_fname + "_" + rhs_gname + "^" + rhs_fname;
        const auto &iter = _kv.find(key);
        if (iter != _kv.end()) return iter->second;
        else return 0;
    }

    uint64_t FeatureMap::encode_quadratic_features(const uint64_t l_hash, const uint64_t r_hash) const {
        // just for implement virtual fun.
        return 0;
    }

}
