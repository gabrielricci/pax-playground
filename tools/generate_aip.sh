#!/bin/sh

TOOLS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="$( dirname $TOOLS_DIR )"
WORK_DIR=`mktemp -d`

echo "Tools dir: $TOOLS_DIR"
echo "Root dir: $ROOT_DIR"
echo "Work dir: $WORK_DIR"

if [[ ! "$WORK_DIR" || ! -d "$WORK_DIR" ]]; then
  echo "Could not create temp dir"
  exit 1
fi

function cleanup {
  rm -rf "$WORK_DIR"
  echo "Deleted temp working directory $WORK_DIR"
}

trap cleanup EXIT

cp $ROOT_DIR/app/appinfo $WORK_DIR
mkdir $WORK_DIR/bin
cp $ROOT_DIR/app/App $WORK_DIR
mkdir $WORK_DIR/lib
cp -rf $ROOT_DIR/app/res $WORK_DIR

( cd $WORK_DIR && zip -r $ROOT_DIR/app/pkg/build.aip . )

echo "Done!"
