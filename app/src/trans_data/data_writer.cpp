//
// Created by qfeng on 2019-01-24.
//

#include "trans_data/data_writer.h"
#include <iostream>

namespace algocomp {

    bool DataWriter::write_to_local(const string &msg) {
        cout << msg << endl;

        if (!cout.good()) return false;
        else return true;
    }

}