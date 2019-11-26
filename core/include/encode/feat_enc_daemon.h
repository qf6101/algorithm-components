//
// Created by qfeng on 2019-01-21.
//

#ifndef ALGORITHM_COMPONENTS_FEAT_ENC_DAEMON_H
#define ALGORITHM_COMPONENTS_FEAT_ENC_DAEMON_H

#include "encode/feature_encoder.h"
#include <memory>
#include <Eigen/Sparse>
#include "log/algocomp_log.h"
#include "featpb/cont_feat_grp.pb.h"
#include "featpb/disc_feat_grp.pb.h"
#include "common/type_def.h"
#include "ctpl.h"

using namespace std;

namespace algocomp {

    // Declare referenced class
    class AUCImprFeats;

    // Define featuren encode scope
    enum class FeatEncScope {
        ALL = 1, // Discrete and continuous features
        DISC = 2, // Discrete features
        CONT = 3 // Continuous features
    };

    // Define feature encode mode
    enum class FeatEncMode {
        ACCUMULATING = 1, // Offline: used for "save_name2enc" function
        PROCESSING = 2 // Online/offline: merely encode feature data one by one
    };

    // Define feature encode string format
    enum class FeatEncStrFormat {
        VW = 1, // Vowpal wabbit string format
        LIBSVM = 2 // Libsvm sgtring format
    };

    /**
     * Feature encode daemon
     *
     * Define context (e.g., scope, mode and string format) and daemon (e.g., thread type and map of names to encodes)
     * of feature encoding.
     */
    class FeatEncDaemon {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Feature encoder (feature hash or feature map)
        const shared_ptr<FeatureEncoder> _feat_enc;

        // Feature encode scope (discrete features, continuous features or all of them)
        const FeatEncScope _enc_scope;

        // Feature encode mode (accumulating or processing)
        const FeatEncMode _enc_mode;

        // Feature encode string format (vowpal wabbit or libsvm format)
        const FeatEncStrFormat _enc_str_format = FeatEncStrFormat::LIBSVM;

        // Map of names to encodes
        map<string, long> _name2enc;

        // Parallel mutex for recording map of names to encodes in multi-threads scene
        std::mutex _record_name2enc_mutex;

        // Thread type (on-example or on-ads)
        const ThreadType _thread_type;

        // Thread pool resource
        std::shared_ptr<ctpl::thread_pool> _thp_rsrc;

        // Encode the continuous features
        void enc_cont_feats(const ::google::protobuf::Map<::std::string, ContFeatGrp> &inp_feats,
                            Eigen::SparseVector<float> &kv);

        // Encode the discrete features
        void enc_disc_feats(const ::google::protobuf::Map<::std::string, DiscFeatGrp> &inp_feats,
                            Eigen::SparseVector<float> &kv);

        const optional<vector<Eigen::SparseVector<float>>> enc_on_none_mode(const AUCImprFeats &inp_feats);

        const optional<vector<Eigen::SparseVector<float>>> enc_on_ads_mode(const AUCImprFeats &inp_feats);

        const optional<vector<Eigen::SparseVector<float>>> enc_on_example_mode(const AUCImprFeats &inp_feats);

        // Encode features to libsvm string format
        const vector<string> to_libsvm(const AUCImprFeats &inp_feats);

        // Encode features to vowpal wabbit string format
        const vector<string> to_vw(const AUCImprFeats &inp_feats);

    public:

        // Constructor with initializations
        FeatEncDaemon(const shared_ptr<FeatureEncoder> feat_enc, const FeatEncScope enc_scope,
                      const FeatEncMode enc_mode, const FeatEncStrFormat enc_str_format,
                      std::shared_ptr<ctpl::thread_pool> thp_rsrc,
                      ThreadType thread_type = ThreadType::NO) : _feat_enc(feat_enc),
                                                                 _enc_scope(enc_scope),
                                                                 _enc_mode(enc_mode),
                                                                 _enc_str_format(enc_str_format),
                                                                 _thp_rsrc(thp_rsrc),
                                                                 _thread_type(thread_type) {
            _logger = spdlog::get(ACLog::_logger_name);
        }

        // Encode features to libsvm or vw string format
        const vector<string> to_str(const AUCImprFeats &inp_feats);

        // Encode features to sparse key-value representation
        const optional<vector<Eigen::SparseVector<float>>> to_kv(const AUCImprFeats &inp_feats);

        // Save the map of names to encodes
        bool save_name2enc(const string &fname);

        // Encode single feature to encode as tool function
        uint64_t encode_single_feature(const string &ns, const string &feat_name);

        // Get feature size
        uint64_t feat_size();
    };

}


#endif //ALGORITHM_COMPONENTS_FEAT_ENC_DAEMON_H
