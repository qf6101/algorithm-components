//
// Created by AIYOJ on 2019-03-04.
//


#include "gtest/gtest.h"
#include <iostream>
#include "calibration/hybrid.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


namespace algocomp_test {

    class TestHybrid : public ::testing::Test {
    protected:
        std::unique_ptr<algocomp::Hybrid> calibration;

        virtual void SetUp() {
            calibration.reset(new algocomp::Hybrid());
        }

        virtual void TearDown() {

        }
    };

    TEST_F(TestHybrid, hybrid) {
        calibration->update(1);
        std::vector<float> scores;
        scores.push_back(0.8);
        scores.push_back(0.9);
        scores.push_back(0.4);

        std::vector<std::pair<float, float>> bins;
        bins.push_back(std::make_pair(0.2, 0.3));
        bins.push_back(std::make_pair(0.4, 0.6));
        bins.push_back(std::make_pair(0.7, 0.8));
        bins.push_back(std::make_pair(0.9, 0.9));
        calibration->update(bins);
        std::vector<float> calibrated_scores = calibration->calibrate(scores);
        for (int i = 0; i < scores.size(); ++i) {
            EXPECT_EQ(calibrated_scores[i], calibration->get(scores[i]));
        }

        calibration->update(0.5);
        scores.clear();
        scores.push_back(0.5);
        scores.push_back(0.7);
        scores.push_back(0.8);
        calibrated_scores = calibration->calibrate(scores);
        for (int i = 0; i < scores.size(); ++i) {
            float score = scores[i] / (scores[i] + (1 - scores[i]) * 2);
            EXPECT_EQ(calibrated_scores[i], calibration->get(score));
        }
    }

    TEST_F(TestHybrid, load_hybrid) {
        calibration = algocomp::Hybrid::load("test/test-data/1-calibrations/1-prior-correction.txt",
                                             "test/test-data/1-calibrations/1-isotonic-regression.txt");

        std::vector<float> scores;
        scores.push_back(0.8);
        scores.push_back(0.9);
        scores.push_back(0.4);

        std::vector<float> correct_calibrated_scores;
        std::for_each(scores.begin(), scores.end(), [this, &correct_calibrated_scores](const float &v) {
            float score = v / (v + (1 - v) * 2);
            correct_calibrated_scores.push_back(calibration->get(score));
        });

        std::vector<float> calibrated_scores = calibration->calibrate(scores);
        for (int i = 0; i < scores.size(); ++i) {
            EXPECT_EQ(calibrated_scores[i], correct_calibrated_scores[i]);
        }
    }
}
