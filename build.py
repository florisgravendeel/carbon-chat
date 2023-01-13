#
# This python script builds the json-cpp library into a single source and header file.
# There is no need to manually run this file, as CMake will automatically run this file upon building.
#
import os
from typing import AnyStr
import include.json.amalgamate


def change_directory(path: AnyStr):
    abspath = os.path.abspath(path)  # Get absolute path
    new_directory = os.path.dirname(abspath)
    os.chdir(new_directory)  # Change directory


if __name__ == "__main__":
    change_directory(include.json.amalgamate.__file__)  # Change to script directory
    include.json.amalgamate.main()
