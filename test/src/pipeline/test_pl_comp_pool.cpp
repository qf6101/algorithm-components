//
// Created by qfeng on 2019-01-10.
//


#include "gtest/gtest.h"
#include <iostream>
#include <fstream>
#include "pipeline/pl_comp_pool.h"
#include <google/protobuf/util/json_util.h>
#include "util/check_feat_exists.h"
#include "base64.h"
#include "encpb/auc_impr_encs.pb.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "calibration/hybrid.h"

using namespace std;
using namespace boost::algorithm;
using namespace algocomp;

namespace algocomp_test {

    class TestPLCompPool : public ::testing::Test {
    protected:
        google::protobuf::Arena _arena;
        AUCImprAttrs inp_attrs;
        ::google::protobuf::Map<::std::string, ::algocomp::AttributeData> *uc_attrs =
                inp_attrs.mutable_user_and_ctx()->mutable_attrs();
        ::google::protobuf::Map<::std::string, ::algocomp::AttributeData> *ad_attrs =
                inp_attrs.mutable_ads()->Add()->mutable_attrs();

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

        FeatureBunch *ad2 = inp_feats.mutable_ads()->Add();
        ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp>
                *ad2_conts = ad2->mutable_cont_feats();
        ::google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp>
                *ad2_disces = ad2->mutable_disc_feats();

        FeatureBunch *ad3 = inp_feats.mutable_ads()->Add();
        ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp>
                *ad3_conts = ad3->mutable_cont_feats();
        ::google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp>
                *ad3_disces = ad3->mutable_disc_feats();

        FeatureBunch *ad4 = inp_feats.mutable_ads()->Add();
        ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp>
                *ad4_conts = ad4->mutable_cont_feats();
        ::google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp>
                *ad4_disces = ad4->mutable_disc_feats();

        bool label_added = false;


        virtual void SetUp() {
            if (!label_added) {

                inp_attrs.mutable_labels()->Add(1.0);
                inp_feats.mutable_labels()->Add(1.0);

                label_added = true;
            }
        }

        virtual void TearDown() {

        }
    };

    TEST_F(TestPLCompPool, pl_head) {
        AttributeData sex_attr;
        sex_attr.set_str_val("male");
        (*uc_attrs)["userSex"] = sex_attr;

        AttributeData age_attr;
        age_attr.set_float_val(26);
        (*uc_attrs)["userAge"] = age_attr;

        AttributeData cost_attr;
        cost_attr.set_float_val(180);
        (*uc_attrs)["userCost"] = cost_attr;

        AttributeData prod_id_attr;
        prod_id_attr.set_str_val("abc");
        (*ad_attrs)["productId"] = prod_id_attr;

        auto pool = PLCompPool("test/test-data/1-pipeline.xml");
        auto plh = pool.get_plhead("pl-head-1");

        string attrs_pb;
        inp_attrs.SerializeToString(&attrs_pb);

        auto outp_feats_ = plh->work(attrs_pb.c_str(), attrs_pb.length());

        string decoded;
        Base64::Decode(outp_feats_, &decoded);

        AUCImprFeats *outp_feats = google::protobuf::Arena::CreateMessage<AUCImprFeats>(&_arena);
        outp_feats->ParseFromString(decoded);

        string outp_feats_json;
        google::protobuf::util::MessageToJsonString(*outp_feats, &outp_feats_json);
        cout << outp_feats_json << endl;

        check_uc_disc_feat(*outp_feats, "user_age#bkt", "user_age#bkt@1");
        check_uc_disc_feat(*outp_feats, "user_sex", "user_sex%male");
        check_uc_disc_feat(*outp_feats, "!cross^user_age#bkt^user_sex", "!cross^user_age#bkt@1^user_sex%male");
        check_uc_cont_feat(*outp_feats, "user_age", "user_age", 0.323);
        check_uc_cont_feat(*outp_feats, "user_cost", "user_cost", 0.726);
        check_uc_cont_feat(*outp_feats, "user_cost#log", "user_cost#log", -0.139);
        check_ad_disc_feat(0, *outp_feats, "product_id", "product_id%abc");
    }

