#!/usr/bin/env bash

# get script dir
script_dir=$( cd `dirname ${BASH_SOURCE[0]}` >/dev/null 2>&1 ; pwd -P )

rm -rf docs/xml docs/_build

# build docs target
cmake --preset Debug
cmake --build --preset Debug --target docs

# activate virtual environment
source .venv/bin/activate

# build as html and dirhtml
pushd docs && make html && make dirhtml && popd


# open in browser
open docs/_build/html/index.html