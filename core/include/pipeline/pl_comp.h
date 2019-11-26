//
// Created by qfeng on 2018-12-27.
//

#ifndef ALGOCOMP_PL_COMP_H
#define ALGOCOMP_PL_COMP_H

#include <string>
#include <memory>
#include <common/type_def.h>

using namespace std;

// Declare referenced class
namespace ctpl {
    class thread_pool;
}

namespace algocomp {

    /**
     * Pipeline component
     *
     * Define context of pipeline component (e.g., thread pool and thread type). This is merely an abstract class.
     * The concrete classes are PLHead, PLMiddle, PLTail and Pipeline.
     */
    class PLComp {
    protected:
        // Thread pool
        shared_ptr<ctpl::thread_pool> _thp_rsrc;

        // Thread type
        const ThreadType _thread_type;
        string _thread_type_str;

        // Signal to outer
        bool _started = false;
    public:
        // Constructor with initialization
        PLComp(shared_ptr<ctpl::thread_pool> thp_rsrc, const ThreadType thread_type = ThreadType::NO);

        // Get thread type
        const ThreadType thread_type() { return _thread_type; }

        // Signal to outer
        bool Started() { return _started; }
    };

}


#endif //ALGOCOMP_PL_COMP_H
