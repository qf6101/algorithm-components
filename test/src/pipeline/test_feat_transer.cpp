//
// Created by qfeng on 2019-01-10.
//


#include "gtest/gtest.h"
#include <iostream>
#include "feature/feat_transer.h"
#include "feature/bucketize_op.h"
#include "feature/log_op.h"
#include "feature/cross_op.h"
#include <google/protobuf/util/json_util.h>
#include "util/check_feat_exists.h"

using namespace std;
using namespace algocomp;

namespace algocomp_test {

    class TestFeatTranser : public ::testing::Test {
    protected:
        shared_ptr<ctpl::thread_pool> _thp;
        shared_ptr<FeatTranser> _feat_transer;
        google::protobuf::Arena _arena;
        AUCImprFeats inp_feats;
        ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp>
                *uc_conts = inp_feats.mutable_user_and_ctx()->mutable_cont_feats();
        ::google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp>
                *uc_disces = inp_feats.mutable_user_and_ctx()->mutable_disc_feats();

        FeatureBunch *ad = inp_feats.mutable_ads()->Add();
        ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp>
                *ad_conts = ad->mutable_cont_feats();
        ::google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp>
                *ad_disces = ad->mutable_disc_feats();


        virtual void SetUp() {
            _thp = make_shared<ctpl::thread_pool>(5, 100);
            map<string, shared_ptr<FeatureOperator>> _feat_ops;
            _feat_ops["bucketize"] = make_shared<BucketizeOp>();
            _feat_ops["log"] = make_shared<LogOp>();
            _feat_ops["cross"] = make_shared<CrossOp>();
            _feat_ops["crossall"] = make_shared<CrossOp>();
            _feat_ops["comp"] = make_shared<CrossOp>();

            _feat_transer = make_shared<FeatTranser>("1-feat-trans.xml", "test/test-data", _thp, _feat_ops,
                                                     ThreadType::NO);
        }

        virtual void TearDown() {
            _feat_transer.reset();
        }
    };

