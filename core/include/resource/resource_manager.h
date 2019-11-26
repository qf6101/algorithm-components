//
// Created by qfeng on 2018-12-27.
//

#ifndef ALGOCOMP_RESOURCE_MANAGER_H
#define ALGOCOMP_RESOURCE_MANAGER_H

#include <string>
#include <memory>
#include <map>
#include "ctpl.h"
#include "log/algocomp_log.h"
#include "feature/feature_operator.h"
#include "common/type_def.h"

using namespace std;

namespace algocomp {

    // Declare referenced classes
    class AttrTranser;

    class FeatTranser;

    class PLCompPool;

    class FeatureEncoder;

    class ModelDaemon;

    class CalibrationDaemon;

    /**
     * Resource manager
     *
     * Define resources (e.g., thread pool, feature operators, feature encoders, models, etc.) for repeated reuses
     */
    class ResourceManager {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;
        // Thread pool
        shared_ptr<ctpl::thread_pool> _thread_pool;
        // Feature operators
        map<string, shared_ptr<FeatureOperator>> _feat_ops;
        // Attribute transformers
        map<string, shared_ptr<AttrTranser>> _attr_transers;
        // Feature transformers
        map<string, shared_ptr<FeatTranser>> _feat_transers;
        // Feature encoders
        map<string, shared_ptr<FeatureEncoder>> _feat_encoders;
        // Models
        map<string, shared_ptr<ModelDaemon>> _models;
        // Calibrations
        map<string, shared_ptr<CalibrationDaemon>> _calibrations;
        // Signal to outer
        bool _started = false;

        // Create basic feature operators
        bool create_base_feat_ops();

    public:
        // Declare friend classes
        friend AttrTranser;
        friend FeatTranser;
        friend PLCompPool;

        // Constructor with initializations
        ResourceManager(const string &conf_fname, const string &conf_dname, ThreadType threadType = ThreadType::NO);

        // Destructor
        ~ResourceManager();

        // Signal to outer
        bool Started() { return _started; }

        // Get attribute transformer by name
        const shared_ptr<AttrTranser> get_attr_transer(const string &name);

        // Get feature transformer by name
        const shared_ptr<FeatTranser> get_feat_transer(const string &name);

        // Get feature encoder by name
        const shared_ptr<FeatureEncoder> get_feat_encoder(const string &name);

        // Get model by name
        const shared_ptr<ModelDaemon> get_model(const string &name);

        // Get calibration by name
        const shared_ptr<CalibrationDaemon> get_calibration(const string &name);
    };

}


#endif //ALGOCOMP_RESOURCE_MANAGER_H
