//
// Created by qfeng on 2018-12-27.
//

#include "pipeline/pl_middle.h"
#include "featpb/auc_impr_feats.pb.h"
#include "encode/feat_enc_daemon.h"
#include <boost/thread/latch.hpp>
#include "encpb/auc_impr_encs.pb.h"
#include "base64.h"

namespace algocomp {

    const std::vector<std::string> PLMiddle::work_toenc(const char *feats_pb, long len_feats_pb) {
        vector<string> outp;
        google::protobuf::Arena arena;

        try {
            AUCImprFeats *feats = google::protobuf::Arena::CreateMessage<AUCImprFeats>(&arena);
            feats->ParseFromArray(feats_pb, len_feats_pb);

            for (int i = 0; i < _feat_transers.size(); ++i) {
                // Transform feature data cyclically
                if (!_feat_transers[i]->apply(*feats, arena)) {
                    _logger->error("Transform feature data (inner used) fail in plmiddle (thread type: {}).",
                                   _thread_type_str);
                }
            }

            if (_feat_enc != nullptr) {
                // Encode feature data
                outp = _feat_enc->to_str(*feats);
            } else {
                _logger->error("Encode feature data with empty feature encoder (thread type: {}).", _thread_type_str);
            }
        } catch (const exception &ex) {
            _logger->error("Transform data (inner used) fail in plmiddle (thread type: {}) : {}.", _thread_type_str,
                           ex.what());
        }

        if (arena.SpaceAllocated()) {
            arena.Reset();
        }
        return outp;
    }

    const std::vector<std::string> PLMiddle::work_toenc(AUCImprFeats &inp_feats, google::protobuf::Arena &arena) {
        try {
            for (int i = 0; i < _feat_transers.size(); ++i) {
                // Transform feature data cyclically
                if (!_feat_transers[i]->apply(inp_feats, arena)) {
                    _logger->error("Transform feature data (inner used) fail in plmiddle (thread type: {}).",
                                   _thread_type_str);
                }
            }

            if (_feat_enc != nullptr) {
                // Encode feature data
                return _feat_enc->to_str(inp_feats);
            } else {
                _logger->error("Encode feature data with empty feature encoder (thread type: {}).", _thread_type_str);
            }
        } catch (const exception &ex) {
            _logger->error("Transform data (inner used) fail in plmiddle (thread type: {}) : {}.", _thread_type_str,
                           ex.what());
        }

        return vector<string>();
    }


