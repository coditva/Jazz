#!/bin/bash

DEFAULT_ARCH="ARCH_X86"

if [[ -z "$RELEASE" || "$RELEASE" == "false" ]]; then
    RELEASE_TYPE="debug"
else
    RELEASE_TYPE="release"
fi

if [[ -z "$ARCH" ]]; then
    ARCH="$DEFAULT_ARCH"
fi

if [[ "$ARCH" == "ARCH_X86" ]]; then
    ARCH_NAME="x86"
elif [[ "$ARCH" == "ARCH_X86_64" ]]; then
    ARCH_NAME="x86-84"
else
    ARCH_NAME="unknown"
fi

export RELEASE_FILE_NAME="Jazz-$ARCH_NAME-$RELEASE_TYPE.img"
mv build/disk.img $RELEASE_FILE_NAME

echo $RELEASE_FILE_NAME
