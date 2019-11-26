//
// Created by AIYOJ on 2019-03-04.
//

#include "gtest/gtest.h"
#include <iostream>
#include "calibration/prior_correction.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


namespace algocomp_test {

    class TestPriorCorrection : public ::testing::Test {
    protected:
        std::unique_ptr<algocomp::PriorCorrection> calibration;

        virtual void SetUp() {
            calibration.reset(new algocomp::PriorCorrection());
        }

        virtual void TearDown() {

        }
    };

    TEST_F(TestPriorCorrection, prior_correction) {
        calibration->update(1);
        std::vector<float> scores;
        scores.push_back(0.8);
        scores.push_back(0.9);
        scores.push_back(0.4);
        std::vector<float> calibrated_scores = calibration->calibrate(scores);
        for (int i = 0; i < scores.size(); ++i) {
            EXPECT_EQ(calibrated_scores[i], scores[i]);
        }

        calibration->update(0.5);
        scores.clear();
        scores.push_back(0.5);
        scores.push_back(0.7);
        scores.push_back(0.8);
        calibrated_scores = calibration->calibrate(scores);
        for (int i = 0; i < scores.size(); ++i) {
            float score = scores[i] / (scores[i] + (1 - scores[i]) * 2);
            EXPECT_EQ(calibrated_scores[i], score);
        }
    }

    TEST_F(TestPriorCorrection, load_prior_correction){
        calibration = algocomp::PriorCorrection::load("test/test-data/1-calibrations/1-prior-correction.txt");

        std::vector<float> scores;
        scores.push_back(0.8);
        scores.push_back(0.9);
        scores.push_back(0.4);
        std::vector<float> calibrated_scores = calibration->calibrate(scores);
        for (int i = 0; i < scores.size(); ++i) {
            float score = scores[i] / (scores[i] + (1 - scores[i]) * 2);
            EXPECT_EQ(calibrated_scores[i], score);
        }
    }
}

