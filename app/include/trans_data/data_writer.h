//
// Created by qfeng on 2019-01-24.
//

#ifndef ALGORITHM_COMPONENTS_DATA_WRITER_H
#define ALGORITHM_COMPONENTS_DATA_WRITER_H

#include <string>

using namespace std;

namespace algocomp {

    class DataWriter {
    public:
        static bool write_to_local(const string &msg);
    };

}


#endif //ALGORITHM_COMPONENTS_DATA_WRITER_H
