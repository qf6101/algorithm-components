#ifndef XGBOOST_FVEC_H_
#define XGBOOST_FVEC_H_

#include <Eigen/Sparse>

/*!
 * \brief dense feature vector that can be taken by RegTree
 * and can be construct from sparse feature vector.
 */

typedef float bst_float;

namespace xgboost {
    class FVecBase {
    public:
        /*!
         * \brief returns the size of the feature vector
         * \return the size of the feature vector
         */
        virtual size_t size() const;

        /*!
         * \brief get ith value
         * \param i feature index.
         * \return the i-th feature value
         */
        virtual bst_float fvalue(size_t i) const;

        /*!
         * \brief check whether i-th entry is missing
         * \param i feature index.
         * \return whether i-th value is missing.
         */
        virtual bool is_missing(size_t i) const;
    };

    class FVec {
    public:
        /*!
         * \brief initialize the vector with size vector
         * \param size The size of the feature vector.
         */
        FVec(const Eigen::SparseVector<bst_float> &features) {
            Entry e;
            e.flag = -1;
            data.resize(features.size());
            std::fill(data.begin(), data.end(), e);

            for (Eigen::SparseVector<bst_float >::InnerIterator it(features); it; ++it) {
                data[it.index()].fvalue = it.value();
            }
        }

        /*!
         * \brief returns the size of the feature vector
         * \return the size of the feature vector
         */
        size_t size() {
            return data.size();
        }

        /*!
         * \brief get ith value
         * \param i feature index.
         * \return the i-th feature value
         */
        bst_float fvalue(size_t i) const {
            return data[i].fvalue;
        };

        /*!
         * \brief check whether i-th entry is missing
         * \param i feature index.
         * \return whether i-th value is missing.
         */
        bool is_missing(size_t i) const {
            return data[i].flag == -1;
        };

    private:
        /*!
         * \brief a union value of value and flag
         *  when flag == -1, this indicate the value is missing
         */
        union Entry {
            bst_float fvalue;
            int flag;
        };
        std::vector<Entry> data;
    };

    class FHash : FVecBase {
    public:

        FHash(const Eigen::SparseVector<bst_float> &features) : data(features) {}

        bst_float fvalue(size_t i) const {
            return data.data().at(i, 0);
        }

        size_t size() const {
            return data.size();
        }


        bool is_missing(size_t i) const {
            auto searched = data.data().at(i, -1);
            if(searched >= 0) return true; else return false;
        }

    private:
        const Eigen::SparseVector<bst_float> &data;
    };
}

#endif //XGBOOST_FVEC_H_
