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

#include <iostream>
#include <chrono>

#include "progress_printer.hpp"

namespace fesutils {
    Progress_printer::Progress_printer() :
            work_packet_total_count(0),
            work_packet_done_count(0),
            printer_thread_continue(true),
            printer_thread(&Progress_printer::actual_printer_thread_func, this)
    {
        this->work_packet_total_count.store(0,std::memory_order_release);
        this->work_packet_done_count.store(0,std::memory_order_release);
        this->printer_thread_continue.store(true,std::memory_order_release);
    }

    void Progress_printer::actual_printer_thread_func() {
        long int last_total_wp = this->work_packet_total_count.load(std::memory_order_acquire);
        long int last_done_wp = this->work_packet_done_count.load(std::memory_order_acquire);
        bool shouldPrint = true;
        while(true) {
            if(! this->printer_thread_continue.load(std::memory_order_acquire)) {
                break;
            }
            long int total_wp = this->work_packet_total_count.load(std::memory_order_acquire);
            long int done_wp = this->work_packet_done_count.load(std::memory_order_acquire);
            if(last_total_wp != total_wp){
                last_total_wp = total_wp;
                shouldPrint = true;
            }
            if(last_done_wp != done_wp){
                last_done_wp = done_wp;
                shouldPrint = true;
            }
            if(shouldPrint) {
                std::cout << "\rProgress | done: " << done_wp << " | total: " << total_wp;
                shouldPrint = false;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    void Progress_printer::finish() {
        this->printer_thread_continue.store(false,std::memory_order_release);
        this->printer_thread.join();
        std::cout << std::endl;
        std::cout << "Progress | Finished | Work packet total: " << this->work_packet_total_count.load(std::memory_order_acquire) << " | ";
        std::cout << "done: " << this->work_packet_done_count.load(std::memory_order_acquire) << std::endl;
        std::cout << std::endl;
    }

    Progress_printer::~Progress_printer() {
        bool continue_status = this->printer_thread_continue.load(std::memory_order_acquire);
        if(continue_status) {
            // LCOV_EXCL_START
            // Reason for coverage exclusion: If finish() is not called, behaviour is possibly non-deterministic so difficult to test
            std::cout << "WARNING: Progress_printer destructor was called, but finish() was not called. May indicate use error." << std::endl;
            // LCOV_EXCL_STOP
        }
    }


}

