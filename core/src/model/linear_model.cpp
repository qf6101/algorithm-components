//
// Created by qfeng on 17-5-25.
//

#include "model/linear_model.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "log/algocomp_log.h"
#include "encode/feature_hash.h"
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace boost::algorithm;

namespace algocomp {

    LinearModel::LinearModel(uint64_t weight_size) {
        _weights.resize(weight_size);
        _logger = spdlog::get(ACLog::_logger_name);
        _logger->info("Initialize linear model with weight_size={}.", weight_size);
    }

    LinearModel::~LinearModel() {
        _weights.setZero();
        _logger->info("Leave linear model.");
    }

    uint64_t LinearModel::weight_size() {
        return _weights.size();
    }

    float LinearModel::get(uint64_t index) {
        return _weights.coeff(index);
    }

    void LinearModel::update_weight(uint64_t index, float value) {
        if (value != 0) _weights.coeffRef(index) = value;
    }

    float LinearModel::score(const Eigen::SparseVector<float> &sample) {
        // Eigen dot func will assert size of two vector
        return _bias + _weights.dot(sample);
    }

    unique_ptr<LinearModel> LinearModel::load_vw_model(const string &model_path) {
        auto logger = spdlog::get(ACLog::_logger_name);
        try {
            uint64_t constant_hash_code = 0;
            ifstream model_file;
            model_file.open(model_path);
            if (model_file.is_open()) {
                unique_ptr<LinearModel> output_model;
                string line;
                bool start_reading_weights = false;
                while (!model_file.eof()) {
                    getline(model_file, line);
                    if (start_reading_weights && line.length() > 0) {
                        vector<string> splits;
                        boost::algorithm::split(splits, line, boost::algorithm::is_any_of(":"));
                        uint64_t index = boost::lexical_cast<uint64_t>(splits[0]);
                        float value = boost::lexical_cast<float>(splits[1]);
                        // Get the value of intercept
                        if (index == constant_hash_code) output_model->_bias = value;
                        else output_model->update_weight(index, value);
                    } else if (boost::algorithm::starts_with(line, "bits")) {
                        vector<string> splits;
                        boost::algorithm::split(splits, line, boost::algorithm::is_any_of(":"));
                        uint64_t bits = boost::lexical_cast<uint64_t>(splits[1]);
                        uint64_t hash_size = 1 << bits;
                        output_model = unique_ptr<LinearModel>(new LinearModel(hash_size));
                        // Get the index of intercept
                        constant_hash_code = FeatureHash::hash_constant(hash_size);
                    } else if (boost::algorithm::starts_with(line, ":")) {
                        start_reading_weights = true;
                    }
                }
                if (model_file.is_open()) model_file.close();
                if (output_model) {
                    logger->info("Natively loaded vw linear model from {}.", model_path);
                    return output_model;
                } else logger->error("Fail to read model file when natively loading vw linear model from {}.", model_path);
            } else logger->error("Fail to open model file when natively loading vw linear model from {}.", model_path);
        } catch (exception &ex) {
            logger->error("Fail to natively load vw linear model from {}, detail: {}.", model_path, ex.what());
        }
        return nullptr;
    }

    unique_ptr<LinearModel> LinearModel::load_skl_model(const string &model_path) {
        auto logger = spdlog::get(ACLog::_logger_name);
        try {
            ifstream model_file;
            model_file.open(model_path);
            if (model_file.is_open()) {
                unique_ptr<LinearModel> output_model = unique_ptr<LinearModel>(new LinearModel(pow(2, 30)));
                string line;
                while (!model_file.eof()) {
                    getline(model_file, line);
                    line = trim_copy(line);
                    if (line.length() == 0) continue;
                    vector<string> splits;
                    boost::algorithm::split(splits, line, boost::algorithm::is_any_of(" "));
                    if (splits.size() != 2) {
                        logger->error("Invalid skl model coef {}", line);
                        continue;
                    }

                    if (splits[0] == "0") {
                        float value = boost::lexical_cast<float>(splits[1]);
                        output_model->_bias = value;
                    } else {
                        uint64_t index = boost::lexical_cast<uint64_t>(splits[0]);
                        float value = boost::lexical_cast<float>(splits[1]);
                        output_model->update_weight(index, value);
                    }
                }
                if (model_file.is_open()) model_file.close();
                if (output_model) {
                    logger->info("Natively loaded skl linear model from {}.", model_path);
                    return output_model;
                } else logger->error("Fail to read model file when natively loading skl linear model from {}.", model_path);
            } else logger->error("Fail to open model file when natively loading skl linear model from {}.", model_path);
        } catch (exception &ex) {
            logger->error("Fail to natively load skl linear model from {}, detail: {}.", model_path, ex.what());
        }
        return nullptr;

    }

}
