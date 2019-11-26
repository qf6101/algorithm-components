//
// Created by qfeng on 2019-01-21.
//

#include "encode/feat_enc_daemon.h"
#include "featpb/auc_impr_feats.pb.h"
#include <boost/thread/latch.hpp>
#include <fstream>
#include <sstream>

using namespace std;


namespace algocomp {

    void FeatEncDaemon::enc_cont_feats(const ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp> &inp_feats,
                                       Eigen::SparseVector<float> &kv) {
        for (auto &feat_grp: inp_feats) {
            for (auto &feat: feat_grp.second.feats()) {
                // Encode continuous feature one by one
                auto k = _feat_enc->encode_single_feature("", feat.second.fname());
                auto v = feat.second.fval();
                if (k > 0) kv.insert(k) = v;

                if (_enc_mode == FeatEncMode::ACCUMULATING) {
                    // Add to map of names to encodes in accumulating mode
                    {
                        std::lock_guard<std::mutex> lock(_record_name2enc_mutex);
                        _name2enc[feat.second.fname()] = k;
                    }
                }
            }
        }
    }

    void FeatEncDaemon::enc_disc_feats(const ::google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp> &inp_feats,
                                       Eigen::SparseVector<float> &kv) {
        for (auto &feat_grp: inp_feats) {
            for (auto &feat: feat_grp.second.feats()) {
                // Encode discrete features one by one
                auto k = _feat_enc->encode_single_feature("", feat.second.fname());
                auto v = 1.0;
                if (k > 0) kv.insert(k) = v;

                if (_enc_mode == FeatEncMode::ACCUMULATING) {
                    // Add to map of names to encodes in accumulating mode
                    {
                        std::lock_guard<std::mutex> lock(_record_name2enc_mutex);
                        _name2enc[feat.second.fname()] = k;
                    }
                }
            }
        }
    }

    const optional<vector<Eigen::SparseVector<float>>>
    FeatEncDaemon::enc_on_ads_mode(const algocomp::AUCImprFeats &inp_feats) {
        try {
            int uc_disc_size = inp_feats.user_and_ctx().disc_feats_size();
            int uc_cont_size = inp_feats.user_and_ctx().cont_feats_size();

            // multi-thread in encode user-and-context
            boost::latch count_down_latch_for_uc(1);
            Eigen::SparseVector<float> uc_kv(_feat_enc->size());
            auto mth_func_for_uc = [this, &inp_feats, &uc_kv, &uc_disc_size, &uc_cont_size, &count_down_latch_for_uc](
                    int id) {

                if (_enc_scope == FeatEncScope::CONT) { // Encode only continuous features
                    uc_kv.reserve(uc_cont_size);
                    enc_cont_feats(inp_feats.user_and_ctx().cont_feats(), uc_kv);
                } else if (_enc_scope == FeatEncScope::DISC) { //Encode only discrete features
                    uc_kv.reserve(uc_disc_size);
                    enc_disc_feats(inp_feats.user_and_ctx().disc_feats(), uc_kv);
                } else { // Encode continuous and discrete features
                    uc_kv.reserve(uc_cont_size + uc_disc_size);
                    enc_cont_feats(inp_feats.user_and_ctx().cont_feats(), uc_kv);
                    enc_disc_feats(inp_feats.user_and_ctx().disc_feats(), uc_kv);
                }
                count_down_latch_for_uc.count_down();
            };
            _thp_rsrc->push(mth_func_for_uc);
            count_down_latch_for_uc.wait();

            // multi-thread in encode ads
            std::mutex res_mutex;
            boost::latch count_down_latch_for_ads(inp_feats.ads_size());
            std::vector<std::pair<int, Eigen::SparseVector<float>>> outp_;
            outp_.reserve(inp_feats.ads_size());

            auto mth_func_for_ads = [this, &uc_kv, &res_mutex, &inp_feats, &uc_disc_size, &uc_cont_size, &outp_, &count_down_latch_for_ads](
                    int id, int index) {
                auto ad = inp_feats.ads(index);
                int ad_disc_size = ad.disc_feats_size();
                int ad_cont_size = ad.cont_feats_size();

                Eigen::SparseVector<float> kv(_feat_enc->size());

                if (_enc_scope == FeatEncScope::CONT) { // Encode only continuous features
                    kv.reserve(uc_cont_size + ad_cont_size);
                    kv = uc_kv;
                    enc_cont_feats(ad.cont_feats(), kv);
                } else if (_enc_scope == FeatEncScope::DISC) { //Encode only discrete features
                    kv.reserve(uc_disc_size + ad_disc_size);
                    kv = uc_kv;
                    enc_disc_feats(ad.disc_feats(), kv);
                } else { // Encode continuous and discrete features
                    kv.reserve(uc_cont_size + uc_disc_size + ad_cont_size + ad_disc_size);
                    kv = uc_kv;
                    enc_cont_feats(ad.cont_feats(), kv);
                    enc_disc_feats(ad.disc_feats(), kv);
                }

                {
                    std::lock_guard<std::mutex> lock(res_mutex);
                    outp_.emplace_back(index, std::move(kv));
                }

                count_down_latch_for_ads.count_down();
            };

            for (int i = 0; i < inp_feats.ads_size(); ++i) {
                _thp_rsrc->push(mth_func_for_ads, i);
            }
            count_down_latch_for_ads.wait();

            // sort
            std::sort(outp_.begin(), outp_.end(),
                      [](const std::pair<int, Eigen::SparseVector<float>> &a,
                         const std::pair<int, Eigen::SparseVector<float>> &b) { return a.first < b.first; });

            std::vector<Eigen::SparseVector<float>> outp;
            outp.reserve(outp_.size());
            for (auto v: outp_) {
                outp.emplace_back(std::move(v.second));
            }

            return outp;

        } catch (const exception &ex) {
            _logger->error("Encode features(in thread type on-ads) fail: {}.", ex.what());
        }

        return std::nullopt;

    }

