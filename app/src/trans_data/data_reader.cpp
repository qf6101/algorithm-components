//
// Created by qfeng on 2019-01-24.
//

#include "trans_data/data_reader.h"
#include <iostream>

namespace algocomp {

    bool DataReader::read_from_local(string &msg) {
        std::getline(cin, msg);

        if (!cin.good()) return false;
        else return true;
    }

}