    const std::string PLMiddle::work_tokv(const char *feats_pb, long len_feats_pb) {
        string outp = "";
        google::protobuf::Arena arena;
        optional<vector<Eigen::SparseVector<float>>> enc_kv_;

        try {
            AUCImprFeats *feats = google::protobuf::Arena::CreateMessage<AUCImprFeats>(&arena);
            feats->ParseFromArray(feats_pb, len_feats_pb);

            for (int i = 0; i < _feat_transers.size(); ++i) {
                // Transform feature data cyclically
                if (!_feat_transers[i]->apply(*feats, arena)) {
                    _logger->error("Transform feature data (inner used) fail in plmiddle (thread type: {}).",
                                   _thread_type_str);
                }
            }

            if (_feat_enc != nullptr) {
                // Encode feature data
                auto res = _feat_enc->to_kv(*feats);
                if (res.has_value()) {
                    enc_kv_ = res.value();
                }
            } else {
                _logger->error("Encode feature data with empty feature encoder (thread type: {}).", _thread_type_str);
            }

            if (enc_kv_.has_value() && enc_kv_.value().size() > 0) {
                auto enc_kv = enc_kv_.value();
                if (_thread_type == ThreadType::NO) {
                    AUCImprEncs *auc_enc = google::protobuf::Arena::CreateMessage<AUCImprEncs>(&arena);
                    auc_enc->set_feat_size(_feat_enc->feat_size());
                    for (auto encs : enc_kv) {
                        EncodeBunch *enc_bunch = google::protobuf::Arena::CreateMessage<EncodeBunch>(&arena);
                        for (Eigen::SparseVector<float>::InnerIterator it(encs); it; ++it) {
                            auto f_idx = it.index();
                            auto f_val = it.value();
                            EncodeData *enc_data = google::protobuf::Arena::CreateMessage<EncodeData>(&arena);
                            enc_data->set_enc(f_idx);
                            enc_data->set_val(f_val);
                            enc_bunch->mutable_encs()->AddAllocated(enc_data);
                        }
                        auc_enc->mutable_encs()->AddAllocated(enc_bunch);
                    }
                    string auc_enc_pb;
                    auc_enc->SerializeToString(&auc_enc_pb);
                    string encoded;
                    Base64::Encode(auc_enc_pb, &encoded);
                    outp = encoded;
                } else if (_thread_type == ThreadType::ON_EXAMPLE) {
                    boost::latch count_down_latch(1);
                    AUCImprEncs *auc_enc = google::protobuf::Arena::CreateMessage<AUCImprEncs>(&arena);
                    auc_enc->set_feat_size(_feat_enc->feat_size());
                    auto thread_func = [this, &enc_kv, &auc_enc, &count_down_latch, &arena](int id) {
                        for (auto encs : enc_kv) {
                            EncodeBunch *enc_bunch = google::protobuf::Arena::CreateMessage<EncodeBunch>(&arena);
                            for (Eigen::SparseVector<float>::InnerIterator it(encs); it; ++it) {
                                auto f_idx = it.index();
                                auto f_val = it.value();
                                EncodeData *enc_data = google::protobuf::Arena::CreateMessage<EncodeData>(&arena);
                                enc_data->set_enc(f_idx);
                                enc_data->set_val(f_val);
                                enc_bunch->mutable_encs()->AddAllocated(enc_data);
                            }
                            auc_enc->mutable_encs()->AddAllocated(enc_bunch);
                        }
                        count_down_latch.count_down();
                    };
                    _thp_rsrc->push(thread_func);
                    count_down_latch.wait();
                    if (auc_enc->encs_size() > 0 && auc_enc->encs_size() == enc_kv.size()) {
                        string auc_enc_pb;
                        auc_enc->SerializeToString(&auc_enc_pb);
                        string encoded;
                        Base64::Encode(auc_enc_pb, &encoded);
                        outp = encoded;
                    } else {
                        _logger->warn("Wrap encode size is zero or inconsistent with input size {} != {}",
                                      auc_enc->encs_size(),
                                      enc_kv.size());
                    }
                } else if (_thread_type == ThreadType::ON_ADS) {
                    vector<pair<int, EncodeBunch *>> outp_;
                    boost::latch count_down_latch(enc_kv.size());
                    std::mutex res_mutex;

                    auto thread_func = [this, &outp_, &arena, &count_down_latch, &res_mutex](
                            int id, int idx, Eigen::SparseVector<float> &encs) {
                        try {
                            EncodeBunch *enc_bunch = google::protobuf::Arena::CreateMessage<EncodeBunch>(&arena);
                            for (Eigen::SparseVector<float>::InnerIterator it(encs); it; ++it) {
                                auto f_idx = it.index();
                                auto f_val = it.value();
                                EncodeData *enc_data = google::protobuf::Arena::CreateMessage<EncodeData>(&arena);
                                enc_data->set_enc(f_idx);
                                enc_data->set_val(f_val);
                                enc_bunch->mutable_encs()->AddAllocated(enc_data);
                            }

                            auto to_add = make_pair(idx, enc_bunch);
                            {
                                std::lock_guard<std::mutex> lock(res_mutex);
                                outp_.push_back(to_add);
                            }
                        } catch (const exception &ex) {
                            _logger->error("Wrap encode data fail (in thread {}) in plmiddle: {}", id, ex.what());
                        }

                        count_down_latch.count_down();
                    };

                    for (int i = 0; i < enc_kv.size(); ++i) {
                        _thp_rsrc->push(thread_func, i, enc_kv[i]);
                    }

                    count_down_latch.wait();

                    if (outp_.size() > 0 && outp_.size() == enc_kv.size()) {
                        AUCImprEncs *auc_enc = google::protobuf::Arena::CreateMessage<AUCImprEncs>(&arena);
                        auc_enc->set_feat_size(_feat_enc->feat_size());

                        // Sort kv in k's ascending order
                        std::sort(outp_.begin(), outp_.end(),
                                  [](const pair<int, EncodeBunch *> &lhs, const pair<int, EncodeBunch *> &rhs) {
                                      return lhs.first < rhs.first;
                                  });

                        for_each(outp_.begin(), outp_.end(), [&auc_enc](pair<int, EncodeBunch *> e) {
                            auc_enc->mutable_encs()->AddAllocated(e.second);
                        });

                        string auc_enc_pb;
                        auc_enc->SerializeToString(&auc_enc_pb);
                        string encoded;
                        Base64::Encode(auc_enc_pb, &encoded);
                        outp = encoded;
                    } else {
                        _logger->warn("Wrap encode size is zero or inconsistent with input size {} != {}", outp_.size(),
                                      enc_kv.size());
                    }
                } else {
                    _logger->error("Unknown thread type.");
                }
            }
        } catch (const exception &ex) {
            _logger->error("Transform data (separately used) fail in plmiddle: {}", ex.what());
        }

        if (arena.SpaceAllocated()) arena.Reset();
        return outp;
    }

    const optional<vector<Eigen::SparseVector<float>>>
    PLMiddle::work_tokv(AUCImprFeats &inp_feats, google::protobuf::Arena &arena) {
        try {
            for (int i = 0; i < _feat_transers.size(); ++i) {
                // Transform feature data cyclically
                if (!_feat_transers[i]->apply(inp_feats, arena)) {
                    _logger->error("Transform feature data (inner used) fail in plmiddle (thread type: {}).",
                                   _thread_type_str);
                }
            }

            if (_feat_enc != nullptr) {
                // Encode feature data
                return _feat_enc->to_kv(inp_feats);
            } else {
                _logger->error("Encode feature data with empty feature encoder (thread type: {}).", _thread_type_str);
            }

        } catch (const exception &ex) {
            _logger->error("Transform data (inner used) fail in plmiddle(thread type: {}) : {}.", _thread_type_str,
                           ex.what());
        }

        return std::nullopt;
    }

    bool PLMiddle::save_name2enc(const string &fname) {
        try {
            if (_feat_enc != nullptr) {
                if (!_feat_enc->save_name2enc(fname)) {
                    _logger->error("Save name2enc map fail.");
                }
            } else {
                _logger->error("Save name2enc map with empty feature encoder.");
            }
        } catch (const exception &ex) {
            _logger->error("Save name2enc map fail {}.", ex.what());
        }
    }

    long PLMiddle::encode_single_feature(const string &ns, const string &feat_name) {
        if (_feat_enc) {
            return _feat_enc->encode_single_feature(ns, feat_name);
        } else {
            _logger->error("Use empty feature encoder daemon.");
            return 0;
        }
    }
}
