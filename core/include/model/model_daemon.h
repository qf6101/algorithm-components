//
// Created by qfeng on 17-5-27.
//

#ifndef ALGOCOMP_LINEAR_MODE_DAEMON_H
#define ALGOCOMP_LINEAR_MODE_DAEMON_H

#include "model/model.h"
#include <string>
#include <boost/thread/thread.hpp>
#include <Eigen/Sparse>
#include <spdlog/logger.h>

using namespace std;

namespace algocomp {

    /**
     *  Model daemon
     *
     *  Define the context of model (e.g., model update thread)
     */
    class ModelDaemon {
    public:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Model path and type
        const string _model_path;
        const string _model_type;

        // Model
        unique_ptr<Model> _model;

        // Daemon thread for updating model
        unique_ptr<boost::thread> _daemon_thread;

        // Update interval
        const int _update_interval;

        // Model success file
        string _model_succ_file_path;

        // Last modified time of the model file
        std::time_t _last_mod_time;

        // Signal to outer
        bool _started = false;

        // Constructor
        ModelDaemon(const string model_path, const string model_type, const int update_interval);

        // Destructor
        virtual ~ModelDaemon();

        // Start the daemon thread
        bool start();

        // Stop the daemon thread
        void stop();

        // Update model
        void update_model();

        // Check if the model file is modified
        bool is_model_file_modified();

        // Load the model from model file
        unique_ptr<Model> load_model();

        // Score the encoded feature data
        float score(const Eigen::SparseVector<float> &sample);
    };

}

#endif //ALGOCOMP_LINEAR_MODE_DAEMON_H
