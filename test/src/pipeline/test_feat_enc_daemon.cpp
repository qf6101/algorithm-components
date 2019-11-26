//
// Created by AIYOJ on 2019-02-16.
//

#include "gtest/gtest.h"
#include <iostream>
#include "ctpl.h"
#include "encode/feat_enc_daemon.h"
#include "featpb/auc_impr_feats.pb.h"
#include <google/protobuf/arena.h>
#include "featpb/auc_impr_feats.pb.h"
#include "util/check_feat_exists.h"
#include "encode/feature_hash.h"


namespace algocomp_test {
    class TestFeatEncoder : public ::testing::Test {
    protected:
        std::shared_ptr<ctpl::thread_pool> _thp;
        std::shared_ptr<algocomp::FeatEncDaemon> _feat_enc_daemon;
        std::shared_ptr<algocomp::FeatureEncoder> _feat_enc;
        algocomp::AUCImprFeats inp_feats;

        virtual void SetUp() {
            _thp = std::make_shared<ctpl::thread_pool>(5, 100);
            _feat_enc = std::make_shared<algocomp::FeatureHash>(1 << 20);

            _feat_enc_daemon = make_shared<FeatEncDaemon>(_feat_enc, FeatEncScope::ALL, FeatEncMode::ACCUMULATING,
                                                          FeatEncStrFormat::LIBSVM, _thp, ThreadType::ON_ADS);
        }

        virtual void TearDown() {
            _feat_enc_daemon.reset();
        }
    };

    TEST_F(TestFeatEncoder, ad_multi_thread) {

        algocomp::AUCImprFeats inp_feats;
        google::protobuf::Map<std::string, algocomp::ContFeatGrp>
                *uc_conts = inp_feats.mutable_user_and_ctx()->mutable_cont_feats();

        google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp>
                *uc_disces = inp_feats.mutable_user_and_ctx()->mutable_disc_feats();
        algocomp::FeatureBunch *ad = inp_feats.mutable_ads()->Add();
        google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp>
                *ad_conts = ad->mutable_cont_feats();
        google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp>
                *ad_disces = ad->mutable_disc_feats();

        // uc
        ContFeatData ucost_feat;
        ucost_feat.set_gname("user_cost");
        ucost_feat.set_fname("user_cost");
        ucost_feat.set_fval(20.00);
        ContFeatGrp ucost_feat_grp;
        (*ucost_feat_grp.mutable_feats())["user_cost"] = ucost_feat;
        (*uc_conts)["user_cost"] = ucost_feat_grp;

        // ad0
        {
            algocomp::ContFeatData acost_feat;
            acost_feat.set_gname("ad_cost");
            acost_feat.set_fname("ad_cost");
            acost_feat.set_fval(0);
            algocomp::ContFeatGrp acost_feat_grp;
            (*acost_feat_grp.mutable_feats())["ad_cost"] = acost_feat;
            (*ad_conts)["ad_cost"] = acost_feat_grp;
        }

        // ad1
        {
            FeatureBunch *ad1 = inp_feats.mutable_ads()->Add();
            google::protobuf::Map<::std::string, algocomp::ContFeatGrp>
                    *ad_conts1 = ad1->mutable_cont_feats();
            ContFeatData acost_feat1;
            acost_feat1.set_gname("ad_cost");
            acost_feat1.set_fname("ad_cost");
            acost_feat1.set_fval(1);
            ContFeatGrp acost_feat_grp1;
            (*acost_feat_grp1.mutable_feats())["ad_cost"] = acost_feat1;
            (*ad_conts1)["ad_cost"] = acost_feat_grp1;


            google::protobuf::Map<::std::string, algocomp::DiscFeatGrp>
                    *ad_disces1 = ad1->mutable_disc_feats();
            DiscFeatData adisc_feat1;
            adisc_feat1.set_gname("ss");
            adisc_feat1.set_fname("ss%1");
            DiscFeatGrp adisc_feat_grp1;
            (*adisc_feat_grp1.mutable_feats())["ss%1"] = adisc_feat1;
            (*ad_disces1)["ss"] = adisc_feat_grp1;
        }

        // ad2
        {
            FeatureBunch *ad2 = inp_feats.mutable_ads()->Add();
            google::protobuf::Map<::std::string, algocomp::ContFeatGrp>
                    *ad_conts2 = ad2->mutable_cont_feats();

            ContFeatData acost_feat2;
            acost_feat2.set_gname("ad_cost");
            acost_feat2.set_fname("ad_cost");
            acost_feat2.set_fval(2);
            ContFeatGrp acost_feat_grp2;
            (*acost_feat_grp2.mutable_feats())["ad_cost"] = acost_feat2;
            (*ad_conts2)["ad_cost"] = acost_feat_grp2;

            google::protobuf::Map<::std::string, algocomp::DiscFeatGrp>
                    *ad_disces2 = ad2->mutable_disc_feats();
        }

        // ad3
        {
            FeatureBunch *ad3 = inp_feats.mutable_ads()->Add();
            google::protobuf::Map<::std::string, algocomp::ContFeatGrp>
                    *ad_conts3 = ad3->mutable_cont_feats();

            ContFeatData acost_feat3;
            acost_feat3.set_gname("ad_cost");
            acost_feat3.set_fname("ad_cost");
            acost_feat3.set_fval(3);
            ContFeatGrp acost_feat_grp3;
            (*acost_feat_grp3.mutable_feats())["ad_cost"] = acost_feat3;
            (*ad_conts3)["ad_cost"] = acost_feat_grp3;

            google::protobuf::Map<::std::string, algocomp::DiscFeatGrp>
                    *ad_disces3 = ad3->mutable_disc_feats();
        }

        // ad4
        {
            FeatureBunch *ad4 = inp_feats.mutable_ads()->Add();
            google::protobuf::Map<::std::string, algocomp::ContFeatGrp>
                    *ad_conts4 = ad4->mutable_cont_feats();

            ContFeatData acost_feat4;
            acost_feat4.set_gname("ad_cost");
            acost_feat4.set_fname("ad_cost");
            acost_feat4.set_fval(4);
            ContFeatGrp acost_feat_grp4;
            (*acost_feat_grp4.mutable_feats())["ad_cost"] = acost_feat4;
            (*ad_conts4)["ad_cost"] = acost_feat_grp4;

            google::protobuf::Map<::std::string, algocomp::DiscFeatGrp>
                    *ad_disces4 = ad4->mutable_disc_feats();
        }

//        print_message(inp_feats);

        std::optional<std::vector<Eigen::SparseVector<float>>> kvs = _feat_enc_daemon->to_kv(inp_feats);
        int i = 0;
        for (auto &v: *kvs) {
//            ad_cost: 804698
//            std::cout << v.coeff(804698) << std::endl;
            EXPECT_TRUE(v.coeff(804698) == i);
            i++;
        }

    }
}