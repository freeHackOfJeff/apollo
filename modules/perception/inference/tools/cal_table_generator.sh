#!/usr/bin/env bash

###############################################################################
# Copyright 2017 The Apollo Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###############################################################################

BASEDIR=$(dirname $0)
DATADIR=$BASEDIR/../../../perception-camera/camera/production/data
YOLODIR=$DATADIR/perception/camera/models/yolo_obstacle_detector

model_root=$YOLODIR/3d-r2
model_root=$YOLODIR/3d-r4-half
batch_root=$model_root/batches/
cal_table_root=$model_root/
names_file=$model_root/output_blobs.txt

rm -fr $batch_root $cal_table_root/CalibrationTable
if [ -d $batch_root ]; then
    echo "batch_root already exists: $batch_root"
    exit 0
fi
mkdir -p $batch_root
./cal_table_generator \
    -gen_batch \
    -image_root ~/data/ppic/images/ \
    -test_list ~/data/ppic/images.txt \
    -batch_root $batch_root \
    -cal_table_root $cal_table_root \
    -names_file $names_file \
    -weight_file $model_root/deploy.md \
    -proto_file $model_root/deploy.pt
