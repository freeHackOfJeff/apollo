/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/**
 * @file
 */

#ifndef MODEULES_L3_PERCEPTION_L3_PERCEPTION_UTIL_H_
#define MODEULES_L3_PERCEPTION_L3_PERCEPTION_UTIL_H_

#include <cmath>

#include "modules/common/proto/geometry.pb.h"
#include "modules/perception/proto/perception_obstacle.pb.h"

/**
 * @namespace apollo::l3_perception
 * @brief apollo::l3_perception
 */

namespace apollo {
namespace l3_perception {

using ::apollo::common::Quaternion;
using ::apollo::perception::Point;
using ::apollo::perception::PerceptionObstacle;

double GetAngleFromQuaternion(const Quaternion quaternion);

void FillPerceptionPolygon(PerceptionObstacle* const perception_obstacle,
                           const double mid_x, const double mid_y, const double mid_z,
                           const double length, const double width, const double height,
                           const double heading);

// TODO(lizh): change it to PerceptionObstacle::VEHICLE or so
//             when perception obstacle type is extended.
// object type | int
// car         | 0
// truck       | 1
// bike        | 2
// ped         | 3
// unknown     | 4
double GetDefaultObjectLength(const int object_type);

double GetDefaultObjectWidth(const int object_type);

}  // namespace l3_perception
}  // namespace apollo

#endif  // MODULES_L3_PERCEPTION_L3_PERCEPTION_UTIL_H_

