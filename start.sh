#!/bin/bash
echo "Starting server"
cd "carbon-lang"
bazel run //explorer -- ../main.carbon