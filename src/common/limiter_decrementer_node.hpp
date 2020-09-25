//
// Created by eliane on 08/04/2020.
//

#ifndef FESUTILS_LIMITER_DECREMENTER_NODE_HPP
#define FESUTILS_LIMITER_DECREMENTER_NODE_HPP

#include <tbb/flow_graph.h>

namespace fesutils {

    // LCOV_EXCL_START
    // Reason for coverage exclusion: gcov/lcov does not process template class properly

    template<typename MessageType>
    class limiter_decrementer_node {
    public:

        limiter_decrementer_node() {

        };

        tbb::flow::continue_msg operator()(MessageType message) {
            return tbb::flow::continue_msg();
        }

    };

    // LCOV_EXCL_STOP

}




#endif //FESUTILS_LIMITER_DECREMENTER_NODE_HPP
