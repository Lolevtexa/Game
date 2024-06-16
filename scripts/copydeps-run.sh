#!/bin/bash

# Taken from https://github.com/suve/copydeps/tree/trunk
COPYDEPS=$1
TARGET=$2
TARGET_DIR=$3
SEARCH_DIR=$4

# Go through all directories recursively and create
# a search list for COPYDEPS
# Yes, this is a bit of a hack, but it works
SEARCH_LIST=""
for dir in $(find ${SEARCH_DIR} -type d); do
    SEARCH_LIST="${SEARCH_LIST} --search-dir ${dir}"
done

${COPYDEPS} ${TARGET} --no-clobber ${SEARCH_LIST} ${TARGET_DIR} \
    --ignore OPENGL32.dll
