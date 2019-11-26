//
// Created by qfeng on 2018-12-27.
//

#include "pipeline/pl_tail.h"
#include "model/model_daemon.h"
#include "ctpl.h"
#include <boost/thread/latch.hpp>


namespace algocomp {

    float PLTail::sigmoid(float inp_score) {
        if (std::isfinite(inp_score)) {
            return 1.0 / (1.0 + exp(-inp_score));
        } else return 0.5;
    }

    const std::vector<float> PLTail::work_on_none_mode(AUCImprEncs &inp_encs) {
        // encs->kvs->scores

        std::vector<float> outp;
        try {
            for (int i = 0; i < inp_encs.encs_size(); ++i) {
                auto enc_bunch = inp_encs.encs(i);

                Eigen::SparseVector<float> kv(inp_encs.feat_size());
                kv.reserve(enc_bunch.encs_size());
                for (auto enc: enc_bunch.encs()) {
                    kv.insert(enc.enc()) = enc.val();
                }

                if (_model) {
                    // Model score and sigmoid transform
                    auto score = _model->score(kv);
                    outp.push_back(sigmoid(score));
                } else {
                    _logger->error("Use (separately) empty model daemon.");
                }
            }

        } catch (const exception &ex) {
            _logger->error("Predict score (separately used) fail in pltail (thread type: {}) : {}",
                           _thread_type_str,
                           ex.what());
        }

        return outp;
    }

    const std::vector<float> PLTail::work_on_example_mode(AUCImprEncs &inp_encs) {
        std::vector<float> outp;
        try {
            boost::latch count_down_latch(1);
            auto mth_func = [this, &outp, &inp_encs, &count_down_latch](int id) {
                for (int i = 0; i < inp_encs.encs_size(); ++i) {
                    auto enc_bunch = inp_encs.encs(i);
                    Eigen::SparseVector<float> kv(inp_encs.feat_size());
                    kv.reserve(enc_bunch.encs_size());
                    for (auto enc: enc_bunch.encs()) {
                        kv.insert(enc.enc()) = enc.val();
                    }

                    if (_model) {
                        // Model score and sigmoid transform
                        auto score = _model->score(kv);
                        outp.push_back(sigmoid(score));
                    } else {
                        _logger->error("Use (separately) empty model daemon.");
                    }
                }
                count_down_latch.count_down();
            };

            _thp_rsrc->push(mth_func);
            count_down_latch.wait();
        } catch (const exception &ex) {
            _logger->error("Predict score (separately used) fail in pltail (thread type: {}) : {}",
                           _thread_type_str,
                           ex.what());
        }

        return outp;
    }

    const std::vector<float> PLTail::work_on_ads_mode(AUCImprEncs &inp_encs) {
        std::vector<float> outp;
        try {
            std::vector<std::pair<int, float>> outp_;
            boost::latch count_down_latch(inp_encs.encs_size());
            std::mutex res_mutex;

            auto mth_func = [this, &outp_, &res_mutex, &count_down_latch, &inp_encs](int id, int index) {
                try {
                    auto enc_bunch = inp_encs.encs(index);
                    Eigen::SparseVector<float> kv(inp_encs.feat_size());
                    kv.reserve(enc_bunch.encs_size());
                    for (auto enc: enc_bunch.encs()) {
                        kv.insert(enc.enc()) = enc.val();
                    }

                    if (_model) {
                        auto score = _model->score(kv);
                        {
                            std::lock_guard<std::mutex> lock(res_mutex);
                            outp_.emplace_back(index, sigmoid(score));
                        }
                    } else {
                        _logger->error("Use (separately) empty model daemon.");
                    }

                } catch (const std::exception &ex) {
                    _logger->error("Predict score fail (in thread {}) in pltail: {}", id, ex.what());
                }
                count_down_latch.count_down();
            };

            for (int i = 0; i < inp_encs.encs_size(); ++i) {
                _thp_rsrc->push(mth_func, i);
            }
            count_down_latch.wait();

            if (outp_.size() > 0 && outp_.size() == inp_encs.encs_size()) {
                std::sort(outp_.begin(), outp_.end(),
                          [](const std::pair<int, float> &lhs, const std::pair<int, float> &rhs) {
                              return lhs.first < rhs.first;
                          });
                std::for_each(outp_.begin(), outp_.end(), [&outp](std::pair<int, float> ss) {
                    outp.push_back(ss.second);
                });
            } else {
                _logger->warn("Number of predicted scores is zero or inconsistent with input size {} != {}",
                              outp_.size(), inp_encs.encs_size());
            }
        } catch (const exception &ex) {
            _logger->error("Predict score (separately used) fail in pltail (thread type: {}) : {}",
                           _thread_type_str,
                           ex.what());
        }

        return outp;
    }

