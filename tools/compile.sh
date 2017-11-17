#!/bin/sh

VAGRANT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

(
    cd $VAGRANT_DIR && vagrant ssh -c "cd /pax-app && make clean && make all"
)

echo "Done!"
