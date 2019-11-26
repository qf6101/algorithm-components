//
// Created by qfeng on 2019-01-14.
//

#ifndef ALGORITHM_COMPONENTS_CHECKFEATEXISTS_H
#define ALGORITHM_COMPONENTS_CHECKFEATEXISTS_H

#include "featpb/auc_impr_feats.pb.h"
#include <string>

using namespace std;
using namespace algocomp;

namespace algocomp_test {
    bool check_disc_feat(const FeatureBunch &inp_feats, const string &gname, const string &fname);

    bool check_cont_feat(const FeatureBunch &inp_feats, const string &gname, const string &fname, float fval);

    bool check_uc_disc_feat(const AUCImprFeats &inp_feats, const string &gname, const string &fname);

    bool check_uc_cont_feat(const AUCImprFeats &inp_feats, const string &gname, const string &fname, float fval);

    bool check_ad_disc_feat(int idx, const AUCImprFeats &inp_feats, const string &gname, const string &fname);

    bool
    check_ad_cont_feat(int idx, const AUCImprFeats &inp_feats, const string &gname, const string &fname, float fval);

    void print_message(const ::google::protobuf::Message &msg);
}

#endif //ALGORITHM_COMPONENTS_CHECKFEATEXISTS_H
