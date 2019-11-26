//
// Created by AIYOJ on 2019-03-04.
//


#include "gtest/gtest.h"
#include <iostream>
#include "calibration/isotonic_regression.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace algocomp_test {

    class TestIsotonicRegression : public ::testing::Test {
    protected:
        std::unique_ptr<algocomp::IsotonicRegression> calibration;

        virtual void SetUp() {
            calibration.reset(new algocomp::IsotonicRegression());
        }

        virtual void TearDown() {

        }
    };

    TEST_F(TestIsotonicRegression, isotonic_regression) {
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
    }

    TEST_F(TestIsotonicRegression, load_isotonic_regression) {
        calibration = algocomp::IsotonicRegression::load("test/test-data/1-calibrations/1-isotonic-regression.txt");

        std::vector<float> scores;
        scores.push_back(0.8);
        scores.push_back(0.9);
        scores.push_back(0.4);

        std::vector<float> correct_calibrated_scores;
        correct_calibrated_scores.push_back(0.15296713764868813);
        correct_calibrated_scores.push_back(0.29661971830985917);
        correct_calibrated_scores.push_back(0.02298094617097653);

        std::vector<float> calibrated_scores = calibration->calibrate(scores);
        for (int i = 0; i < scores.size(); ++i) {
            EXPECT_EQ(calibrated_scores[i], correct_calibrated_scores[i]);
        }
    }
}



