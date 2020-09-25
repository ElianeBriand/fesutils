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
    class ProgressPrinter {
    public:
        ProgressPrinter();
        ~ProgressPrinter();

        inline void increment_inflight_count() {
            this->inflight_count.fetch_add(1, std::memory_order_acq_rel);
        };

        inline void decrement_inflight_count() {
            this->inflight_count.fetch_sub(1, std::memory_order_acq_rel);
        };

        inline void substract_from_inflight_count(unsigned int i) {
            this->inflight_count.fetch_sub(i, std::memory_order_acq_rel);
        };

        inline void add_to_inflight_count(unsigned int i) {
            this->inflight_count.fetch_add(i, std::memory_order_acq_rel);
        };

        inline void set_todo_count(unsigned int i) {
            this->todo_count.store(i, std::memory_order_acq_rel);
        };
        inline void substract_from_todo_count(unsigned int i) {
            this->todo_count.fetch_sub(i, std::memory_order_acq_rel);
        };

        inline void add_to_todo_count(unsigned int i) {
            this->todo_count.fetch_add(i, std::memory_order_acq_rel);
        };

        inline void increment_done_count() {
            this->done_count.fetch_add(1, std::memory_order_acq_rel);
        };

        inline void substract_from_done_count(unsigned int i) {
            this->done_count.fetch_sub(i, std::memory_order_acq_rel);
        };

        inline void add_to_done_count(unsigned int i) {
            this->done_count.fetch_add(i, std::memory_order_acq_rel);
        };


        inline long int get_todo_count() {
            long int val = this->todo_count.load(std::memory_order_acquire);
            return val;
        }
        inline long int get_inflight_count() {
            long int val = this->inflight_count.load(std::memory_order_acquire);
            return val;
        }
        inline long int get_done_count() {
            long int val = this->done_count.load(std::memory_order_acquire);
            return val;
        }


        void finish();

    private:
        std::thread printer_thread;
        std::string space_prefix;

        void actual_printer_thread_func();

        std::atomic<bool> printer_thread_continue;

        std::atomic<long int> todo_count;
        std::atomic<long int> inflight_count;
        std::atomic<long int> done_count;
    };
}



#endif //FESUTILS_PROGRESS_PRINTER_HPP
