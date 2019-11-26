//
// Created by qfeng on 2019-01-10.
//


#include "gtest/gtest.h"
#include <iostream>
#include <google/protobuf/util/json_util.h>
#include "util/check_feat_exists.h"
#include <fstream>
#include <sstream>
#include "base64.h"
#include "attrpb/auc_impr_attrs.pb.h"
#include "pipeline/pl_comp_pool.h"
#include "encpb/auc_impr_encs.pb.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace algocomp;
using namespace boost::algorithm;

namespace algocomp_test {

    class JustTest : public ::testing::Test {
    protected:
        google::protobuf::Arena _arena;

        virtual void SetUp() {
        }

        virtual void TearDown() {
        }
    };

//    TEST_F(JustTest, test1) {
//        auto pool = PLCompPool("test/test-data/2-pipeline.xml");
//        auto plh = pool.get_plhead("pl-head-1");
//        std::ifstream in("test/test-data/encoded_attributes.txt");
//        std::ofstream out;
//        out.open("test/test-data/out_feats.json");
//
//        std::string line;
//        while (std::getline(in, line)) {
//            string decoded_attrs;
//            Base64::Decode(line, &decoded_attrs);
//            AUCImprAttrs attrs;
//            attrs.ParseFromString(decoded_attrs);
//            print_message(attrs);
//            auto feats_str = plh->work(decoded_attrs.c_str(), decoded_attrs.length());
//
//            string decoded_feats;
//            Base64::Decode(feats_str, &decoded_feats);
//
//            AUCImprFeats *outp_feats = google::protobuf::Arena::CreateMessage<AUCImprFeats>(&_arena);
//            outp_feats->ParseFromString(decoded_feats);
//
//            string msg_json;
//            google::protobuf::util::MessageToJsonString(*outp_feats, &msg_json);
//            out << msg_json << endl;
//        }
//
//        in.close();
//        out.close();
//    }

