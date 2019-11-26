//
// Created by AIYOJ on 2019-02-26.
//


#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "base64.h"
#include "pipeline/pl_comp_pool.h"
#include "Python.h"
#include "util/check_feat_exists.h"
#include "absl/strings/str_split.h"


namespace algocomp_test {
    class TestOnline : public ::testing::Test {
    protected:
        virtual void SetUp() {

        }

        virtual void TearDown() {

        }
    };

    TEST_F(TestOnline, valid) {
        auto pool = algocomp::PLCompPool("test/test-data/online/pipeline.xml");
        auto plh = pool.get_plhead("pl-head-1");
        auto plm = pool.get_plmiddle("pl-middle-1");
        auto plt = pool.get_pltail("pl-tail-1");

        auto plh2m = pool.get_pipeline("pl-1");

        std::ifstream in("test/test-data/online/0-2019-01-01.log");
        std::string attrs_enc_pb_str;
        std::string attrs_pb_str;
        std::string feats_pb_str;
        std::string encs_pb_str;

        // python initialize
        Py_Initialize();
        if (!Py_IsInitialized()) {
            std::cout << "[ERROR]." << std::endl;
        }

        // add python env
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('./test/src/online')");

        // import python module
        PyObject *pModule = PyImport_ImportModule("test_offline");
        if (pModule == NULL) {
            std::cout << "[ERROR] Python get module failed." << endl;
        }

        // python "predict" function
        PyObject *pFunc = PyObject_GetAttrString(pModule, "predict");

        // "predict" parameters: (data, model, hash2seq)
        PyObject *pArgs = PyTuple_New(3);

        int j = 0;
        while (std::getline(in, attrs_enc_pb_str)) {
            std::vector<std::string> offline_results;
            std::vector<float> offline_scores;
            std::vector<float> online_scores;

            Base64::Decode(attrs_enc_pb_str, &attrs_pb_str);

            algocomp::AUCImprAttrs attrs_pb;
            attrs_pb.ParseFromString(attrs_pb_str);

            auto true_ad = attrs_pb.ads(0);
            auto true_ad_id = (*true_ad.mutable_attrs())["agentProductId"].long_val();

            std::vector<int> pids = {116, 69, 6, 83, 147, 4, 139, 145, 99, 110, 98, 138, 66, 96, 159};
            bool first = true;
            for (int i = 0; i < pids.size(); ++i) {
                algocomp::AttributeData attr_data;
                attr_data.set_long_val(pids[i]);

                if (first) {
                    auto ad = attrs_pb.ads(0);
                    (*ad.mutable_attrs())["agentProductId"] = attr_data;

                    attrs_pb.set_labels(0, -1);
                    first = false;
                } else {
                    auto ad = attrs_pb.add_ads();
                    (*ad->mutable_attrs())["agentProductId"] = attr_data;

                    attrs_pb.add_labels(-1);
                }
            }

            bool ad_found = false;
            for (int i = 0; i < pids.size(); ++i) {
                if (true_ad_id == pids[i]) {
                    attrs_pb.set_labels(i, 1);
                    ad_found = true;
                }
            }

            if (!ad_found) {
                std::cout << "AD NOT FOUND!" << std::endl;
            }

            attrs_pb.SerializeToString(&attrs_pb_str);
//            print_message(attrs_pb);

            /** offline data transform **/
            // (user and context, multi-ads) data  ==>  [(user and context, ad0), (user and context, ad1), ...]
            std::vector<std::string> enc_examples = plh2m->head_to_middle(attrs_pb_str.c_str(), attrs_pb_str.length());

            /** offline predict by call python shell **/
            PyObject *pList = PyList_New(enc_examples.size());
            for (int i = 0; i < enc_examples.size(); ++i) {
                PyList_SetItem(pList, i, Py_BuildValue("s", enc_examples[i].c_str()));
            }

            // python function method parameters
            PyObject *pArg0 = Py_BuildValue("O", pList);
            PyObject *pArg1 = Py_BuildValue("s",
                                            "./test/test-data/online/skl-model.joblib");
            PyObject *pArg2 = Py_BuildValue("s",
                                            "./test/test-data/online/hash_to_seq.txt");
            PyTuple_SetItem(pArgs, 0, pArg0);
            PyTuple_SetItem(pArgs, 1, pArg1);
            PyTuple_SetItem(pArgs, 2, pArg2);


            if (pFunc) {
                PyObject *pReturn = PyEval_CallObject(pFunc, pArgs);
                for (int i = 0; i < PyList_Size(pReturn); ++i) {
                    offline_results.push_back(PyUnicode_AsUTF8(PyList_GetItem(pReturn, i)));
                }
                for (int i = 0; i < offline_results.size(); ++i) {
                    std::vector<std::string> tmp = absl::StrSplit(offline_results[i], ';');
                    std::for_each(tmp.begin(), tmp.end(), [&](string s) {
                        std::vector<std::string> t = absl::StrSplit(s, ':');
                        if (t[0] == "P1") {
                            offline_scores.push_back(std::stof(t[1]));
                        }
                    });
                }
            } else {
                std::cout << "[ERROR] pFunc == NULL" << std::endl;
            }

            /** online predict **/
            // pl-head
            auto feats = plh->work(attrs_pb_str.c_str(), attrs_pb_str.length());

            Base64::Decode(feats, &feats_pb_str);

            // pl-middle
            auto encs = plm->work_tokv(feats_pb_str.c_str(), feats_pb_str.length());

            Base64::Decode(encs, &encs_pb_str);

            // pl-tail
            online_scores = plt->work(encs_pb_str.c_str(), encs_pb_str.length());

            /** compare offline score and online score **/
            for (int i = 0; i < online_scores.size(); ++i) {
                std::cout << "sample" << j << " ad" << i << "==> " << "online: " << online_scores[i] << " VS "
                          << "offline: "
                          << offline_scores[i] << std::endl;
                EXPECT_NEAR(online_scores[i], offline_scores[i], 1E-4);
            }
            j++;

            /** valid 10 samples **/
            if (j == 10) {
                break;
            }
        }

        Py_DECREF(pModule);
        Py_Finalize();
    }
}