    TEST_F(TestPLCompPool, pl_middle) {
        ContFeatData ucost_feat;
        ucost_feat.set_gname("user_cost");
        ucost_feat.set_fname("user_cost");
        ucost_feat.set_fval(100);
        ContFeatGrp ucost_feat_grp;
        (*ucost_feat_grp.mutable_feats())["user_cost"] = ucost_feat;
        (*uc_conts)["user_cost"] = ucost_feat_grp;

        ContFeatData uage_feat;
        uage_feat.set_gname("user_age");
        uage_feat.set_fname("user_age");
        uage_feat.set_fval(26);
        ContFeatGrp uage_feat_grp;
        (*uage_feat_grp.mutable_feats())["user_age"] = uage_feat;
        (*uc_conts)["user_age"] = uage_feat_grp;

        DiscFeatData usex_feat;
        usex_feat.set_gname("user_sex");
        usex_feat.set_fname("user_sex%male");
        DiscFeatGrp usex_feat_grp;
        (*usex_feat_grp.mutable_feats())["user_sex"] = usex_feat;
        (*uc_disces)["user_sex"] = usex_feat_grp;

        DiscFeatData uage_bkt_feat;
        uage_bkt_feat.set_gname("user_age#bkt");
        uage_bkt_feat.set_fname("user_age#bkt@1");
        DiscFeatGrp uage_bkt_feat_grp;
        (*uage_bkt_feat_grp.mutable_feats())["user_age#bkt"] = uage_bkt_feat;
        (*uc_disces)["user_age#bkt"] = uage_bkt_feat_grp;

        DiscFeatData pid_feat;
        pid_feat.set_gname("product_id");
        pid_feat.set_fname("product_id%abc");
        DiscFeatGrp pid_feat_grp;
        (*pid_feat_grp.mutable_feats())["product_id%abc"] = pid_feat;
        (*ad_disces)["product_id"] = pid_feat_grp;

        string feats_pb;
        inp_feats.SerializeToString(&feats_pb);

        auto pool = PLCompPool("test/test-data/1-pipeline.xml");
        auto plm = pool.get_plmiddle("pl-middle-1");
        auto res = plm->work_toenc(feats_pb.c_str(), feats_pb.length());
        if (res.size() > 0) cout << res[0] << endl;
    }

    TEST_F(TestPLCompPool, pl_head_to_middle) {
        AttributeData sex_attr;
        sex_attr.set_str_val("male");
        (*uc_attrs)["userSex"] = sex_attr;

        AttributeData age_attr;
        age_attr.set_float_val(26);
        (*uc_attrs)["userAge"] = age_attr;

        AttributeData cost_attr;
        cost_attr.set_float_val(180);
        (*uc_attrs)["userCost"] = cost_attr;

        AttributeData prod_id_attr;
        prod_id_attr.set_str_val("abc");
        (*ad_attrs)["productId"] = prod_id_attr;

        auto pool = PLCompPool("test/test-data/1-pipeline.xml");
        auto pl = pool.get_pipeline("pl-1");

        string attrs_pb;
        inp_attrs.SerializeToString(&attrs_pb);

        auto encoded = pl->head_to_middle(attrs_pb.c_str(), attrs_pb.length());
        if (encoded.size() > 0) cout << encoded[0] << endl;
    }

