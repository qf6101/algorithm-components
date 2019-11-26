//
// Created by qfeng on 2019-01-21.
//

#ifndef ALGORITHM_COMPONENTS_TYPE_DEF_H
#define ALGORITHM_COMPONENTS_TYPE_DEF_H

#include <string>

using namespace std;

namespace algocomp {

    // Define thread type
    enum class ThreadType {
        NO = 0,
        ON_ADS = 1, // Multi-threads work on ads (for many ads)
        ON_EXAMPLE = 2 // Multi-threads work on example (for few ads)
    };

    // Interpret thread type to string
    const string thread_type_str(const ThreadType &tt);

}

#endif //ALGORITHM_COMPONENTS_TYPE_DEF_H
