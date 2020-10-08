from __future__ import absolute_import

import subprocess
import os

from st_utils import save_pretest_state, get_fesutils_binary_path, move_to_root, restore_pretest_state, restore_pretest_state


state = save_pretest_state()
fesutil_bin_path = get_fesutils_binary_path(state)
move_to_root()


process = subprocess.Popen([fesutil_bin_path, 'examples/systemtest_load_fes_data.yaml'],
                           stdout=subprocess.PIPE,
                           stderr=subprocess.PIPE)

process.wait()

if process.returncode != 0:
    restore_pretest_state(state)
    exit(1)


# Else success
restore_pretest_state(state)
exit(0)
