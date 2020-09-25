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
#include <locale>

#include <fmt/format.h>
#include <fmt/locale.h>

#include "progress_printer.hpp"
#include "../frontend_common.h"

namespace fesutils {
    ProgressPrinter::ProgressPrinter()
    {
        this->space_prefix = non_boost_log_get_space_prefix();
        this->todo_count.store(0,std::memory_order_release);
        this->inflight_count.store(0,std::memory_order_release);
        this->done_count.store(0,std::memory_order_release);
        this->printer_thread_continue.store(true,std::memory_order_release);
        this->printer_thread = std::thread(&ProgressPrinter::actual_printer_thread_func, this);
    }

    void ProgressPrinter::actual_printer_thread_func() {

        long int last_todo_count = this->todo_count.load(std::memory_order_acquire);
        long int last_inflight_count = this->inflight_count.load(std::memory_order_acquire);
        long int last_done_count = this->done_count.load(std::memory_order_acquire);
        bool shouldPrint = true;

        while(true) {
            if(! this->printer_thread_continue.load(std::memory_order_acquire)) {
                break;
            }

            long int local_todo_count = this->todo_count.load(std::memory_order_acquire);
            long int local_inflight_count = this->inflight_count.load(std::memory_order_acquire);
            long int local_done_count = this->done_count.load(std::memory_order_acquire);
            if(last_todo_count != local_todo_count){
                last_todo_count = local_todo_count;
                shouldPrint = true;
            }
            if(last_inflight_count != local_inflight_count){
                last_inflight_count = local_inflight_count;
                shouldPrint = true;
            }
            if(last_done_count != local_done_count){
                last_done_count = local_done_count;
                shouldPrint = true;
            }

            if(shouldPrint) {
                std::string progress_line = fmt::format(std::locale("en_US.UTF-8"),
                                                       "{:s}Progress | todo: {:L} | inflight: {:L} | done: {:L}      ",
                                                        this->space_prefix, local_todo_count, local_inflight_count, local_done_count );

                std::cout << "\r" << progress_line;
                shouldPrint = false;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

        }
    }

    void ProgressPrinter::finish() {
        this->printer_thread_continue.store(false,std::memory_order_release);
        this->printer_thread.join();

        long int local_todo_count = this->todo_count.load(std::memory_order_acquire);
        long int local_inflight_count = this->inflight_count.load(std::memory_order_acquire);
        long int local_done_count = this->done_count.load(std::memory_order_acquire);
        std::string progress_line = fmt::format(std::locale("en_US.UTF-8"),
                                                "{:s}Progress | todo: {:L} | inflight: {:L} | done: {:L} | Finished      ",
                                                this->space_prefix, local_todo_count, local_inflight_count, local_done_count );
        std::cout << "\r" << progress_line;
        std::cout << std::endl;

    }

    ProgressPrinter::~ProgressPrinter() {
        bool continue_status = this->printer_thread_continue.load(std::memory_order_acquire);
        if(continue_status) {
            // LCOV_EXCL_START
            // Reason for coverage exclusion: If finish() is not called, behaviour is possibly non-deterministic so difficult to test
            std::cout << "WARNING: Progress_printer destructor was called, but finish() was not called. May indicate use error." << std::endl;
            // LCOV_EXCL_STOP
        }
    }


}

