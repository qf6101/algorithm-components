//
// Created by qfeng on 2018-12-27.
//

#include "pipeline/pl_head.h"
#include "attrpb/auc_impr_attrs.pb.h"
#include "base64.h"
#include <boost/thread/latch.hpp>
#include "ctpl.h"


namespace algocomp {

    PLHead::PLHead(const shared_ptr<AttrTranser> attr_transer, const vector<shared_ptr<FeatTranser>> feat_transers,
                   shared_ptr<ctpl::thread_pool> thp_rsrc, const ThreadType thread_type) : _attr_transer(
            attr_transer), _feat_transers(feat_transers), PLComp(thp_rsrc, thread_type) {
        _logger = spdlog::get(ACLog::_logger_name);

        bool success = true;

        if (!attr_transer) {
            _logger->error("Attribute transformer is empty for pl head.");
            success = false;
        }

        _logger->info("PLHead uses thread type {}.", _thread_type_str);

        _started = success;
    }

    const string PLHead::work(const char *attrs_pb, long len_attrs_pb) {
        string outp = "";
        google::protobuf::Arena arena;
        try {

            AUCImprAttrs *attrs = google::protobuf::Arena::CreateMessage<AUCImprAttrs>(&arena);
            attrs->ParseFromArray(attrs_pb, len_attrs_pb);

            // Transform attribute data to feature data
            auto feats_ = _attr_transer->apply(*attrs, arena);

            if (feats_.has_value()) {
                auto feats = feats_.value();
                for (auto &ft: _feat_transers) {
                    // Transform feature data cyclically
                    if (!ft->apply(feats, arena)) {
                        _logger->error("Transform feature data (inner used) fail in plhead (thread type: {}).",
                                       _thread_type_str);
                    }
                }
                string feats_pb;
                feats.SerializeToString(&feats_pb);
                string encoded;
                Base64::Encode(feats_pb, &encoded);
                outp = encoded;
            } else {
                _logger->error("Transform attribute data (inner used) fail in plhead (thread type: {}).",
                               _thread_type_str);
            }

        } catch (const exception &ex) {
            _logger->error("Transform data (separately used) fail in plhead: {}", ex.what());
        }

        if (arena.SpaceAllocated()) arena.Reset();
        return outp;
    }

    std::optional<AUCImprFeats>
    PLHead::work(const char *attrs_pb, long len_attrs_pb, google::protobuf::Arena &arena) {
        try {
            AUCImprAttrs *attrs = google::protobuf::Arena::CreateMessage<AUCImprAttrs>(&arena);
            attrs->ParseFromArray(attrs_pb, len_attrs_pb);

            // Transform attribute data to feature data
            auto feats_ = _attr_transer->apply(*attrs, arena);

            if (feats_.has_value()) {
                auto feats = feats_.value();
                for (auto &ft: _feat_transers) {
                    // Transform feature data cyclically
                    if (!ft->apply(feats, arena)) {
                        _logger->error("Transform feature data (inner used) fail in plhead (thread type: {}).",
                                       _thread_type_str);
                    }
                }

                return feats;
            } else {
                _logger->error("Transform attribute data (inner used) fail in plhead (thread type: {}).",
                               _thread_type_str);
            }
        } catch (const exception &ex) {
            _logger->error("Transform data fail (inner used) in plhead (thread type: {}): {}", _thread_type_str,
                           ex.what());
        }

        return std::nullopt;
    }
}