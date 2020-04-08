//
// Created by eliane on 08/04/2020.
//

#ifndef FESUTILS_LIMITER_DECREMENTER_NODE_HPP
#define FESUTILS_LIMITER_DECREMENTER_NODE_HPP

#include <tbb/flow_graph.h>

namespace fesutils {

    template<typename MessageType>
    class limiter_decrementer_node {
    public:

        limiter_decrementer_node() = default;

        tbb::flow::continue_msg operator()(MessageType message) {
            return tbb::flow::continue_msg();
        }

    };

}




#endif //FESUTILS_LIMITER_DECREMENTER_NODE_HPP
