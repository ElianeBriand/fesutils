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


#ifndef FESUTILS_PROGRESS_PRINTER_HPP
#define FESUTILS_PROGRESS_PRINTER_HPP

#include <thread>
#include <atomic>

namespace fesutils {
    class Progress_printer {
    public:
        Progress_printer();
        ~Progress_printer();

        inline void increment_workpacket_total_count() {
            this->work_packet_total_count.fetch_add(1, std::memory_order_acq_rel);
        };

        inline void increment_workpacket_done_count() {
            this->work_packet_done_count.fetch_add(1, std::memory_order_acq_rel);
        };

        inline long int get_work_packet_done_count() {
            long int val = this->work_packet_done_count.load(std::memory_order_acquire);
            return val;
        }

        inline long int get_work_packet_total_count() {
            long int val = this->work_packet_total_count.load(std::memory_order_acquire);
            return val;
        }

        void finish();

    private:
        std::thread printer_thread;

        void actual_printer_thread_func();

        std::atomic<bool> printer_thread_continue;
        std::atomic<long int> work_packet_done_count;
        std::atomic<long int> work_packet_total_count;
    };
}



#endif //FESUTILS_PROGRESS_PRINTER_HPP
