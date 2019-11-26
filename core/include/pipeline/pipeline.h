//
// Created by qfeng on 2018-12-30.
//

#ifndef ALGORITHM_COMPONENTS_PIPELINE_H
#define ALGORITHM_COMPONENTS_PIPELINE_H

#include <spdlog/logger.h>
#include <memory>
#include "pl_comp.h"
#include "pl_head.h"
#include "pl_middle.h"
#include "pl_tail.h"
#include <string>
#include <vector>
#include "common/type_def.h"


using namespace std;


namespace algocomp {

    /**
     * Pipeline
     *
     * Define components of pipeline (e.g., pipeline head, middle and tail)
     */
    class Pipeline : public PLComp {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;
        // Pipeline components
        shared_ptr<PLHead> _head;
        shared_ptr<PLMiddle> _middle;
        shared_ptr<PLTail> _tail;

    public:
        // Constructor
        Pipeline(shared_ptr<PLHead> head, shared_ptr<PLMiddle> middle, shared_ptr<PLTail> tail,
                 shared_ptr<ctpl::thread_pool> thp_rsrc, const ThreadType thread_type = ThreadType::NO) : PLComp(
                thp_rsrc, thread_type), _head(head), _middle(middle), _tail(tail) {
            _logger = spdlog::get(ACLog::_logger_name);

            _logger->info("Pipeline uses thread type {}.", _thread_type_str);
        }

        // Transform with pipeline head and middle in order
        const std::vector<std::string> head_to_middle(const char *attrs_pb, long len_attrs_pb);

        // Transform with pipeline middle and tail in order
        std::vector<float> middle_to_tail(const char *feats_pb, long len_feats_pb);

        // Transform with pipeline head, middle and tail in order
        std::vector<float> head_to_tail(const char *attrs_pb, long len_attrs_pb);

        // Save the map of names to encodes
        bool save_name2enc(const string &fname);

    };

}

#endif //ALGORITHM_COMPONENTS_PIPELINE_H
