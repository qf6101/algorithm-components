//
// Created by qfeng on 2019-01-10.
//


#include "gtest/gtest.h"
#include <iostream>
#include "attribute/attr_transer.h"
#include "util/check_feat_exists.h"
#include "attrpb/setcate_value.pb.h"
#include "ctpl.h"

using namespace std;
using namespace algocomp;

namespace algocomp_test {

    class TestAttrTranser : public ::testing::Test {
    protected:
        shared_ptr<ctpl::thread_pool> _thp;
        shared_ptr<AttrTranser> _attr_transer;
        google::protobuf::Arena _arena;
        AUCImprAttrs inp_attrs;
        ::google::protobuf::Map<::std::string, ::algocomp::AttributeData> *uc_attrs =
                inp_attrs.mutable_user_and_ctx()->mutable_attrs();
        ::google::protobuf::Map<::std::string, ::algocomp::AttributeData> *ad_attrs =
                inp_attrs.mutable_ads()->Add()->mutable_attrs();

        virtual void SetUp() {
            _thp = make_shared<ctpl::thread_pool>(5, 100);
            _attr_transer = make_shared<AttrTranser>("1-attr-trans.xml",
                                                     "test/test-data",
                                                     _thp, ThreadType::ON_ADS);
        }

        virtual void TearDown() {
            _attr_transer.reset();
        }
    };

