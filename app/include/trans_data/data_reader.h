//
// Created by qfeng on 2019-01-24.
//

#ifndef ALGORITHM_COMPONENTS_DATA_LOADER_H
#define ALGORITHM_COMPONENTS_DATA_LOADER_H

#include <string>

using namespace std;

namespace algocomp {

    class DataReader {
    public:
        static bool read_from_local(string &msg);
    };

}

#endif //ALGORITHM_COMPONENTS_DATA_LOADER_H
