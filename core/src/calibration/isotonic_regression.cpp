//
// Created by AIYOJ on 2019-03-02.
//

#include "log/algocomp_log.h"
#include "calibration/isotonic_regression.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


namespace algocomp {
    IsotonicRegression::IsotonicRegression() {
        _logger = spdlog::get(ACLog::_logger_name);
        _logger->info("Initialize Isotonic Regression.");
    }

    IsotonicRegression::~IsotonicRegression() {
        _logger->info("Leave Isotonic Regression.");
    }

    std::vector<float> IsotonicRegression::calibrate(const std::vector<float> &scores) {
        std::vector<float> calibrated_scores;
        for (int i = 0; i < scores.size(); ++i) {
            auto calibrated_score = get(scores[i], _search_type);
            calibrated_scores.push_back(calibrated_score);
        }

        return calibrated_scores;
    }

    float IsotonicRegression::calibrate(float score) {
        float calibrated_score = get(score, _search_type);
        return calibrated_score;
    }

    std::unique_ptr<IsotonicRegression> IsotonicRegression::load(const std::string &path) {
        auto logger = spdlog::get(ACLog::_logger_name);

        try {
            std::ifstream file;
            file.open(path);
            if (file.is_open()) {
                std::unique_ptr<IsotonicRegression> output_calibration = std::unique_ptr<IsotonicRegression>(
                        new IsotonicRegression());
                std::string line;
                float pre = 0;
                bool order = true;
                std::vector<std::pair<float, float >> tmp_bins;
                while (!file.eof()) {
                    std::getline(file, line);
                    line = boost::algorithm::trim_copy(line);
                    if (line.length() == 0) {
                        continue;
                    }

                    std::vector<std::string> splits;
                    boost::algorithm::split(splits, line, boost::algorithm::is_any_of(" "));

                    if (splits.size() != 2) {
                        logger->error("Invalid isotonic regression coef {}", line);
                        continue;
                    }

                    float bin = boost::lexical_cast<float>(splits[0]);
                    float value = boost::lexical_cast<float>(splits[1]);

                    if (bin > pre) {
                        order = true;
                        pre = bin;
                    } else if (bin < pre) {
                        order = false;
                        pre = bin;
                    } else {
                        logger->error("the same bin: {}", bin);
                        break;
                    }

                    tmp_bins.push_back(std::make_pair(bin, value));
                }

                if (order) {
                    output_calibration->update(tmp_bins);
                } else {
                    // sort
                    std::sort(tmp_bins.begin(), tmp_bins.end(),
                              [](const std::pair<float, float> &a, const std::pair<float, float> &b) {
                                  return a.first < b.first;
                              });
                    output_calibration->update(tmp_bins);
                }
                if (file.is_open()) {
                    file.close();
                }
                if (output_calibration) {
                    logger->info("Natively loaded isotonic regression from {}.", path);
                    return output_calibration;
                } else {
                    logger->error("Fail to read calibration file when natively loading isotonic regression from {}.",
                                  path);
                }
            } else {
                logger->error(
                        "Fail to open isotonic regression file when natively loading isotonic regression from {}.",
                        path);
            }

        } catch (std::exception &ex) {
            logger->error("Fail to natively load isotonic regression from {}, detail: {}.", path, ex.what());
        }
        return nullptr;
    }

    void IsotonicRegression::update(const std::vector<std::pair<float, float>> &bins) {
        _bins.clear();
        _bins = bins;
    }

    float IsotonicRegression::get(float bin, const SearchType search_type) {
        // upper bound
        auto upper_bound = [this](float key) {
            int first = 0;
            int len = _bins.size() - 1;
            int half, middle;

            while (len > 0) {
                half = len >> 1;
                middle = first + half;
                // The median is greater than the key and is looked up in the left half of the sequence containing last.
                if (_bins[middle].first > key) {
                    len = half;
                } else {  // The median is less than or equal to the key and is looked up in the right half of the sequence.
                    first = middle + 1;
                    len = len - half - 1;
                }
            }
            return first;
        };

        // lower bound
        auto lower_bound = [this](float key) {
            int first = 0, middle;
            int half, len;
            len = _bins.size();

            while (len > 0) {
                half = len >> 1;
                middle = first + half;
                if (_bins[middle].first < key) {
                    first = middle + 1;
                    len = len - half - 1;
                } else {
                    len = half;
                }
            }
            return first;
        };

        // search type
        if (search_type == SearchType::UPPERBOUND) {
            auto index = upper_bound(bin);
            return _bins[index].second;
        } else if (search_type == SearchType::LOWERBOUND) {
            auto index = lower_bound(bin);
            return _bins[index].second;
        } else {
            _logger->error("Unknown Search Type.");
        }
    }
}
