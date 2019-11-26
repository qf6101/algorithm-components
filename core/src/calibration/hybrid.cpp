//
// Created by AIYOJ on 2019-03-04.
//

#include "calibration/hybrid.h"
#include "log/algocomp_log.h"
#include <exception>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <exception>


namespace algocomp {
    Hybrid::Hybrid() {
        _logger = spdlog::get(ACLog::_logger_name);
        _logger->info("Initialize Hybrid.");
    }

    Hybrid::~Hybrid() {
        _logger->info("Leave Hybrid.");
    }

    std::vector<float> Hybrid::calibrate(const std::vector<float> &scores) {
        std::vector<float> calibrated_scores;
        calibrated_scores.reserve(scores.size());
        std::for_each(scores.begin(), scores.end(), [&calibrated_scores, this](float score) {
            float calibrated_score = score / (score + (1 - score) * (1 / _neg_smp_r));
            calibrated_scores.push_back(calibrated_score);
        });

        std::for_each(calibrated_scores.begin(), calibrated_scores.end(), [this](float &score) {
            auto calibrated_score = get(score, _search_type);
            score = calibrated_score;
        });

        return calibrated_scores;
    }

    float Hybrid::calibrate(float score) {
        float score_ = score / (score + (1 - score) * (1 / _neg_smp_r));
        float calibrated_score = get(score_, _search_type);
        return calibrated_score;
    }

    std::unique_ptr<Hybrid> Hybrid::load(const std::string &path1, const std::string &path2) {
        auto logger = spdlog::get(ACLog::_logger_name);
        try {
            std::ifstream prior_correction_file;
            prior_correction_file.open(path1);

            std::ifstream isotonic_regression_file;
            isotonic_regression_file.open(path2);

            if (prior_correction_file.is_open() && isotonic_regression_file.is_open()) {
                std::unique_ptr<Hybrid> output_calibration = std::unique_ptr<Hybrid>(
                        new Hybrid());
                std::string line;
                int read = 0;
                while (!prior_correction_file.eof()) {
                    std::getline(prior_correction_file, line);
                    line = boost::algorithm::trim_copy(line);
                    if (line.length() == 0) {
                        continue;
                    }

                    float neg_smp_r = boost::lexical_cast<float>(line);
                    output_calibration->update(neg_smp_r);

                    read++;
                    if (read == 1) {
                        break;
                    }
                }

                float pre = 0;
                bool order = true;
                std::vector<std::pair<float, float >> tmp_bins;
                while (!isotonic_regression_file.eof()) {
                    std::getline(isotonic_regression_file, line);
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

                if (prior_correction_file.is_open()) {
                    prior_correction_file.close();
                }

                if (isotonic_regression_file.is_open()) {
                    isotonic_regression_file.close();
                }

                if (output_calibration) {

                    logger->info("Natively loaded hybrid from {} {}.", path1, path2);
                    return output_calibration;
                } else {
                    logger->error("Fail to read calibration file when natively loading hybrid from {} {}.",
                                  path1, path2);
                }
            } else {
                logger->error(
                        "Fail to open hybrid file when natively loading hybrid from {} {}.",
                        path1, path2);
            }
        } catch (std::exception &ex) {
            logger->error("Fail to natively load hybrid from {} {}, detail: {}.", path1, path2, ex.what());
        }
        return nullptr;
    }

    void Hybrid::update(const std::vector<std::pair<float, float>> &bins) {
        _bins.clear();
        _bins = bins;
    }

    void Hybrid::update(float neg_smp_r) {
        _neg_smp_r = neg_smp_r;
    }

    float Hybrid::get(float bin, const SearchType search_type) {
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