    TEST_F(JustTest, test2) {
        std::ifstream in("test/test-data/tmp3.log");

        std::string line;
        while (std::getline(in, line)) {
            string decoded_attrs;
            Base64::Decode(line, &decoded_attrs);
            AUCImprAttrs attrs;
            attrs.ParseFromString(decoded_attrs);
            print_message(attrs);
        }

        in.close();
    }

//    TEST_F(JustTest, test3) {
//        auto pool = PLCompPool("test/test-data/3-pipeline.xml");
//        auto plh = pool.get_plhead("pl-head-1");
//        std::ifstream in("test/test-data/25.log");
//        std::ofstream out;
//        out.open("test/test-data/encoded_attributes_offline_outp.json");
//
//        std::string line;
//        while (std::getline(in, line)) {
//            string decoded_attrs;
//            Base64::Decode(line, &decoded_attrs);
//            AUCImprAttrs attrs;
//            attrs.ParseFromString(decoded_attrs);
//            print_message(attrs);
//            auto feats_str = plh->work(decoded_attrs.c_str(), decoded_attrs.length());
//
//            string decoded_feats;
//            Base64::Decode(feats_str, &decoded_feats);
//
//            AUCImprFeats *outp_feats = google::protobuf::Arena::CreateMessage<AUCImprFeats>(&_arena);
//            outp_feats->ParseFromString(decoded_feats);
//
//            string msg_json;
//            google::protobuf::util::MessageToJsonString(*outp_feats, &msg_json);
//            out << msg_json << endl;
//            break;
//        }
//
//        in.close();
//        out.close();
//    }

//    TEST_F(JustTest, test4) {
//        auto pool = PLCompPool("test/test-data/3-pipeline.xml");
//        auto pl = pool.get_pipeline("pl-1");
//        std::ifstream in("test/test-data/09.log");
//        std::ofstream out;
//        out.open("test/test-data/09_outp.json");
//
//        std::string line;
//        while (std::getline(in, line)) {
//            string decoded_attrs;
//            Base64::Decode(line, &decoded_attrs);
//            AUCImprAttrs attrs;
//            attrs.ParseFromString(decoded_attrs);
//            print_message(attrs);
//            auto encoded_vec = pl->head_to_middle(decoded_attrs.c_str(), decoded_attrs.length());
//
//            for (auto encoded: encoded_vec) {
//                out << encoded << endl;
//            }
//        }
//
//        in.close();
//        out.close();
//    }

//    TEST_F(JustTest, test5) {
//        ifstream model_file;
//        model_file.open("test/test-data/4-models/2-skl-model.txt");
//        set<long> coeffs;
//        if (model_file.is_open()) {
//            Eigen::SparseVector<float> kv(int(pow(2,30)));
//            string line;
//            while (!model_file.eof()) {
//                getline(model_file, line);
//                line = trim_copy(line);
//                if (line.length() == 0) continue;
//                vector<string> splits;
//                boost::algorithm::split(splits, line, boost::algorithm::is_any_of(" "));
//                if (splits.size() == 2) {
//                    uint64_t index = boost::lexical_cast<uint64_t>(splits[0]);
//                    float value = boost::lexical_cast<float>(splits[1]);
//                    if (value > 0) coeffs.insert(index);
////                    cout << "coef: " << index << endl;
//                }
//            }
//            model_file.close();
//        }
//
//
//        auto pool = PLCompPool("test/test-data/4-pipeline.xml");
//        auto plh = pool.get_plhead("pl-head-1");
//        auto plm = pool.get_plmiddle("pl-middle-1");
//        auto plt = pool.get_pltail("pl-tail-1");
//        std::ifstream in("test/test-data/4.log");
//        std::ofstream out;
//        out.open("test/test-data/4.res");
//
//        std::string line;
//        while (std::getline(in, line)) {
//            string decoded_attrs;
//            Base64::Decode(line, &decoded_attrs);
//            AUCImprAttrs attrs;
//            attrs.ParseFromString(decoded_attrs);
//
//            {
//                auto ad = attrs.add_ads();
//                AttributeData attr_data;
//                attr_data.set_long_val(83);
//                (*ad->mutable_attrs())["productId"] = attr_data;
//            }
//
//            {
//                auto ad = attrs.add_ads();
//                AttributeData attr_data;
//                attr_data.set_long_val(99);
//                (*ad->mutable_attrs())["productId"] = attr_data;
//            }
//
//
//            {
//                auto ad = attrs.add_ads();
//                AttributeData attr_data;
//                attr_data.set_long_val(96);
//                (*ad->mutable_attrs())["productId"] = attr_data;
//            }
//
////            print_message(attrs);
//
//
//            string attrs_pb;
//            attrs.SerializeToString(&attrs_pb);
//
//            auto feats_str = plh->work(attrs_pb.c_str(), attrs_pb.length());
//
//            string decoded_feats;
//            Base64::Decode(feats_str, &decoded_feats);
//
//            AUCImprFeats *outp_feats = google::protobuf::Arena::CreateMessage<AUCImprFeats>(&_arena);
//            outp_feats->ParseFromString(decoded_feats);
//
////            print_message(*outp_feats);
//
//            auto encs_str = plm->work_tokv(decoded_feats.c_str(), decoded_feats.size());
//
//            string decoded_encs;
//            Base64::Decode(encs_str, &decoded_encs);
//
//            AUCImprEncs *outp_encs = google::protobuf::Arena::CreateMessage<AUCImprEncs>(&_arena);
//            outp_encs->ParseFromString(decoded_encs);
//
////            print_message(*outp_encs);
//
//            set<long> ad1_encs;
//            set<long> ad2_encs;
//            set<long> ad3_encs;
//
//            for(int i = 0; i < outp_encs->encs_size(); ++i) {
//                auto enc_bunch = outp_encs->encs(i);
//                for (int j = 0; j < enc_bunch.encs_size(); ++j) {
//                    auto enc = enc_bunch.encs(j).enc();
//                    if (i == 0) {
//                        if (coeffs.find(enc) != coeffs.end()) ad1_encs.insert(enc);
//                    } else if(i == 1) {
//                        if (coeffs.find(enc) != coeffs.end()) ad2_encs.insert(enc);
//                    } else if(i == 2) {
//                        if (coeffs.find(enc) != coeffs.end()) ad3_encs.insert(enc);
//                    }
//                }
//            }
//
//            cout << "1: " << ad1_encs.size() << ", 2: " << ad2_encs.size() << ", 3: " << ad3_encs.size() << endl;
//
//            set<long> ad1_ad2_diff;
//            for (auto a : ad2_encs) {
//                if (ad1_encs.find(a) == ad1_encs.end()) {
//                    ad1_ad2_diff.insert(a);
//                }
//            }
//
//            cout << "diff of ad1 and ad2: " << ad1_ad2_diff.size() << endl;
//
//            auto scores = plt->work(decoded_encs.c_str(), decoded_encs.size());
//
//            for (int i = 0; i < scores.size(); ++i)
//                cout << "score " << i << " " << scores[i] << std::endl;
//
//            break;
//        }
//
//        in.close();
//        out.close();
//    }

}
