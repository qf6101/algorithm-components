//
// Created by qfeng on 2018-12-27.
//

#ifndef ALGOCOMP_PL_TAIL_H
#define ALGOCOMP_PL_TAIL_H

#include "pl_comp.h"
#include "log/algocomp_log.h"
#include <Eigen/Sparse>
#include "common/type_def.h"
#include "encpb/auc_impr_encs.pb.h"


// Declare referenced class
namespace ctpl {
    class thread_pool;
}


namespace algocomp {

    // Declare referenced class
    class ModelDaemon;

    /**
     * Pipeline Tail
     *
     * Define context of pipeline tail (i.e., model)
     */
    class PLTail : public PLComp {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Model daemon
        shared_ptr<ModelDaemon> _model;

        // Sigmoid transform
        float sigmoid(float inp_score);

        // Used by "work" function on none mode
        const std::vector<float> work_on_none_mode(AUCImprEncs &inp_encs);

        const std::vector<float> work_on_none_mode(const std::vector<Eigen::SparseVector<float>> &kv);

        // Used by "work" function on example mode
        const std::vector<float> work_on_example_mode(AUCImprEncs &inp_encs);

        const std::vector<float> work_on_example_mode(const std::vector<Eigen::SparseVector<float>> &kv);

        // Used by "work" function on ads mode
        const std::vector<float> work_on_ads_mode(AUCImprEncs &inp_encs);

        const std::vector<float> work_on_ads_mode(const std::vector<Eigen::SparseVector<float>> &kv);

    public:
        // Constructor
        PLTail(shared_ptr<ModelDaemon> model, shared_ptr<ctpl::thread_pool> thp_rsrc,
               const ThreadType thread_type = ThreadType::NO) :
                PLComp(thp_rsrc, thread_type), _model(model) {
            _logger = spdlog::get(ACLog::_logger_name);

            _logger->info("PLTail uses thread type {}.", _thread_type_str);
        }

        // Separately used for predicting scores
        const std::vector<float> work(const char *encs_pb, long len_encs_pb);

        const std::vector<float> work(const vector<Eigen::SparseVector<float>> &kv);
    };

}


#endif //ALGOCOMP_PL_TAIL_H
