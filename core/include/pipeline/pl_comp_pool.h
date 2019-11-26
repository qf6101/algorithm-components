//
// Created by qfeng on 2018-12-29.
//

#ifndef ALGORITHM_COMPONENTS_PL_COMP_POOL_H
#define ALGORITHM_COMPONENTS_PL_COMP_POOL_H

#include <memory>
#include <map>
#include <vector>
#include "pl_head.h"
#include "pl_middle.h"
#include "pl_tail.h"
#include "pipeline.h"
#include "resource/resource_manager.h"
#include "spdlog/logger.h"
#include "calibration/calibration_daemon.h"

using namespace std;

namespace algocomp {

    class PLCompPool {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Resource manager
        unique_ptr<ResourceManager> _rsrc_mgr;

        // pipeline components
        std::map<std::string, shared_ptr<PLHead>> _pl_heads;
        std::map<std::string, shared_ptr<PLMiddle>> _pl_middles;
        std::map<std::string, shared_ptr<PLTail>> _pl_tails;
        std::map<std::string, shared_ptr<Pipeline>> _pls;
        std::map<std::string, shared_ptr<CalibrationDaemon>> _pl_calibrations;

        // Get pipeline calibration by name
        shared_ptr<CalibrationDaemon> get_calibration(const string &name);


    public:
        // Constructor
        PLCompPool(const string &conf_fname);

        // Get pipeline head by name
        PLHead *get_plhead(const string &name);

        // Get pipeline middle by name
        PLMiddle *get_plmiddle(const string &name);

        // Get pipeline tail by name
        PLTail *get_pltail(const string &name);

        // Get pipeline by name
        Pipeline *get_pipeline(const string &name);

        vector<float> calibrate(const vector<string> &pids, const vector<float> &scores);

        float calibrate(const string &pid, float score);
    };

}


#endif //ALGORITHM_COMPONENTS_PL_COMP_POOL_H
