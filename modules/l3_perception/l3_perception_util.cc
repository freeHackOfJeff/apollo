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

#include <cmath>

#include "modules/l3_perception/l3_perception_util.h"
#include "modules/l3_perception/l3_perception_gflags.h"

/**
 * @namespace apollo::l3_perception
 * @brief apollo::l3_perception
 */

namespace apollo {
namespace l3_perception {

double GetAngleFromQuaternion(const Quaternion quaternion) {
  double theta =
    std::atan2(2.0 * quaternion.qw() * quaternion.qz() +
                   quaternion.qx() * quaternion.qy(),
               1.0 - 2.0 * (quaternion.qy() * quaternion.qy() +
                   quaternion.qz() * quaternion.qz())) +
                   std::acos(-1.0) / 2.0;
  return theta;
}

void FillPerceptionPolygon(PerceptionObstacle* const perception_obstacle,
                           const double mid_x, const double mid_y, const double mid_z,
                           const double length, const double width, const double height,
                           const double heading) {
  for (int i = 0; i < 8; ++i) {
    perception_obstacle->add_polygon_point();
  }
  
  // axis z
  for (int i = 0; i < 8; ++i) {
    double sign = i%2 == 0 ? 1.0 : -1.0;
    perception_obstacle->mutable_polygon_point(i)->set_z(mid_z + sign * height / 2.0);
  }

  // axis x
  for (int i = 0; i < 8; ++i) {
    if (i == 0 || i == 1) { 
      perception_obstacle->mutable_polygon_point(i)->set_x(mid_x + 
                         length * std::cos(heading) / 2.0 + width * std::sin(heading) / 2.0);
    } else if (i == 2 || i == 3) {
      perception_obstacle->mutable_polygon_point(i)->set_x(mid_x + 
                         length * std::cos(heading) / 2.0 - width * std::sin(heading) / 2.0);
    } else if (i == 4 || i == 5) {
      perception_obstacle->mutable_polygon_point(i)->set_x(mid_x - 
                         length * std::cos(heading) / 2.0 - width * std::sin(heading) / 2.0);
    } else {
      perception_obstacle->mutable_polygon_point(i)->set_x(mid_x - 
                         length * std::cos(heading) / 2.0 + width * std::sin(heading) / 2.0);
    }
  }
  
  // axis y
  for (int i = 0; i < 8; ++i) {
    if (i == 0 || i == 1) { 
      perception_obstacle->mutable_polygon_point(i)->set_y(mid_y + 
                         length * std::sin(heading) / 2.0 - width * std::cos(heading) / 2.0);
    } else if (i == 2 || i == 3) {
      perception_obstacle->mutable_polygon_point(i)->set_y(mid_y + 
                         length * std::sin(heading) / 2.0 + width * std::cos(heading) / 2.0);
    } else if (i == 4 || i == 5) {
      perception_obstacle->mutable_polygon_point(i)->set_y(mid_y - 
                         length * std::sin(heading) / 2.0 + width * std::cos(heading) / 2.0);
    } else {
      perception_obstacle->mutable_polygon_point(i)->set_y(mid_y - 
                         length * std::sin(heading) / 2.0 - width * std::cos(heading) / 2.0);
    }
  }
}

double GetDefaultObjectLength(const int object_type) {
  double default_object_length = 0.0;
  switch (object_type) {
    case 0: {
      default_object_length = FLAGS_default_car_length;
      break;
    }
    case 1: {
      default_object_length = FLAGS_default_truck_length;
      break;
    }
    case 2: {
      default_object_length = FLAGS_default_bike_length;
      break;
    }
    case 3: {
      default_object_length = FLAGS_default_ped_length;
      break;
    }
    case 4: {
      default_object_length = FLAGS_default_unknown_length;
      break;
    }
  }
  return default_object_length;
}

double GetDefaultObjectWidth(const int object_type) {
  double default_object_width = 0.0;
  switch (object_type) {
    case 0: {
      default_object_width = FLAGS_default_car_width;
      break;
    }
    case 1: {
      default_object_width = FLAGS_default_truck_width;
      break;
    }
    case 2: {
      default_object_width = FLAGS_default_bike_width;
      break;
    }
    case 3: {
      default_object_width = FLAGS_default_ped_width;
      break;
    }
    case 4: {
      default_object_width = FLAGS_default_unknown_width;
      break;
    }
  }
  return default_object_width;
}


}  // namespace l3_perception
}  // namespace apollo


