//
// Created by qfeng on 2019-01-10.
//


#include "gtest/gtest.h"
#include <iostream>
#include "pipeline/pl_comp_pool.h"
#include <google/protobuf/util/json_util.h>
#include "util/check_feat_exists.h"
#include "base64.h"

using namespace std;
using namespace algocomp;

namespace algocomp_test {

    class TestReadXML : public ::testing::Test {
    protected:
        virtual void SetUp() {

        }

        virtual void TearDown() {

        }
    };

    TEST_F(TestReadXML, resource) {
        auto rsrc = ResourceManager("1-resource.xml", "test/test-data");
        EXPECT_TRUE(rsrc.Started());
        auto attr_transer = rsrc.get_attr_transer("attr-trans-1");
        EXPECT_TRUE(attr_transer != nullptr);
        auto feat_transer = rsrc.get_feat_transer("feat-trans-1");
        EXPECT_TRUE(feat_transer != nullptr);
        auto feat_enc = rsrc.get_feat_encoder("feat-hash-1");
        EXPECT_TRUE(feat_enc != nullptr);
    }

    TEST_F(TestReadXML, pl_pool) {
        auto pool = PLCompPool("test/test-data/1-pipeline.xml");
        auto plh = pool.get_plhead("pl-head-1");
        EXPECT_TRUE(plh != nullptr);
        auto plm = pool.get_plmiddle("pl-middle-1");
        EXPECT_TRUE(plm != nullptr);
        auto pl = pool.get_pipeline("pl-1");
        EXPECT_TRUE(pl != nullptr);
    }

}