    TEST_F(TestPLCompPool, pl_middle_to_tail) {
        ContFeatData ucost_feat;
        ucost_feat.set_gname("user_cost");
        ucost_feat.set_fname("user_cost");
        ucost_feat.set_fval(100);
        ContFeatGrp ucost_feat_grp;
        (*ucost_feat_grp.mutable_feats())["user_cost"] = ucost_feat;
        (*uc_conts)["user_cost"] = ucost_feat_grp;

        ContFeatData uage_feat;
        uage_feat.set_gname("user_age");
        uage_feat.set_fname("user_age");
        uage_feat.set_fval(26);
        ContFeatGrp uage_feat_grp;
        (*uage_feat_grp.mutable_feats())["user_age"] = uage_feat;
        (*uc_conts)["user_age"] = uage_feat_grp;

        DiscFeatData usex_feat;
        usex_feat.set_gname("user_sex");
        usex_feat.set_fname("user_sex%male");
        DiscFeatGrp usex_feat_grp;
        (*usex_feat_grp.mutable_feats())["user_sex"] = usex_feat;
        (*uc_disces)["user_sex"] = usex_feat_grp;

        DiscFeatData uage_bkt_feat;
        uage_bkt_feat.set_gname("user_age#bkt");
        uage_bkt_feat.set_fname("user_age#bkt@1");
        DiscFeatGrp uage_bkt_feat_grp;
        (*uage_bkt_feat_grp.mutable_feats())["user_age#bkt"] = uage_bkt_feat;
        (*uc_disces)["user_age#bkt"] = uage_bkt_feat_grp;

        DiscFeatData pid_feat;
        pid_feat.set_gname("product_id");
        pid_feat.set_fname("product_id%abc");
        DiscFeatGrp pid_feat_grp;
        (*pid_feat_grp.mutable_feats())["product_id%abc"] = pid_feat;
        (*ad_disces)["product_id"] = pid_feat_grp;

        string feats_pb;
        inp_feats.SerializeToString(&feats_pb);

        auto pool = PLCompPool("test/test-data/1-pipeline.xml");
        auto pl = pool.get_pipeline("pl-2");
        auto res = pl->middle_to_tail(feats_pb.c_str(), feats_pb.length());
        if (res.size() > 0) cout << res[0] << endl;
    }

    TEST_F(TestPLCompPool, plmiddle_multi_threads) {
        ContFeatData ucost_feat;
        ucost_feat.set_gname("user_cost");
        ucost_feat.set_fname("user_cost");
        ucost_feat.set_fval(100);
        ContFeatGrp ucost_feat_grp;
        (*ucost_feat_grp.mutable_feats())["user_cost"] = ucost_feat;
        (*uc_conts)["user_cost"] = ucost_feat_grp;

        ContFeatData uage_feat;
        uage_feat.set_gname("user_age");
        uage_feat.set_fname("user_age");
        uage_feat.set_fval(26);
        ContFeatGrp uage_feat_grp;
        (*uage_feat_grp.mutable_feats())["user_age"] = uage_feat;
        (*uc_conts)["user_age"] = uage_feat_grp;

        DiscFeatData usex_feat;
        usex_feat.set_gname("user_sex");
        usex_feat.set_fname("user_sex%male");
        DiscFeatGrp usex_feat_grp;
        (*usex_feat_grp.mutable_feats())["user_sex"] = usex_feat;
        (*uc_disces)["user_sex"] = usex_feat_grp;

        DiscFeatData uage_bkt_feat;
        uage_bkt_feat.set_gname("user_age#bkt");
        uage_bkt_feat.set_fname("user_age#bkt@1");
        DiscFeatGrp uage_bkt_feat_grp;
        (*uage_bkt_feat_grp.mutable_feats())["user_age#bkt"] = uage_bkt_feat;
        (*uc_disces)["user_age#bkt"] = uage_bkt_feat_grp;

        DiscFeatData pid_feat;
        pid_feat.set_gname("product_id");
        pid_feat.set_fname("product_id%1");
        DiscFeatGrp pid_feat_grp;
        (*pid_feat_grp.mutable_feats())["product_id%1"] = pid_feat;
        (*ad_disces)["product_id"] = pid_feat_grp;

        DiscFeatData pid_feat2;
        pid_feat2.set_gname("product_id");
        pid_feat2.set_fname("product_id%2");
        DiscFeatGrp pid_feat_grp2;
        (*pid_feat_grp2.mutable_feats())["product_id%2"] = pid_feat2;
        (*ad2_disces)["product_id"] = pid_feat_grp2;

        DiscFeatData pid_feat3;
        pid_feat3.set_gname("product_id");
        pid_feat3.set_fname("product_id%3");
        DiscFeatGrp pid_feat_grp3;
        (*pid_feat_grp3.mutable_feats())["product_id%3"] = pid_feat3;
        (*ad3_disces)["product_id"] = pid_feat_grp3;

        DiscFeatData pid_feat4;
        pid_feat4.set_gname("product_id");
        pid_feat4.set_fname("product_id%4");
        DiscFeatGrp pid_feat_grp4;
        (*pid_feat_grp4.mutable_feats())["product_id%4"] = pid_feat4;
        (*ad4_disces)["product_id"] = pid_feat_grp4;

        string feats_pb;
        inp_feats.SerializeToString(&feats_pb);

        auto pool = PLCompPool("test/test-data/1-pipeline.xml");
        auto plm = pool.get_plmiddle("pl-middle-1");
        auto outp_encs_ = plm->work_tokv(feats_pb.c_str(), feats_pb.length());
        EXPECT_TRUE(outp_encs_.size() > 0);

        string decoded;
        Base64::Decode(outp_encs_, &decoded);

        AUCImprEncs *outp_encs = google::protobuf::Arena::CreateMessage<AUCImprEncs>(&_arena);
        outp_encs->ParseFromString(decoded);

        vector<long> order_constraints = {73040628, 693821831, 1045909557, 655385424};
        for (int i = 0; i < 4; ++i) {
            EXPECT_TRUE(outp_encs->encs(i).encs_size() == 5);

            bool meet_constraints = false;
            for (int j = 0; j < 5; ++j) {
                if (outp_encs->encs(i).encs(j).enc() == order_constraints[i]) meet_constraints = true;
            }

            EXPECT_TRUE(meet_constraints);
        }

//        print_message(*outp_encs);
    }

