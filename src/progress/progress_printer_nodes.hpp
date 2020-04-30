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

    template<typename workPacketType>
    class progress_work_packet_registerer {
    public:
        explicit progress_work_packet_registerer(Progress_printer& progress_printer_) :
                progress_printer(progress_printer_){

        };

        inline workPacketType  operator()(workPacketType workPacket) {
            this->progress_printer.increment_workpacket_total_count();
            return workPacket;
        }

    private:
        Progress_printer& progress_printer;
    };

    template<typename workPacketType>
    class progress_work_packet_done_reporter {
    public:
        explicit progress_work_packet_done_reporter(Progress_printer& progress_printer_) :
                progress_printer(progress_printer_){

        };

        inline workPacketType  operator()(workPacketType workPacket) {
            this->progress_printer.increment_workpacket_done_count();
            return workPacket;
        }

    private:
        Progress_printer& progress_printer;
    };

}


#endif //FESUTILS_PROGRESS_PRINTER_NODES_HPP
