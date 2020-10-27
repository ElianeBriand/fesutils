/*
 * Copyright (c) 2020 Eliane Briand
 *
 * This file is part of fesutils.
 *
 * fesutils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * fesutils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fesutils.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef FESUTILS_DUMMY_SINK_NODE_HPP
#define FESUTILS_DUMMY_SINK_NODE_HPP



namespace fesutils {

    /**
     * A TBB graph nodes that does nothing. Useful when a node is expected but a NOP is desired.
     * \tparam T Passthrough data type
     */
    template<typename T>
    class dummy_sink_node {
    public:

        dummy_sink_node() : count(0) {};

        bool operator()(const T& lines);

    protected:
        unsigned int count;
    };


    template<typename T>
    bool dummy_sink_node<T>::operator()(const T& lines) {
        this->count++;
        return true;
    }
}
#endif //FESUTILS_DUMMY_SINK_NODE_HPP
