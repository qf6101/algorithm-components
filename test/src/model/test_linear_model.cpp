//
// Created by qfeng on 2019-01-10.
//


#include "gtest/gtest.h"
#include <iostream>
#include "model/linear_model.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


using namespace std;
using namespace algocomp;

namespace algocomp_test {

    class TestLinearModel : public ::testing::Test {
    protected:
        unique_ptr<LinearModel> model;

        virtual void SetUp() {
            model.reset(new LinearModel(pow(2, 18)));
        }

        virtual void TearDown() {

        }
    };

    TEST_F(TestLinearModel, linear_model) {
        model->update_weight(0, 0.032);
        model->update_weight(1, 0.125);
        model->update_weight(3, -0.0265);
        model->update_weight(6, 0);
        Eigen::SparseVector<float> sample(pow(2, 18));
        sample.coeffRef(0) = 0.683;
        sample.coeffRef(1) = -0.0234;
        sample.coeffRef(2) = 0.435;
        sample.coeffRef(3) = 0.112;
        sample.coeffRef(4) = -0.087;
        sample.coeffRef(5) = 0.123;
        sample.coeffRef(6) = -0.1345;
        sample.coeffRef(8) = 0.1;
        float score = model->score(sample);
        EXPECT_NEAR(score, 0.015963003, 1E-7);
    }

    TEST_F(TestLinearModel, skl_model) {
        auto model = LinearModel::load_skl_model("test/test-data/1-models/1-skl-model.txt");

        ifstream sample_file;
        sample_file.open("test/test-data/1-models/1-skl-sample.txt");
        if (sample_file.is_open()) {
            Eigen::SparseVector<float> kv(int(pow(2,30)));
            string line;
            while (!sample_file.eof()) {
                getline(sample_file, line);
                if (line.length() > 0) {
                    vector<string> splits;
                    boost::algorithm::split(splits, line, boost::algorithm::is_any_of(" "));
                    uint64_t index = boost::lexical_cast<uint64_t>(splits[0]);
                    float value = boost::lexical_cast<float>(splits[1]);
                    kv.insert(index) = value;
                }
            }

            sample_file.close();
            auto score = model->score(kv);
            auto pred = 1.0 / (1.0 + exp(-score));
            EXPECT_NEAR(pred, 0.450796238486696, 1E-3);
        }
    }

}