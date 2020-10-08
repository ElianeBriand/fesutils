
import os


def save_pretest_state():
    cwd = os.getcwd()

    return cwd


def move_to_root():
    os.chdir("..")


def restore_pretest_state(state):
    os.chdir(state)

def get_fesutils_binary_path(state):
    return state + "/fesutils"

if __name__ == "__main__":
    exit(0)