    TEST_F(TestAttrTranser, uc_categorical) {
        AttributeData sex_attr;
        sex_attr.set_str_val("male");
        (*uc_attrs)["userSex"] = sex_attr;

        auto outp_feats = _attr_transer->apply(inp_attrs, _arena);
        EXPECT_TRUE(outp_feats.has_value());

        auto disc_feats = outp_feats.value().user_and_ctx().disc_feats();
//        print_message(outp_feats.value());
        auto it_grp_found = disc_feats.find("user_sex");
        EXPECT_TRUE(it_grp_found != disc_feats.end());
        auto it_feat_found = it_grp_found->second.feats().find("user_sex%male");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "user_sex");
        EXPECT_TRUE(it_feat_found->second.fname() == "user_sex%male");
    }

    TEST_F(TestAttrTranser, uc_setcate_str1) {
        AttributeData poi_attr;
        poi_attr.set_str_val("sport|1:1;ent:1;other:1");
        (*uc_attrs)["poi_cpt"] = poi_attr;
//        print_message(inp_attrs);
        auto outp_feats = _attr_transer->apply(inp_attrs, _arena);
        EXPECT_TRUE(outp_feats.has_value());
//        print_message(outp_feats.value());

        auto cont_feats = outp_feats.value().user_and_ctx().cont_feats();
        auto it_grp_found = cont_feats.find("poi_cpt");
        auto it_feat_found = it_grp_found->second.feats().find("poi_cpt%sport-1");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "poi_cpt");
        EXPECT_TRUE(it_feat_found->second.fname() == "poi_cpt%sport-1");
        EXPECT_NEAR(it_feat_found->second.fval(), 0.333, 1E-3);
    }

    TEST_F(TestAttrTranser, uc_setcate_str2) {
        AttributeData poi_attr;
        poi_attr.set_str_val("spor|t,ent,other");
        (*uc_attrs)["poi_cpt2"] = poi_attr;
//        print_message(inp_attrs);
        auto outp_feats = _attr_transer->apply(inp_attrs, _arena);
        EXPECT_TRUE(outp_feats.has_value());
//        print_message(outp_feats.value());

        auto cont_feats = outp_feats.value().user_and_ctx().cont_feats();
        auto it_grp_found = cont_feats.find("poi_cpt2");
        auto it_feat_found = it_grp_found->second.feats().find("poi_cpt2%spor-t");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "poi_cpt2");
        EXPECT_TRUE(it_feat_found->second.fname() == "poi_cpt2%spor-t");
        EXPECT_NEAR(it_feat_found->second.fval(), 1.0, 1E-3);
    }

    TEST_F(TestAttrTranser, uc_setcate_str3) {
        AttributeData poi_attr;
        poi_attr.set_str_val("spor|t,ent,other");
        (*uc_attrs)["poi_cpt3"] = poi_attr;
//        print_message(inp_attrs);
        auto outp_feats = _attr_transer->apply(inp_attrs, _arena);
        EXPECT_TRUE(outp_feats.has_value());
//        print_message(outp_feats.value());

        auto disc_feats = outp_feats.value().user_and_ctx().disc_feats();
        auto it_grp_found = disc_feats.find("poi_cpt3");
        auto it_feat_found = it_grp_found->second.feats().find("poi_cpt3%spor-t");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "poi_cpt3");
        EXPECT_TRUE(it_feat_found->second.fname() == "poi_cpt3%spor-t");
    }

    TEST_F(TestAttrTranser, uc_setcate_map) {
        AttributeData poi_attr;
        auto setcate = poi_attr.mutable_scate_val();
        auto val = (*setcate).mutable_vals();
        (*val)["sport|1"] = 1;
        (*val)["ent"] = 1;
        (*val)["other"] = 1;
        (*uc_attrs)["poi_cpt"] = poi_attr;
//        print_message(inp_attrs);
        auto outp_feats = _attr_transer->apply(inp_attrs, _arena);
        EXPECT_TRUE(outp_feats.has_value());
//        print_message(outp_feats.value());

        auto cont_feats = outp_feats.value().user_and_ctx().cont_feats();
        auto it_grp_found = cont_feats.find("poi_cpt");
        auto it_feat_found = it_grp_found->second.feats().find("poi_cpt%sport-1");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "poi_cpt");
        EXPECT_TRUE(it_feat_found->second.fname() == "poi_cpt%sport-1");
        EXPECT_NEAR(it_feat_found->second.fval(), 0.333, 1E-3);
    }

    TEST_F(TestAttrTranser, uc_realvalued) {
        AttributeData age_attr;
        age_attr.set_float_val(26);
        (*uc_attrs)["userAge"] = age_attr;

        auto outp_feats = _attr_transer->apply(inp_attrs, _arena);
        EXPECT_TRUE(outp_feats.has_value());

        auto cont_feats = outp_feats.value().user_and_ctx().cont_feats();
        auto it_grp_found = cont_feats.find("user_age");
        EXPECT_TRUE(it_grp_found != cont_feats.end());
        auto it_feat_found = it_grp_found->second.feats().find("user_age");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "user_age");
        EXPECT_TRUE(it_feat_found->second.fname() == "user_age");
        EXPECT_NEAR(it_feat_found->second.fval(), 0.323, 1E-3);
    }

    TEST_F(TestAttrTranser, uc_realvalued2) {
        AttributeData cost_attr;
        cost_attr.set_float_val(180);
        (*uc_attrs)["userCost"] = cost_attr;

        auto outp_feats = _attr_transer->apply(inp_attrs, _arena);
        EXPECT_TRUE(outp_feats.has_value());

        auto cont_feats = outp_feats.value().user_and_ctx().cont_feats();
        auto it_grp_found = cont_feats.find("user_cost");
        EXPECT_TRUE(it_grp_found != cont_feats.end());
        auto it_feat_found = it_grp_found->second.feats().find("user_cost");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "user_cost");
        EXPECT_TRUE(it_feat_found->second.fname() == "user_cost");
        EXPECT_NEAR(it_feat_found->second.fval(), 0.726, 1E-3);
    }

    TEST_F(TestAttrTranser, uc_time) {
        AttributeData time_attr;
        time_attr.set_int_val(1547620467);
        (*uc_attrs)["happen_time"] = time_attr;

        auto outp_feats = _attr_transer->apply(inp_attrs, _arena);
        EXPECT_TRUE(outp_feats.has_value());

//        print_message(outp_feats.value());

        check_uc_disc_feat(outp_feats.value(), "happen_time+dayofweek", "happen_time+dayofweek%3");
        check_uc_disc_feat(outp_feats.value(), "happen_time+workday", "happen_time+workday");
        check_uc_disc_feat(outp_feats.value(), "happen_time+hour", "happen_time+hour%14");
        check_uc_disc_feat(outp_feats.value(), "happen_time+hourdesc", "happen_time+hourdesc%5");
        check_uc_disc_feat(outp_feats.value(), "happen_time+day", "happen_time+day%16");
        check_uc_disc_feat(outp_feats.value(), "happen_time+daydesc", "happen_time+daydesc%2");
        check_uc_disc_feat(outp_feats.value(), "happen_time+month", "happen_time+month%1");
        check_uc_disc_feat(outp_feats.value(), "happen_time+year", "happen_time+year%2019");
    }

    TEST_F(TestAttrTranser, ad_categorical) {
        AttributeData prod_id_attr;
        prod_id_attr.set_str_val("abc");
        (*ad_attrs)["productId"] = prod_id_attr;

        auto outp_feats = _attr_transer->apply(inp_attrs, _arena);
        EXPECT_TRUE(outp_feats.has_value());

        auto disc_feats = outp_feats.value().ads(0).disc_feats();
        auto it_grp_found = disc_feats.find("product_id");
        EXPECT_TRUE(it_grp_found != disc_feats.end());
        auto it_feat_found = it_grp_found->second.feats().find("product_id%abc");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "product_id");
        EXPECT_TRUE(it_feat_found->second.fname() == "product_id%abc");
    }

    TEST_F(TestAttrTranser, multi_thread4ads) {
        AttributeData prod_id_attr;
        prod_id_attr.set_str_val("产品0");
        (*ad_attrs)["productId"] = prod_id_attr;

        google::protobuf::Map<::std::string, AttributeData> *ad1_attrs =
                inp_attrs.mutable_ads()->Add()->mutable_attrs();
        AttributeData prod_id_attr1;
        prod_id_attr1.set_str_val("产品1");
        (*ad1_attrs)["productId"] = prod_id_attr1;

        google::protobuf::Map<::std::string, AttributeData> *ad2_attrs =
                inp_attrs.mutable_ads()->Add()->mutable_attrs();
        AttributeData prod_id_attr2;
        prod_id_attr2.set_str_val("产品2");
        (*ad2_attrs)["productId"] = prod_id_attr2;

        google::protobuf::Map<::std::string, AttributeData> *ad3_attrs =
                inp_attrs.mutable_ads()->Add()->mutable_attrs();
        AttributeData prod_id_attr3;
        prod_id_attr3.set_str_val("产品3");
        (*ad3_attrs)["productId"] = prod_id_attr3;

        google::protobuf::Map<::std::string, AttributeData> *ad4_attrs =
                inp_attrs.mutable_ads()->Add()->mutable_attrs();
        AttributeData prod_id_attr4;
        prod_id_attr4.set_str_val("产品4");
        (*ad4_attrs)["productId"] = prod_id_attr4;

        google::protobuf::Map<::std::string, AttributeData> *ad5_attrs =
                inp_attrs.mutable_ads()->Add()->mutable_attrs();
        AttributeData prod_id_attr5;
        prod_id_attr5.set_str_val("产品5");
        (*ad5_attrs)["productId"] = prod_id_attr5;
        auto outp_feats = _attr_transer->apply(inp_attrs, _arena);
        EXPECT_TRUE(outp_feats.has_value());
//        print_message(inp_attrs);
//        print_message(outp_feats.value());

        for (int i = 0; i < outp_feats.value().ads_size(); ++i) {
            auto disc_feats = outp_feats.value().ads(i).disc_feats();
            auto it_grp_found = disc_feats.find("product_id");
            EXPECT_TRUE(it_grp_found != disc_feats.end());
            std::stringstream ss;
            ss << "product_id%产品" << i;
            auto it_feat_found = it_grp_found->second.feats().find(ss.str());
            if (it_feat_found != it_grp_found->second.feats().end()) {
                EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
                EXPECT_TRUE(it_feat_found->second.gname() == "product_id");
                EXPECT_TRUE(it_feat_found->second.fname() == ss.str());
            } else{
                EXPECT_TRUE(1==2);
            }
        }
    }

    TEST_F(TestAttrTranser, uc_handle_neg) {
        AttributeData cost_attr;
        cost_attr.set_float_val(-400);
        (*uc_attrs)["userCost2"] = cost_attr;

        auto outp_feats = _attr_transer->apply(inp_attrs, _arena);
        EXPECT_TRUE(outp_feats.has_value());

        auto cont_feats = outp_feats.value().user_and_ctx().cont_feats();
        auto it_grp_found = cont_feats.find("user_cost2");
        EXPECT_TRUE(it_grp_found != cont_feats.end());
        auto it_feat_found = it_grp_found->second.feats().find("user_cost2");
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == "user_cost2");
        EXPECT_TRUE(it_feat_found->second.fname() == "user_cost2");
        EXPECT_NEAR(it_feat_found->second.fval(), 0.693, 1E-3);
    }

    TEST_F(TestAttrTranser, ad_time) {
        AttributeData time_attr2;
        time_attr2.set_str_val("2018-11-23");
        (*ad_attrs)["happen_time2"] = time_attr2;

        AttributeData time_attr3;
        time_attr3.set_str_val("2018-11-23 15:36:12");
        (*ad_attrs)["happen_time3"] = time_attr3;

        auto outp_feats = _attr_transer->apply(inp_attrs, _arena);
        EXPECT_TRUE(outp_feats.has_value());

//        print_message(outp_feats.value());

        check_ad_disc_feat(0, outp_feats.value(), "happen_time2+year", "happen_time2+year%2018");
        check_ad_disc_feat(0, outp_feats.value(), "happen_time2+month", "happen_time2+month%11");
        check_ad_disc_feat(0, outp_feats.value(), "happen_time2+day", "happen_time2+day%23");
        check_ad_disc_feat(0, outp_feats.value(), "happen_time2+daydesc", "happen_time2+daydesc%3");
        check_ad_disc_feat(0, outp_feats.value(), "happen_time2+dayofweek", "happen_time2+dayofweek%5");

        check_ad_disc_feat(0, outp_feats.value(), "happen_time3+year", "happen_time3+year%2018");
        check_ad_disc_feat(0, outp_feats.value(), "happen_time3+month", "happen_time3+month%11");
        check_ad_disc_feat(0, outp_feats.value(), "happen_time3+day", "happen_time3+day%23");
        check_ad_disc_feat(0, outp_feats.value(), "happen_time3+daydesc", "happen_time3+daydesc%3");
        check_ad_disc_feat(0, outp_feats.value(), "happen_time3+dayofweek", "happen_time3+dayofweek%5");
        check_ad_disc_feat(0, outp_feats.value(), "happen_time3+workday", "happen_time3+workday");
        check_ad_disc_feat(0, outp_feats.value(), "happen_time3+hour", "happen_time3+hour%15");
        check_ad_disc_feat(0, outp_feats.value(), "happen_time3+hourdesc", "happen_time3+hourdesc%5");
    }
}