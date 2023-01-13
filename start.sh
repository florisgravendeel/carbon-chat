#!/bin/bash
echo "Starting server"
cd "include/carbon-lang"
bazel run //explorer -- ../../main.carbon