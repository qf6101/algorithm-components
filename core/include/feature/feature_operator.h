//
// Created by qfeng on 2018-12-26.
//

#ifndef ALGOCOMP_FEATURE_OPERATOR_H
#define ALGOCOMP_FEATURE_OPERATOR_H

#include <string>
#include "featpb/disc_feat_data.pb.h"
#include "featpb/cont_feat_data.pb.h"

using namespace std;

namespace algocomp {

    /**
     * Feature Operator
     *
     * Define transform from feature to feature. This is merely an abstract class.
     */
    class FeatureOperator {
    public:
        virtual ~FeatureOperator() = default;
    };


}


#endif //ALGOCOMP_FEATURE_OPERATOR_H
