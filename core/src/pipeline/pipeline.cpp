//
// Created by qfeng on 2018-12-30.
//

#include "pipeline/pipeline.h"
#include <boost/thread/latch.hpp>

namespace algocomp {
    const std::vector<std::string> Pipeline::head_to_middle(const char *attrs_pb, long len_attrs_pb) {
        std::vector<std::string> outp;
        google::protobuf::Arena arena;

        try {
            auto feats_ = _head->work(attrs_pb, len_attrs_pb, arena);

            if (feats_.has_value()) {
                auto feats = feats_.value();
                outp = _middle->work_toenc(feats, arena);
            } else {
                _logger->error("Transform attribute data fail in head->work.");
            }

        } catch (const exception &ex) {
            _logger->error("Transform data fail in head_to_middle fail: {}", ex.what());
        }

        if (arena.SpaceAllocated()) arena.Reset();
        return outp;
    }

    std::vector<float> Pipeline::middle_to_tail(const char *feats_pb, long len_feats_pb) {
        vector<float> outp;
        google::protobuf::Arena arena;

        try {
            AUCImprFeats *feats = google::protobuf::Arena::CreateMessage<AUCImprFeats>(&arena);
            feats->ParseFromArray(feats_pb, len_feats_pb);
            auto encoded = _middle->work_tokv(*feats, arena);

            if (encoded.has_value()) {
                outp = _tail->work(encoded.value());
            } else {
                _logger->error("Transform feature data fail in middle_to_tail.");
            }
        } catch (const exception &ex) {
            _logger->error("Transform data fail in middle_to_tail fail: {}", ex.what());
        }

        if (arena.SpaceAllocated()) arena.Reset();
        return outp;

    }

    std::vector<float> Pipeline::head_to_tail(const char *attrs_pb, long len_attrs_pb) {
        vector<float> outp;
        google::protobuf::Arena arena;

        try {
            auto feats_ = _head->work(attrs_pb, len_attrs_pb, arena);
            if (feats_.has_value()) {
                auto feats = feats_.value();
                auto encoded = _middle->work_tokv(feats, arena);

                if (encoded.has_value()) {
                    outp = _tail->work(encoded.value());
                } else {
                    _logger->error("Transform feature data fail in head_to_tail.");
                }
            } else {
                _logger->error("Transform attribute data fail in head_to_tail.");
            }

        } catch (const exception &ex) {
            _logger->error("Transform data fail in head_to_tail: {}.", ex.what());
        }

        if (arena.SpaceAllocated()) arena.Reset();
        return outp;
    }

    bool Pipeline::save_name2enc(const string &fname) {
        if (_middle) {
            _middle->save_name2enc(fname);
        } else {
            _logger->error("Save name2enc in with empty pipeline middle.");
        }
    }
}
