//
// Created by qfeng on 2019-01-14.
//

#include "util/check_feat_exists.h"
#include "gtest/gtest.h"
#include <google/protobuf/util/json_util.h>

using namespace std;
using namespace algocomp;

namespace algocomp_test {
    bool check_disc_feat(const FeatureBunch &inp_feats, const string &gname, const string &fname) {
        auto disc_feats= inp_feats.disc_feats();
        auto it_grp_found = disc_feats.find(gname);
        EXPECT_TRUE(it_grp_found != disc_feats.end());
        auto it_feat_found = it_grp_found->second.feats().find(fname);
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == gname);
        EXPECT_TRUE(it_feat_found->second.fname() == fname);
    }

    bool check_cont_feat(const FeatureBunch &inp_feats, const string &gname, const string &fname, float fval) {
        auto cont_feats = inp_feats.cont_feats();
        auto it_grp_found = cont_feats.find(gname);
        EXPECT_TRUE(it_grp_found != cont_feats.end());
        auto it_feat_found = it_grp_found->second.feats().find(fname);
        EXPECT_TRUE(it_feat_found != it_grp_found->second.feats().end());
        EXPECT_TRUE(it_feat_found->second.gname() == gname);
        EXPECT_TRUE(it_feat_found->second.fname() == fname);
        EXPECT_NEAR(it_feat_found->second.fval(), fval, 1E-3);
    }

    bool check_uc_disc_feat(const AUCImprFeats &inp_feats, const string &gname, const string &fname) {
        return check_disc_feat(inp_feats.user_and_ctx(), gname, fname);
    }

    bool check_uc_cont_feat(const AUCImprFeats &inp_feats, const string &gname, const string &fname, float fval) {
        return check_cont_feat(inp_feats.user_and_ctx(), gname, fname, fval);
    }

    bool check_ad_disc_feat(int idx, const AUCImprFeats &inp_feats, const string &gname, const string &fname) {
        return check_disc_feat(inp_feats.ads(idx), gname, fname);
    }

    bool check_ad_cont_feat(int idx, const AUCImprFeats &inp_feats, const string &gname, const string &fname, float fval) {
        return check_cont_feat(inp_feats.ads(idx), gname, fname, fval);
    }

    void print_message(const ::google::protobuf::Message &msg) {
        string msg_json;
        google::protobuf::util::MessageToJsonString(msg, &msg_json);
        cout << msg_json << endl;
    }
}

