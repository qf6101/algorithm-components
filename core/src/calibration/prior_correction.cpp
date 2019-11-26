//
// Created by AIYOJ on 2019-03-02.
//


#include "calibration/prior_correction.h"
#include <fstream>
#include <boost/lexical_cast.hpp>
#include "log/algocomp_log.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <exception>

namespace algocomp {
    PriorCorrection::PriorCorrection() {
        _logger = spdlog::get(ACLog::_logger_name);
        _logger->info("Initialize Prior Correction.");
    }

    PriorCorrection::~PriorCorrection() {
        _logger->info("Leave Prior Correction.");
    }


    std::vector<float> PriorCorrection::calibrate(const std::vector<float> &scores) {
        std::vector<float> calibrated_scores;
        calibrated_scores.reserve(scores.size());
        std::for_each(scores.begin(), scores.end(), [&calibrated_scores, this](float score) {
            float calibrated_score = score / (score + (1 - score) * (1 / _neg_smp_r));
            calibrated_scores.push_back(calibrated_score);
        });

        return calibrated_scores;
    }


    float PriorCorrection::calibrate(float score) {
        float calibrated_score = score / (score + (1 - score) * (1 / _neg_smp_r));
        return calibrated_score;
    }

    std::unique_ptr<PriorCorrection> PriorCorrection::load(const std::string &path) {
        auto logger = spdlog::get(ACLog::_logger_name);

        try {
            std::ifstream calibration_file;
            calibration_file.open(path);
            if (calibration_file.is_open()) {
                std::unique_ptr<PriorCorrection> output_calibration = std::unique_ptr<PriorCorrection>(
                        new PriorCorrection());
                std::string line;
                int read = 0;
                while (!calibration_file.eof()) {
                    std::getline(calibration_file, line);
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
                if (calibration_file.is_open()) {
                    calibration_file.close();
                }
                if (output_calibration) {
                    logger->info("Natively loaded prior correction from {}.", path);
                    return output_calibration;
                } else {
                    logger->error("Fail to read model file when natively loading prior correction from {}.", path);
                }

            } else {
                logger->error("Fail to open prior correction file when natively loading prior correction from {}.",
                              path);
            }

        } catch (std::exception &ex) {
            logger->error("Fail to natively load prior correction from {}, detail: {}.", path, ex.what());
        }
        return nullptr;
    }

    void PriorCorrection::update(float neg_smp_r) {
        _neg_smp_r = neg_smp_r;
    }
}
