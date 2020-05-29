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

#ifndef FESUTILS_RECONSTRUCT_FROM_FRAGMENTS_HPP
#define FESUTILS_RECONSTRUCT_FROM_FRAGMENTS_HPP

#include <string>
#include <optional>
#include <vector>

#include "../GeneralOptions.h"

namespace fesutils {

    struct reconstruct_from_fragments_args {
        std::vector<std::string> fragment_fes_paths;

        std::optional<std::string> fefield;
    };

    /**
     * Entry point for the reconstruct_from_fragments subcommand.
     *
     * \param options General options passed on the command line
     * \param args Specific arguments for the command, passed as a struct
     */
    int do_reconstruct_from_fragments(GeneralOptions& options,
                                const reconstruct_from_fragments_args& args);

}

#endif //FESUTILS_RECONSTRUCT_FROM_FRAGMENTS_HPP