    const optional<vector<Eigen::SparseVector<float>>> FeatEncDaemon::enc_on_example_mode(
            const algocomp::AUCImprFeats &inp_feats) {
        try {
            std::vector<Eigen::SparseVector<float>> outp;
            outp.reserve(inp_feats.ads_size());
            boost::latch count_down_latch(1);

            auto mth_func = [this, &inp_feats, &outp, &count_down_latch](int id) {
                // encode user-and-context
                int uc_disc_size = inp_feats.user_and_ctx().disc_feats_size();
                int uc_cont_size = inp_feats.user_and_ctx().cont_feats_size();
                Eigen::SparseVector<float> uc_kv(_feat_enc->size());
                if (_enc_scope == FeatEncScope::CONT) { // Encode only continuous features
                    uc_kv.reserve(uc_cont_size);
                    enc_cont_feats(inp_feats.user_and_ctx().cont_feats(), uc_kv);
                } else if (_enc_scope == FeatEncScope::DISC) { //Encode only discrete features
                    uc_kv.reserve(uc_disc_size);
                    enc_disc_feats(inp_feats.user_and_ctx().disc_feats(), uc_kv);
                } else { // Encode continuous and discrete features
                    uc_kv.reserve(uc_cont_size + uc_disc_size);
                    enc_cont_feats(inp_feats.user_and_ctx().cont_feats(), uc_kv);
                    enc_disc_feats(inp_feats.user_and_ctx().disc_feats(), uc_kv);
                }

                // encode ads
                for (auto ad: inp_feats.ads()) {
                    // Prepare features in ad
                    int ad_disc_size = ad.disc_feats_size();
                    int ad_cont_size = ad.cont_feats_size();

                    Eigen::SparseVector<float> kv(_feat_enc->size());

                    if (_enc_scope == FeatEncScope::CONT) { // Encode only continuous features
                        kv.reserve(uc_cont_size + ad_cont_size);
                        kv = uc_kv;
                        enc_cont_feats(ad.cont_feats(), kv);
                    } else if (_enc_scope == FeatEncScope::DISC) { //Encode only discrete features
                        kv.reserve(uc_disc_size + ad_disc_size);
                        kv = uc_kv;
                        enc_disc_feats(ad.disc_feats(), kv);
                    } else { // Encode continuous and discrete features
                        kv.reserve(uc_cont_size + uc_disc_size + ad_cont_size + ad_disc_size);
                        kv = uc_kv;
                        enc_cont_feats(ad.cont_feats(), kv);
                        enc_disc_feats(ad.disc_feats(), kv);
                    }

                    outp.emplace_back(std::move(kv));
                }

                count_down_latch.count_down();
            };
            _thp_rsrc->push(mth_func);
            count_down_latch.wait();
            return outp;
        } catch (const exception &ex) {
            _logger->error("Encode features(in thread type on-ads) fail: {}.", ex.what());
        }

        return std::nullopt;
    }

