#!/bin/bash

DEFAULT_ARCH="ARCH_X86"

if [[ -z "$RELEASE" || "$RELEASE" == "true" ]]; then
    RELEASE_TYPE="debug"
else
    RELEASE_TYPE="release"
fi

if [[ -z "$ARCH" ]]; then
    ARCH=$DEFAULT_ARCH
fi

export RELEASE_FILE_NAME="Jazz_$ARCH_$RELEASE_TYPE.img"
mv build/disk.img $RELEASE_FILE_NAME
