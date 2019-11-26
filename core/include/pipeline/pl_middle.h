//
// Created by qfeng on 2018-12-27.
//

#ifndef ALGOCOMP_PL_MIDDLE_H
#define ALGOCOMP_PL_MIDDLE_H

#include "pl_comp.h"
#include "feature/feat_transer.h"
#include "log/algocomp_log.h"
#include <Eigen/Sparse>
#include "common/type_def.h"


// Declare referenced class
namespace ctpl {
    class thread_pool;
}

namespace algocomp {

    // Declare referenced classes
    class FeatTranser;

    class AUCImprFeats;

    class AUCImprEncs;

    class FeatEncDaemon;

    /**
     * Pipeline Middle
     *
     * Define context of pipeline middle (e.g., feature transformers and feature encoder)
     */
    class PLMiddle : public PLComp {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Feature transformers
        vector<shared_ptr<FeatTranser>> _feat_transers;

        // Feature encoder
        shared_ptr<FeatEncDaemon> _feat_enc;
    public:
        // Constructor
        PLMiddle(const vector<shared_ptr<FeatTranser>> feat_transers,
                 shared_ptr<ctpl::thread_pool> thp_rsrc,
                 shared_ptr<FeatEncDaemon> feat_enc,
                 ThreadType thread_type = ThreadType::NO) : _feat_transers(feat_transers), _feat_enc(feat_enc),
                                                            PLComp(thp_rsrc, thread_type) {
            _logger = spdlog::get(ACLog::_logger_name);

            _logger->info("PLMiddle uses thread type {}.", _thread_type_str);
        }

        // Separately used for transform feature data into encodes (libsvm or vowpal wabbit string format)
        const std::vector<std::string> work_toenc(const char *feats_pb, long len_feats_pb);

        const std::vector<std::string> work_toenc(AUCImprFeats &inp_feats, google::protobuf::Arena &arena);

        // Separately used for transform feature data into encodes (base64 encoded protobuf string)
        const std::string work_tokv(const char *feats_pb, long len_feats_pb);

        const optional<vector<Eigen::SparseVector<float>>> work_tokv(AUCImprFeats &inp_feats, google::protobuf::Arena &arena);

        // Save the map of names to encodes (for offline encoding mode)
        bool save_name2enc(const string &fname);

        // Encode single feature
        long encode_single_feature(const string &ns, const string &feat_name);

    };

}


#endif //ALGOCOMP_PL_MIDDLE_H