    const optional<vector<Eigen::SparseVector<float>>>
    FeatEncDaemon::enc_on_none_mode(const algocomp::AUCImprFeats &inp_feats) {
        try {
            // encode user-and-context
            int uc_disc_size = inp_feats.user_and_ctx().disc_feats_size();
            int uc_cont_size = inp_feats.user_and_ctx().cont_feats_size();
            Eigen::SparseVector<float> uc_kv(_feat_enc->size());
            if (_enc_scope == FeatEncScope::CONT) { // Encode only continuous features
                uc_kv.reserve(uc_cont_size);
                enc_cont_feats(inp_feats.user_and_ctx().cont_feats(), uc_kv);
            } else if (_enc_scope == FeatEncScope::DISC) { //Encode only discrete features
                uc_kv.reserve(uc_disc_size);
                enc_disc_feats(inp_feats.user_and_ctx().disc_feats(), uc_kv);
            } else { // Encode continuous and discrete features
                uc_kv.reserve(uc_cont_size + uc_disc_size);
                enc_cont_feats(inp_feats.user_and_ctx().cont_feats(), uc_kv);
                enc_disc_feats(inp_feats.user_and_ctx().disc_feats(), uc_kv);
            }

            std::vector<Eigen::SparseVector<float>> outp;
            outp.reserve(inp_feats.ads_size());

            // encode ads
            for (auto ad: inp_feats.ads()) {
                // Prepare features in ad
                int ad_disc_size = ad.disc_feats_size();
                int ad_cont_size = ad.cont_feats_size();

                Eigen::SparseVector<float> kv(_feat_enc->size());

                if (_enc_scope == FeatEncScope::CONT) { // Encode only continuous features
                    kv.reserve(uc_cont_size + ad_cont_size);
                    kv = uc_kv;
                    enc_cont_feats(ad.cont_feats(), kv);
                } else if (_enc_scope == FeatEncScope::DISC) { //Encode only discrete features
                    kv.reserve(uc_disc_size + ad_disc_size);
                    kv = uc_kv;
                    enc_disc_feats(ad.disc_feats(), kv);
                } else { // Encode continuous and discrete features
                    kv.reserve(uc_cont_size + uc_disc_size + ad_cont_size + ad_disc_size);
                    kv = uc_kv;
                    enc_cont_feats(ad.cont_feats(), kv);
                    enc_disc_feats(ad.disc_feats(), kv);
                }
                outp.emplace_back(std::move(kv));
            }
            return outp;
        } catch (const exception &ex) {
            _logger->error("Encode features fail: {}.", ex.what());
        }
        return std::nullopt;
    }

    const optional<vector<Eigen::SparseVector<float>>> FeatEncDaemon::to_kv(const algocomp::AUCImprFeats &inp_feats) {
        if (_thread_type == ThreadType::NO) {
            return enc_on_none_mode(inp_feats);
        } else if (_thread_type == ThreadType::ON_EXAMPLE) {
            return enc_on_example_mode(inp_feats);
        } else if (_thread_type == ThreadType::ON_ADS) {
            return enc_on_ads_mode(inp_feats);
        } else {
            _logger->error("Unknown thread type.");
            return std::nullopt;
        }
    }

    const vector<string> FeatEncDaemon::to_str(const algocomp::AUCImprFeats &inp_feats) {
        if (_enc_str_format == FeatEncStrFormat::VW) {
            return to_vw(inp_feats);
        } else if (_enc_str_format == FeatEncStrFormat::LIBSVM) {
            return to_libsvm(inp_feats);
        }
    }

