//
// Created by qfeng on 2018-12-27.
//

#ifndef ALGOCOMP_PL_HEAD_H
#define ALGOCOMP_PL_HEAD_H

#include "pl_comp.h"
#include "attribute/attr_transer.h"
#include "feature/feat_transer.h"
#include "log/algocomp_log.h"
#include <common/type_def.h>


// Declare referenced class
namespace ctpl {
    class thread_pool;
}

namespace algocomp {

    // Declare referenced classes
    class AttrTranser;

    class FeatTranser;

    class AUCImprAttrs;

    class AUCImprFeats;

    /**
     * Pipeline Head
     *
     * Define context of pipeline head (e.g., attribute transformer and feature transformers)
     */
    class PLHead : public PLComp {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Attribute transformer
        const shared_ptr<AttrTranser> _attr_transer;

        // A set of feature transformers
        const vector<shared_ptr<FeatTranser>> _feat_transers;
    public:
        // Constructor
        PLHead(const shared_ptr<AttrTranser> attr_transer, const vector<shared_ptr<FeatTranser>> feat_transers,
               shared_ptr<ctpl::thread_pool> thp_rsrc, const ThreadType thread_type = ThreadType::NO);

        // Separately used to transform input attribute data to output feature data (base64 encoded protobuf string)
        const std::string work(const char *attrs_pb, long len_attrs_pb);

        std::optional<AUCImprFeats>
        work(const char *attrs_pb, long len_attrs_pb, google::protobuf::Arena &arena);
    };

}

#endif //ALGOCOMP_PL_HEAD_H
