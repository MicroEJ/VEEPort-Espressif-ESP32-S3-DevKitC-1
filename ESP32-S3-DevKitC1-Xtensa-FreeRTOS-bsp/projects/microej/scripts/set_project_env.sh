#!/bin/bash
#
# BASH
#
# Copyright 2022-2023 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.

# 'set_project_env.sh' implementation for Espressif IDF.

# 'set_project_env' is responsible for
# - checking the availability of required environment variables 
# - setting project local variables for 'build.sh' and 'run.sh' 

SCRIPT_PATH=$(dirname "$0")

LOCAL_ENV_SCRIPT="$SCRIPT_PATH/set_local_env.sh"
if [[ -e "$LOCAL_ENV_SCRIPT" ]]; then
    echo "Load $LOCAL_ENV_SCRIPT"
    . "$LOCAL_ENV_SCRIPT"
fi

export ESP_BUILD_DIR=$(realpath $SCRIPT_PATH/../build)
export ESP_PROJECT_DIR=$(realpath $SCRIPT_PATH/..)
export IDF_PATH=$(realpath "$SCRIPT_PATH"/../../../sdk/esp-idf)
export IDF_INSTALL_TARGETS=esp32-s3

echo "ESP_BUILD_DIR is $ESP_BUILD_DIR"
echo "ESP_PROJECT_DIR is $ESP_PROJECT_DIR"
echo "IDF_PATH is $IDF_PATH"
echo "IDF_INSTALL_TARGETS is $IDF_INSTALL_TARGETS"

# Set offsets for combined binary
BOOTLOADER_BIN_OFFSET=0x0
PARTITIONS_BIN_OFFSET=0x8000
OTADATA_BIN_OFFSET=0x20000
APPLICATION_BIN_OFFSET=0x30000
