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

#ifndef FESUTILS_PROGRESS_PRINTER_NODES_HPP
#define FESUTILS_PROGRESS_PRINTER_NODES_HPP

#include "progress_printer.hpp"


namespace fesutils {



    class PerLine_TodoToInflight_node {
    public:
        explicit PerLine_TodoToInflight_node(ProgressPrinter& progress_printer_) :
                progress_printer(progress_printer_){

        };

        inline std::shared_ptr<std::vector<std::string>>  operator()(std::shared_ptr<std::vector<std::string>> workPacket) {
            unsigned int num_line = workPacket->size();
            this->progress_printer.add_to_inflight_count(num_line);
            this->progress_printer.substract_from_todo_count(num_line);
            return workPacket;
        }

    private:
        ProgressPrinter& progress_printer;
    };

    class PerContMsg_InflightToDone_node {
    public:
        explicit PerContMsg_InflightToDone_node(ProgressPrinter& progress_printer_) :
                progress_printer(progress_printer_){

        };

        inline tbb::flow::continue_msg  operator()(tbb::flow::continue_msg workPacket) {
            this->progress_printer.increment_done_count();
            this->progress_printer.decrement_inflight_count();
            return workPacket;
        }

    private:
        ProgressPrinter& progress_printer;
    };

    class PerNumItem_InflightToDone_node {
    public:
        explicit PerNumItem_InflightToDone_node(ProgressPrinter& progress_printer_) :
                progress_printer(progress_printer_){

        };

        inline tbb::flow::continue_msg  operator()(unsigned int num_processed_item) {
            this->progress_printer.add_to_done_count(num_processed_item);
            this->progress_printer.substract_from_inflight_count(num_processed_item);
            return tbb::flow::continue_msg();
        }

    private:
        ProgressPrinter& progress_printer;
    };



}


#endif //FESUTILS_PROGRESS_PRINTER_NODES_HPP
