//
// Created by qfeng on 17-6-6.
//

#ifndef ALGOCOMP_MODEL_H
#define ALGOCOMP_MODEL_H

#include <Eigen/Sparse>


namespace  algocomp {

    /**
     * Model
     *
     * Define abstract model class
     */
    class Model {
    public:
        // score the input encoded feature data
        virtual float score(const Eigen::SparseVector<float> &sample) = 0;

        // Destructor
        virtual ~Model()= default;

        // Get the size of weights (coefficients)
        virtual uint64_t weight_size() = 0;
    };

}

#endif //ALGOCOMP_MODEL_H