    const vector<string> FeatEncDaemon::to_vw(const algocomp::AUCImprFeats &inp_feats) {
        vector<string> outp;

        // Check the consistency of label size and the ad size
        if (inp_feats.labels_size() != inp_feats.ads_size()) {
            _logger->error("Inconsistent label size {} and ad size {}.", inp_feats.labels_size(), inp_feats.ads_size());
            return outp;
        }

        // Encode features to sparse kv representation
        auto kv__ = to_kv(inp_feats);

        if (kv__.has_value()) {
            auto kv_ = kv__.value();

            for (int i = 0; i < kv_.size(); ++i) {
                float label = inp_feats.labels(i);

                auto kv = kv_[i];
                ostringstream ss;

                // Format label string
                if (label > 0) ss << "1 ";
                else ss << "-1 ";
                ss << "| ";

                vector<pair<long, float>> encoded;
                for (Eigen::SparseVector<float>::InnerIterator it(kv); it; ++it) {
                    auto f_idx = it.index();
                    auto f_val = it.value();
                    encoded.emplace_back(f_idx, f_val);
                }

                if (encoded.size() > 0) {
                    // Sort kv in k's ascending order
                    std::sort(encoded.begin(), encoded.end(),
                              [](const pair<int, float> &lhs, const pair<int, float> rhs) {
                                  return lhs.first < rhs.first;
                              });
                    // Format feature strings
                    for_each(encoded.begin(), encoded.end(), [&ss](pair<int, float> f) {
                        if (f.second == 1) {
                            ss << f.first << " ";
                        } else if (f.second != 0) {
                            ss << f.first << ":" << f.second << " ";
                        }
                    });
                    outp.push_back(ss.str());
                } else outp.push_back("");
            }
        }

        return outp;
    }

    const vector<string> FeatEncDaemon::to_libsvm(const algocomp::AUCImprFeats &inp_feats) {
        vector<string> outp;

        // Check the consistency of label size and the ad size
        if (inp_feats.labels_size() != inp_feats.ads_size()) {
            _logger->error("Inconsistent label size {} and ad size {}.", inp_feats.labels_size(), inp_feats.ads_size());
            return outp;
        }

        // Encode features to sparse kv representation
        auto kv__ = to_kv(inp_feats);

        if (kv__.has_value()) {
            auto kv_ = kv__.value();

            for (int i = 0; i < kv_.size(); ++i) {
                float label = inp_feats.labels(i);

                auto kv = kv_[i];
                ostringstream ss;

                // Format label string
                if (label > 0) ss << "+1 ";
                else ss << "-1 ";

                vector<pair<long, float>> encoded;
                for (Eigen::SparseVector<float>::InnerIterator it(kv); it; ++it) {
                    auto f_idx = it.index();
                    auto f_val = it.value();
                    encoded.emplace_back(f_idx, f_val);
                }

                if (encoded.size() > 0) {
                    // Sort kv in k's ascending order
                    std::sort(encoded.begin(), encoded.end(),
                              [](const pair<int, float> &lhs, const pair<int, float> rhs) {
                                  return lhs.first < rhs.first;
                              });
                    // Format feature strings
                    for_each(encoded.begin(), encoded.end(), [&ss](pair<int, float> f) {
                        ss << f.first << ":" << f.second << " ";
                    });
                    outp.push_back(ss.str());
                } else outp.push_back("");
            }
        }

        return outp;
    }

    bool FeatEncDaemon::save_name2enc(const string &fname) {
        try {
            std::ofstream out;
            out.open(fname);

            // Save the map of names to encodes (encode first, name second)
            for (auto &item: _name2enc) {
                out << item.second << " " << item.first << endl;
            }

            out.close();
            return true;
        } catch (const exception &ex) {
            _logger->error("Save name2enc fail: {}", ex.what());
        }

        return false;
    }

    uint64_t FeatEncDaemon::encode_single_feature(const string &ns, const string &feat_name) {
        if (_feat_enc) {
            return _feat_enc->encode_single_feature(ns, feat_name);
        } else {
            _logger->error("Use empty feature encoder.");
            return 0;
        }
    }

    uint64_t FeatEncDaemon::feat_size() {
        if (_feat_enc) {
            return _feat_enc->size();
        } else return 1;
    }
}