    const std::vector<float> PLTail::work_on_none_mode(const std::vector<Eigen::SparseVector<float>> &kv) {
        std::vector<float> outp;
        try {
            if (_model) {
                for (int i = 0; i < kv.size(); ++i) {
                    // Model score and sigmoid transform
                    auto score = _model->score(kv[i]);
                    outp.push_back(sigmoid(score));
                }
            } else {
                _logger->error("Use (separately) empty model daemon.");
            }
        } catch (const exception &ex) {
            _logger->error("Predict score (separately used) fail in pltail (thread type: {}) : {}",
                           _thread_type_str,
                           ex.what());
        }

        if (outp.size() != kv.size()) {
            _logger->error("Inconsistent size of input kvs {} and output socres {} (thread type: {}).", kv.size(),
                           outp.size(), _thread_type_str);
            outp.clear();
        }

        return outp;
    }

    const std::vector<float> PLTail::work_on_example_mode(const std::vector<Eigen::SparseVector<float>> &kv) {
        std::vector<float> outp;
        try {
            if (_model) {
                boost::latch count_down_latch(1);
                auto mth_func = [this, &kv, &outp, &count_down_latch](int id) {
                    for (int i = 0; i < kv.size(); ++i) {
                        // Model score and sigmoid transform
                        auto score = _model->score(kv[i]);
                        outp.push_back(sigmoid(score));
                    }
                    count_down_latch.count_down();
                };
                _thp_rsrc->push(mth_func);
                count_down_latch.wait();
            } else {
                _logger->error("Use (separately) empty model daemon.");
            }

        } catch (const exception &ex) {
            _logger->error("Predict score (separately used) fail in pltail (thread type: {}) : {}",
                           _thread_type_str,
                           ex.what());
        }

        return outp;

    }

    const std::vector<float> PLTail::work_on_ads_mode(const std::vector<Eigen::SparseVector<float>> &kv) {
        std::vector<float> outp;
        try {
            if (_model) {
                std::vector<std::pair<int, float>> outp_;
                boost::latch count_down_latch(kv.size());
                std::mutex res_mutex;
                auto mth_func = [this, &kv, &outp_, &count_down_latch, &res_mutex](int id, int index) {
                    {
                        auto score = _model->score(kv[index]);
                        {
                            std::lock_guard<std::mutex> lock(res_mutex);
                            outp_.emplace_back(index, sigmoid(score));
                        }
                    }
                    count_down_latch.count_down();
                };
                for (int i = 0; i < kv.size(); ++i) {
                    _thp_rsrc->push(mth_func, i);
                }
                count_down_latch.wait();

                if (outp_.size() > 0 && outp_.size() == kv.size()) {
                    std::sort(outp_.begin(), outp_.end(),
                              [](const std::pair<int, float> &lhs, const std::pair<int, float> &rhs) {
                                  return lhs.first < rhs.first;
                              });
                    std::for_each(outp_.begin(), outp_.end(), [&outp](std::pair<int, float> v) {
                        outp.push_back(v.second);
                    });

                } else {
                    _logger->warn("Number of predicted scores is zero or inconsistent with input size {} != {}",
                                  outp_.size(), kv.size());
                }
            } else {
                _logger->error("Use (separately) empty model daemon.");
            }

        } catch (const exception &ex) {
            _logger->error("Predict score (separately used) fail in pltail (thread type: {}) : {}",
                           _thread_type_str,
                           ex.what());
        }

        return outp;
    }

    const std::vector<float> PLTail::work(const char *encs_pb, long len_encs_pb) {
        std::vector<float> outp;
        google::protobuf::Arena arena;

        try {
            AUCImprEncs *encs = google::protobuf::Arena::CreateMessage<AUCImprEncs>(&arena);
            encs->ParseFromArray(encs_pb, len_encs_pb);

            if (_thread_type == ThreadType::NO) {
                outp = work_on_none_mode(*encs);
            } else if (_thread_type == ThreadType::ON_EXAMPLE) {
                outp = work_on_example_mode(*encs);
            } else if (_thread_type == ThreadType::ON_ADS) {
                outp = work_on_ads_mode(*encs);
            } else {
                _logger->error("Unknown thread type.");
            }
        } catch (const exception &ex) {
            _logger->error("Predict scores (separately used) fail in pltail: {}", ex.what());
        }
        if (arena.SpaceAllocated()) {
            arena.Reset();
        }

        return outp;
    }

    const std::vector<float> PLTail::work(const vector<Eigen::SparseVector<float>> &kv) {
        std::vector<float> outp;
        if (_thread_type == ThreadType::NO) {
            outp = work_on_none_mode(kv);
        } else if (_thread_type == ThreadType::ON_EXAMPLE) {
            outp = work_on_example_mode(kv);

        } else if (_thread_type == ThreadType::ON_ADS) {
            outp = work_on_ads_mode(kv);
        } else {
            _logger->error("Unknown thread type.");
        }
        return outp;
    }
}