    TEST_F(TestFeatTranser, uc_log) {

        ContFeatData ucost_feat;
        ucost_feat.set_gname("user_cost");
        ucost_feat.set_fname("user_cost");
        ucost_feat.set_fval(100);
        ContFeatGrp ucost_feat_grp;
        (*ucost_feat_grp.mutable_feats())["user_cost"] = ucost_feat;
        (*uc_conts)["user_cost"] = ucost_feat_grp;
        string inp_feats_json;
        google::protobuf::util::MessageToJsonString(inp_feats, &inp_feats_json);
        cout << inp_feats_json << endl;
        EXPECT_TRUE(_feat_transer->apply(inp_feats, _arena));

//        string inp_feats_json;
//        google::protobuf::util::MessageToJsonString(inp_feats, &inp_feats_json);
//        cout << inp_feats_json << endl;

        auto cont_feats = inp_feats.user_and_ctx().cont_feats();
        auto it_grp_found = cont_feats.find("user_cost#log");
        EXPECT_TRUE(it_grp_found != cont_feats.end());
        auto it_feat_found = it_grp_found->second.feats().find("user_cost#log");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "user_cost#log");
        EXPECT_TRUE(it_feat_found->second.fname() == "user_cost#log");
        EXPECT_NEAR(it_feat_found->second.fval(), 2, 1E-3);
    }

    TEST_F(TestFeatTranser, uc_bucketize) {

        ContFeatData uage_feat;
        uage_feat.set_gname("user_age");
        uage_feat.set_fname("user_age");
        uage_feat.set_fval(26);
        ContFeatGrp uage_feat_grp;
        (*uage_feat_grp.mutable_feats())["user_age"] = uage_feat;
        (*uc_conts)["user_age"] = uage_feat_grp;

        EXPECT_TRUE(_feat_transer->apply(inp_feats, _arena));

//        string inp_feats_json;
//        google::protobuf::util::MessageToJsonString(inp_feats, &inp_feats_json);
//        cout << inp_feats_json << endl;

        auto disc_feats = inp_feats.user_and_ctx().disc_feats();
        auto it_grp_found = disc_feats.find("user_age#bkt");
        EXPECT_TRUE(it_grp_found != disc_feats.end());
        auto it_feat_found = it_grp_found->second.feats().find("user_age#bkt@3");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "user_age#bkt");
        EXPECT_TRUE(it_feat_found->second.fname() == "user_age#bkt@3");
    }

    TEST_F(TestFeatTranser, uc_cross) {
        DiscFeatData usex_feat;
        usex_feat.set_gname("user_sex");
        usex_feat.set_fname("user_sex%male");
        DiscFeatGrp usex_feat_grp;
        (*usex_feat_grp.mutable_feats())["user_sex"] = usex_feat;
        (*uc_disces)["user_sex"] = usex_feat_grp;

        DiscFeatData uage_feat;
        uage_feat.set_gname("user_age#bkt");
        uage_feat.set_fname("user_age#bkt@1");
        DiscFeatGrp uage_feat_grp;
        (*uage_feat_grp.mutable_feats())["user_age#bkt"] = uage_feat;
        (*uc_disces)["user_age#bkt"] = uage_feat_grp;

        EXPECT_TRUE(_feat_transer->apply(inp_feats, _arena));

//        string inp_feats_json;
//        google::protobuf::util::MessageToJsonString(inp_feats, &inp_feats_json);
//        cout << inp_feats_json << endl;

        auto disc_feats = inp_feats.user_and_ctx().disc_feats();
        auto it_grp_found = disc_feats.find("!cross^user_age#bkt^user_sex");
        EXPECT_TRUE(it_grp_found != disc_feats.end());
        auto it_feat_found = it_grp_found->second.feats().find(
                "!cross^user_age#bkt@1^user_sex%male");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "!cross^user_age#bkt^user_sex");
        EXPECT_TRUE(it_feat_found->second.fname() == "!cross^user_age#bkt@1^user_sex%male");
    }

    TEST_F(TestFeatTranser, ad_log) {

        ContFeatData acost_feat;
        acost_feat.set_gname("ad_cost");
        acost_feat.set_fname("ad_cost");
        acost_feat.set_fval(100);
        ContFeatGrp acost_feat_grp;
        (*acost_feat_grp.mutable_feats())["ad_cost"] = acost_feat;
        (*ad_conts)["ad_cost"] = acost_feat_grp;

        EXPECT_TRUE(_feat_transer->apply(inp_feats, _arena));

//        string inp_feats_json;
//        google::protobuf::util::MessageToJsonString(inp_feats, &inp_feats_json);
//        cout << inp_feats_json << endl;

        auto cont_feats = inp_feats.ads(0).cont_feats();
        auto it_grp_found = cont_feats.find("ad_cost#log");
        EXPECT_TRUE(it_grp_found != cont_feats.end());
        auto it_feat_found = it_grp_found->second.feats().find("ad_cost#log");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "ad_cost#log");
        EXPECT_TRUE(it_feat_found->second.fname() == "ad_cost#log");
        EXPECT_NEAR(it_feat_found->second.fval(), 4.605, 1E-3);
    }

    TEST_F(TestFeatTranser, ad_log_multi_thead4ads) {

        // ad0
        {
            ContFeatData acost_feat;
            acost_feat.set_gname("ad_cost");
            acost_feat.set_fname("ad_cost");
            acost_feat.set_fval(100);
            ContFeatGrp acost_feat_grp;
            (*acost_feat_grp.mutable_feats())["ad_cost"] = acost_feat;
            (*ad_conts)["ad_cost"] = acost_feat_grp;
        }

        // ad1
        {
            FeatureBunch *ad1 = inp_feats.mutable_ads()->Add();
            ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp>
                    *ad_conts1 = ad1->mutable_cont_feats();

            ContFeatData acost_feat1;
            acost_feat1.set_gname("ad_cost");
            acost_feat1.set_fname("ad_cost");
            acost_feat1.set_fval(456);
            ContFeatGrp acost_feat_grp1;
            (*acost_feat_grp1.mutable_feats())["ad_cost"] = acost_feat1;
            (*ad_conts1)["ad_cost"] = acost_feat_grp1;
            ::google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp>
                    *ad_disces1 = ad1->mutable_disc_feats();
        }

        // ad2
        {
            FeatureBunch *ad2 = inp_feats.mutable_ads()->Add();
            ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp>
                    *ad_conts2 = ad2->mutable_cont_feats();

            ContFeatData acost_feat2;
            acost_feat2.set_gname("ad_cost");
            acost_feat2.set_fname("ad_cost");
            acost_feat2.set_fval(456);
            ContFeatGrp acost_feat_grp2;
            (*acost_feat_grp2.mutable_feats())["ad_cost"] = acost_feat2;
            (*ad_conts2)["ad_cost"] = acost_feat_grp2;

            ::google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp>
                    *ad_disces2 = ad2->mutable_disc_feats();
        }

        {
            FeatureBunch *ad3 = inp_feats.mutable_ads()->Add();
            ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp>
                    *ad_conts3 = ad3->mutable_cont_feats();
            ::google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp>
                    *ad_disces3 = ad3->mutable_disc_feats();
        }

        {
            FeatureBunch *ad4 = inp_feats.mutable_ads()->Add();
            ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp>
                    *ad_conts4 = ad4->mutable_cont_feats();
            ::google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp>
                    *ad_disces4 = ad4->mutable_disc_feats();
        }

        {
            FeatureBunch *ad5 = inp_feats.mutable_ads()->Add();
            ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp>
                    *ad_conts5 = ad5->mutable_cont_feats();
            ::google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp>
                    *ad_disces5 = ad5->mutable_disc_feats();
        }

        EXPECT_TRUE(_feat_transer->apply(inp_feats, _arena));
//        print_message(inp_feats);

        auto cont_feats = inp_feats.ads(0).cont_feats();
//        print_message(inp_feats.ads(0));
        auto it_grp_found = cont_feats.find("ad_cost#log");
        EXPECT_TRUE(it_grp_found != cont_feats.end());
        auto it_feat_found = it_grp_found->second.feats().find("ad_cost#log");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "ad_cost#log");
        EXPECT_TRUE(it_feat_found->second.fname() == "ad_cost#log");
        EXPECT_NEAR(it_feat_found->second.fval(), 4.605, 1E-3);

    }

    TEST_F(TestFeatTranser, ad_comp) {

        ContFeatData ucost_feat;
        ucost_feat.set_gname("user_cost#log");
        ucost_feat.set_fname("user_cost#log");
        ucost_feat.set_fval(100);
        ContFeatGrp ucost_feat_grp;
        (*ucost_feat_grp.mutable_feats())["user_cost#log"] = ucost_feat;
        (*uc_conts)["user_cost#log"] = ucost_feat_grp;

        ContFeatData acost_feat;
        acost_feat.set_gname("ad_cost#log");
        acost_feat.set_fname("ad_cost#log");
        acost_feat.set_fval(123);
        ContFeatGrp acost_feat_grp;
        (*acost_feat_grp.mutable_feats())["ad_cost#log"] = acost_feat;
        (*ad_conts)["ad_cost#log"] = acost_feat_grp;

        EXPECT_TRUE(_feat_transer->apply(inp_feats, _arena));

//        string inp_feats_json;
//        google::protobuf::util::MessageToJsonString(inp_feats, &inp_feats_json);
//        cout << inp_feats_json << endl;

        check_ad_cont_feat(0, inp_feats, "!comp^user_cost#log^ad_cost#log",
                           "!comp$avg_max^user_cost#log^ad_cost#log", 123);
        check_ad_cont_feat(0, inp_feats, "!comp^user_cost#log^ad_cost#log",
                           "!comp$avg_min^user_cost#log^ad_cost#log", 100);
        check_ad_cont_feat(0, inp_feats, "!comp^user_cost#log^ad_cost#log",
                           "!comp$avg_dot^user_cost#log^ad_cost#log", 12300);
    }

    TEST_F(TestFeatTranser, ad_bucketize) {

        ContFeatData arevenue_feat;
        arevenue_feat.set_gname("ad_revenue");
        arevenue_feat.set_fname("ad_revenue");
        arevenue_feat.set_fval(-0.05);
        ContFeatGrp arevenue_feat_grp;
        (*arevenue_feat_grp.mutable_feats())["ad_revenue"] = arevenue_feat;
        (*ad_conts)["ad_revenue"] = arevenue_feat_grp;

        EXPECT_TRUE(_feat_transer->apply(inp_feats, _arena));

//        string inp_feats_json;
//        google::protobuf::util::MessageToJsonString(inp_feats, &inp_feats_json);
//        cout << inp_feats_json << endl;

        check_ad_disc_feat(0, inp_feats, "ad_revenue#bkt", "ad_revenue#bkt@4");
    }

    TEST_F(TestFeatTranser, cross_all) {
        {
            DiscFeatData feat1;
            feat1.set_gname("feat1_grp");
            feat1.set_fname("feat1");
            DiscFeatGrp feat1_grp;
            (*feat1_grp.mutable_feats())["feat1"] = feat1;
            (*uc_disces)["feat1_grp"] = feat1_grp;
        }

        {
            DiscFeatData feat2;
            feat2.set_gname("feat2_grp");
            feat2.set_fname("feat2");
            DiscFeatGrp feat2_grp;
            (*feat2_grp.mutable_feats())["feat2"] = feat2;
            (*uc_disces)["feat2_grp"] = feat2_grp;
        }

        {
            DiscFeatData feat3;
            feat3.set_gname("feat3_grp");
            feat3.set_fname("feat3");
            DiscFeatGrp feat3_grp;
            (*feat3_grp.mutable_feats())["feat3"] = feat3;
            (*ad_disces)["feat3_grp"] = feat3_grp;
        }

        {
            DiscFeatData feat4;
            feat4.set_gname("feat4_grp");
            feat4.set_fname("feat4");
            DiscFeatGrp feat4_grp;
            (*feat4_grp.mutable_feats())["feat4"] = feat4;
            (*ad_disces)["feat4_grp"] = feat4_grp;
        }

        EXPECT_TRUE(_feat_transer->apply(inp_feats, _arena));

//        string inp_feats_json;
//        google::protobuf::util::MessageToJsonString(inp_feats, &inp_feats_json);
//        cout << inp_feats_json << endl;

        check_uc_disc_feat(inp_feats, "!crossall^feat1_grp^feat2_grp", "!crossall$uc^feat1^feat2");
        check_ad_disc_feat(0, inp_feats, "!crossall^feat3_grp^feat4_grp", "!crossall$ad^feat3^feat4");
        check_ad_disc_feat(0, inp_feats, "!crossall^feat2_grp^feat3_grp", "!crossall$ad2uc^feat2^feat3");
        check_ad_disc_feat(0, inp_feats, "!crossall^feat2_grp^feat4_grp", "!crossall$ad2uc^feat2^feat4");
    }
}

