#!/bin/sh

VAGRANT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

(
    cd $VAGRANT_DIR &&
    vagrant ssh -c "/vagrant/xcb kill-server && /vagrant/xcb connect com:/dev/ttyPos0 && /vagrant/xcb installer aip /pax-app/pkg/build.aip"
)

echo "Done!"