    TEST_F(TestPLCompPool, pltail_multi_threads) {
        ifstream model_file;
        model_file.open("test/test-data/1-models/1-skl-sample.txt");
        vector<long> coeffs;
        if (model_file.is_open()) {
            Eigen::SparseVector<float> kv(int(pow(2, 30)));
            string line;
            while (!model_file.eof()) {
                getline(model_file, line);
                line = trim_copy(line);
                if (line.length() == 0) continue;
                vector<string> splits;
                boost::algorithm::split(splits, line, boost::algorithm::is_any_of(" "));
                if (splits.size() == 2) {
                    uint64_t index = boost::lexical_cast<uint64_t>(splits[0]);
                    float value = boost::lexical_cast<float>(splits[1]);
                    if (value > 0) coeffs.push_back(index);
                }
            }
            model_file.close();
        }

        AUCImprEncs *encs = google::protobuf::Arena::CreateMessage<AUCImprEncs>(&_arena);
        encs->set_feat_size(1073741824);

        for (int i = 0; i < 5; ++i) {
            auto enc_bunch = encs->add_encs();
            for (int j = 0; j < 4; ++j) {
                auto enc_data = enc_bunch->add_encs();
                enc_data->set_enc(coeffs[j]);
                enc_data->set_val(1.0);
            }
            for (int j = 4; j < 8; ++j) {
                auto enc_data = enc_bunch->add_encs();
                enc_data->set_enc(coeffs[j]);
                enc_data->set_val(i + 1.0);
            }
        }

        string encs_pb;
        encs->SerializeToString(&encs_pb);

        auto pool = PLCompPool("test/test-data/1-pipeline.xml");
        auto plt = pool.get_pltail("pl-tail-1");
        auto outp_scores = plt->work(encs_pb.c_str(), encs_pb.length());

        EXPECT_TRUE(outp_scores.size() == 5);
        for (int i = 1; i < outp_scores.size(); ++i) {
            EXPECT_TRUE(outp_scores[i] > outp_scores[i - 1]);
        }
    }

    TEST_F(TestPLCompPool, pl_calibration) {
        auto pool = PLCompPool("test/test-data/1-pipeline.xml");

        std::vector<float> correct_scores = {0.169003, 0.0768808, 0.0123666};

        std::vector<std::string> pids = {"ad1", "ad2", "ad5"};
        std::vector<float> scores = {0.9, 0.8, 0.4};

        auto new_scores = pool.calibrate(pids, scores);

        auto new_score1 = pool.calibrate("ad1", 0.9);
        auto new_score2 = pool.calibrate("ad2", 0.8);
        auto new_score3 = pool.calibrate("ad5", 0.4);

        EXPECT_EQ(new_scores[0], new_score1);
        EXPECT_EQ(new_scores[1], new_score2);
        EXPECT_EQ(new_scores[2], new_score3);

        int i = 0;
        std::for_each(new_scores.begin(), new_scores.end(), [&i, &correct_scores](float v) {
            std::cout << v << std::endl;
            EXPECT_NEAR(v, correct_scores[i], 1E-4);
            i++;
        });
    }
}