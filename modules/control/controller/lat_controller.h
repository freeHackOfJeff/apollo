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
 * @brief Defines the LatController class.
 */

#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "Eigen/Core"

#include "modules/common/configs/proto/vehicle_config.pb.h"

#include "modules/common/filters/digital_filter.h"
#include "modules/common/filters/digital_filter_coefficients.h"
#include "modules/common/filters/mean_filter.h"
#include "modules/control/common/interpolation_1d.h"
#include "modules/control/common/leadlag_controller.h"
#include "modules/control/common/trajectory_analyzer.h"
#include "modules/control/controller/controller.h"

/**
 * @namespace apollo::control
 * @brief apollo::control
 */
namespace apollo {
namespace control {

/**
 * @class LatController
 *
 * @brief LQR-Based lateral controller, to compute steering target.
 * For more details, please refer to "Vehicle dynamics and control."
 * Rajamani, Rajesh. Springer Science & Business Media, 2011.
 */
class LatController : public Controller {
 public:
  /**
   * @brief constructor
   */
  LatController();

  /**
   * @brief destructor
   */
  virtual ~LatController();

  /**
   * @brief initialize Lateral Controller
   * @param control_conf control configurations
   * @return Status initialization status
   */
  common::Status Init(const ControlConf *control_conf) override;

  /**
   * @brief compute steering target based on current vehicle status
   *        and target trajectory
   * @param localization vehicle location
   * @param chassis vehicle status e.g., speed, acceleration
   * @param trajectory trajectory generated by planning
   * @param cmd control command
   * @return Status computation status
   */
  common::Status ComputeControlCommand(
      const localization::LocalizationEstimate *localization,
      const canbus::Chassis *chassis, const planning::ADCTrajectory *trajectory,
      ControlCommand *cmd) override;

  /**
   * @brief reset Lateral Controller
   * @return Status reset status
   */
  common::Status Reset() override;

  /**
   * @brief stop Lateral controller
   */
  void Stop() override;

  /**
   * @brief Lateral controller name
   * @return string controller name in string
   */
  std::string Name() const override;

 protected:
  void UpdateState(SimpleLateralDebug *debug);

  // logic for reverse driving mode
  void UpdateDrivingOrientation();

  void UpdateMatrix();

  void UpdateMatrixCompound();

  double ComputeFeedForward(double ref_curvature) const;

  void ComputeLateralErrors(const double x, const double y, const double theta,
                            const double linear_v, const double angular_v,
                            const TrajectoryAnalyzer &trajectory_analyzer,
                            SimpleLateralDebug *debug);
  bool LoadControlConf(const ControlConf *control_conf);
  void InitializeFilters(const ControlConf *control_conf);
  void LoadLatGainScheduler(const LatControllerConf &lat_controller_conf);
  void LogInitParameters();
  void ProcessLogs(const SimpleLateralDebug *debug,
                   const canbus::Chassis *chassis);

  void CloseLogFile();

  // vehicle
  const ControlConf *control_conf_ = nullptr;

  // vehicle parameter
  common::VehicleParam vehicle_param_;

  // a proxy to analyze the planning trajectory
  TrajectoryAnalyzer trajectory_analyzer_;

  // the following parameters are vehicle physics related.
  // control time interval
  double ts_ = 0.0;
  // corner stiffness; front
  double cf_ = 0.0;
  // corner stiffness; rear
  double cr_ = 0.0;
  // distance between front and rear wheel center
  double wheelbase_ = 0.0;
  // mass of the vehicle
  double mass_ = 0.0;
  // distance from front wheel center to COM
  double lf_ = 0.0;
  // distance from rear wheel center to COM
  double lr_ = 0.0;
  // rotational inertia
  double iz_ = 0.0;
  // the ratio between the turn of the steering wheel and the turn of the wheels
  double steer_ratio_ = 0.0;
  // the maximum turn of steer
  double steer_single_direction_max_degree_ = 0.0;

  // limit steering to maximum theoretical lateral acceleration
  double max_lat_acc_ = 0.0;

  // number of control cycles look ahead (preview controller)
  int preview_window_ = 0;
  // number of states without previews, includes
  // lateral error, lateral error rate, heading error, heading error rate
  const int basic_state_size_ = 4;
  // vehicle state matrix
  Eigen::MatrixXd matrix_a_;
  // vehicle state matrix (discrete-time)
  Eigen::MatrixXd matrix_ad_;
  // vehicle state matrix compound; related to preview
  Eigen::MatrixXd matrix_adc_;
  // control matrix
  Eigen::MatrixXd matrix_b_;
  // control matrix (discrete-time)
  Eigen::MatrixXd matrix_bd_;
  // control matrix compound
  Eigen::MatrixXd matrix_bdc_;
  // gain matrix
  Eigen::MatrixXd matrix_k_;
  // control authority weighting matrix
  Eigen::MatrixXd matrix_r_;
  // state weighting matrix
  Eigen::MatrixXd matrix_q_;
  // updated state weighting matrix
  Eigen::MatrixXd matrix_q_updated_;
  // vehicle state matrix coefficients
  Eigen::MatrixXd matrix_a_coeff_;
  // 4 by 1 matrix; state matrix
  Eigen::MatrixXd matrix_state_;

  // parameters for lqr solver; number of iterations
  int lqr_max_iteration_ = 0;
  // parameters for lqr solver; threshold for computation
  double lqr_eps_ = 0.0;

  common::DigitalFilter digital_filter_;

  std::unique_ptr<Interpolation1D> lat_err_interpolation_;

  std::unique_ptr<Interpolation1D> heading_err_interpolation_;

  // MeanFilter heading_rate_filter_;
  common::MeanFilter lateral_error_filter_;
  common::MeanFilter heading_error_filter_;

  // Lead/Lag controller
  LeadlagController leadlag_controller_;

  // for logging purpose
  std::ofstream steer_log_file_;

  const std::string name_;

  double query_relative_time_;

  double pre_steer_angle_ = 0.0;

  double minimum_speed_protection_ = 0.1;

  double current_trajectory_timestamp_ = -1.0;

  double init_vehicle_x_ = 0.0;

  double init_vehicle_y_ = 0.0;

  double init_vehicle_heading_ = 0.0;

  double min_turn_radius_ = 0.0;

  double driving_orientation_ = 0.0;
};

}  // namespace control
}  // namespace apollo
