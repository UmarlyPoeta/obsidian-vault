---
typ: solar
tags: [solar, mapa]
zaktualizowano: 2026-10-04
---
# Mapa topików i serwisów

> [!skrot] W 30 sekund
> - Wygenerowana z analizy kodu wszystkich repo: 68 węzłów, 158 topików, 11 serwisów/akcji. Gałęzie: najnowsze wymeldowane (stan po zawodach), szczegóły w notatkach repo.
> - ⚠️ = coś się nie zgadza: nikt nie publikuje (dane muszą przyjść z zewnątrz, np. driver spoza `~/solar`), nikt nie słucha, albo różne typy po obu stronach.
> - Szukaj topiku: Ctrl+F. Kto co robi: kliknij pakiet.

## Topiki

| Topik | Typ | Publikuje | Subskrybuje | Uwagi |
|---|---|---|---|---|
| `/from_can_bus` | `can_msgs/msg/Frame` | — | [[can_driver]]/CanDriver | ⚠️ brak nadawcy w ~/solar |
| `/to_can_bus` | `can_msgs/msg/Frame` | [[can_sequence]]/can_sequence<br>[[diff_thrust_driver]]/DiffThrustDriver<br>[[new_diff_thrust_driver]]/NewDiffThrustDriver<br>[[peripherals_handler]]/PeripheralsHandler<br>[[regulator]]/Regulator | [[sim_can_adapter]]/SimCanAdapter |  |
| `/filter/euler` | `geometry_msgs/msg/Vector3Stamped` | [[imu_to_yaw]]/imu_to_yaw | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs)<br>[[message_protobuf]]/protobuf<br>[[regulator]]/Regulator |  |
| `/filter/positionlla` | `geometry_msgs/msg/Vector3Stamped` | — | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs) | ⚠️ brak nadawcy w ~/solar |
| `/filter/velocity` | `geometry_msgs/msg/Vector3Stamped` | — | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs)<br>[[basic_controller]]/my_bt_node (+ węzły BT)<br>[[message_protobuf]]/protobuf<br>[[odometry]]/imu_odometry_node<br>[[regulator]]/Regulator<br>[[swim_sequence]]/SwimSequence | ⚠️ brak nadawcy w ~/solar |
| `/filtered_cloud` | `sensor_msgs/msg/PointCloud2` | [[obstacle_detector]]/ObstacleDetector | — | ⚠️ nikt nie słucha |
| `/imu/data` | `sensor_msgs/msg/Imu`<br>`std_msgs/msg/Float32` | — | [[odometry]]/imu_odometry_node<br>[[regulator]]/Regulator<br>[[tracking_detection]]/TrackingDetection | ⚠️ brak nadawcy w ~/solar; ⚠️ różne typy |
| `/l1/detection_buoy_detector` | `l1_interfaces/msg/VectorOfDetections` | [[vision_detector]]/buoy_detector | [[detection_fusion]]/DetectionFusion<br>[[detection_fusion_projection]]/DetectionFusionProjection<br>[[detection_pointcloud_fusion]]/DetectionPointCloudFusion |  |
| `/l1/detection_buoy_detector_simulation` | `l1_interfaces/msg/VectorOfDetections` | [[vision_detector]]/buoy_detector_simulation | — | ⚠️ nikt nie słucha |
| `/l1/obstacle_detection` | `l1_interfaces/msg/VectorOfClusters` | [[obstacle_detector]]/ObstacleDetector | [[Pakiety/sbt_transforms|sbt_transforms]]/sbt_axis_normalizer<br>[[basic_controller]]/my_bt_node (+ węzły BT)<br>[[detection_fusion]]/DetectionFusion<br>[[detection_fusion_projection]]/DetectionFusionProjection<br>[[mapper]]/mapper<br>[[obstacle_avoidance]]/obstacle_avoidance_node<br>[[pose_graph]]/feature_extractor (+GlobalMapBuilder) |  |
| `/l1/visualization/bb_320_compressed_buoy_detector` | `sensor_msgs/msg/Image` | [[vision_detector]]/buoy_detector | [[agh-dashboard-live]]/ros_image_to_rtsp |  |
| `/l1/visualization/bb_320_compressed_buoy_detector_simulation` | `sensor_msgs/msg/Image` | [[vision_detector]]/buoy_detector_simulation | — | ⚠️ nikt nie słucha |
| `/l2/speed_vector` | `l0_interfaces/msg/SpeedVector` | [[basic_controller]]/my_bt_node (+ węzły BT)<br>[[planner]]/planner<br>[[swim_sequence]]/SwimSequence<br>[[trajectory_planner]]/TrajectoryPlannerNode | [[diff_thrust_driver]]/DiffThrustDriver<br>[[sim_thrust_driver]]/SimThrustDriver |  |
| `/slam/boat_marker` | `visualization_msgs/msg/Marker` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/slam/boat_pose` | `nav_msgs/msg/Odometry` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) |  |
| `/slam/debug/candidate_lidar_pose` | `nav_msgs/msg/Odometry` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/slam/debug/candidate_scan_map` | `sensor_msgs/msg/PointCloud2` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/slam/debug/deskewed_scan` | `sensor_msgs/msg/PointCloud2` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/slam/debug/frame_axes` | `visualization_msgs/msg/Marker` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/slam/debug/map_to_odom` | `nav_msgs/msg/Odometry` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/slam/debug/predicted_lidar_pose` | `nav_msgs/msg/Odometry` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) |  |
| `/slam/debug/predicted_scan_map` | `sensor_msgs/msg/PointCloud2` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/slam/debug/raw_scan` | `sensor_msgs/msg/PointCloud2` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/slam/dock_polygon_map` | `geometry_msgs/msg/PolygonStamped` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/slam/global_costmap` | `nav_msgs/msg/OccupancyGrid` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | [[planner]]/planner |  |
| `/slam/global_map_points` | `sensor_msgs/msg/PointCloud2` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/slam/local_map_points` | `sensor_msgs/msg/PointCloud2` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) |  |
| `/slam/odom` | `nav_msgs/msg/Odometry` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | [[basic_controller]]/my_bt_node (+ węzły BT)<br>[[colision_detector]]/colision_detector_node<br>[[planner]]/planner<br>[[regulator]]/Regulator<br>[[trajectory_planner]]/TrajectoryPlannerNode |  |
| `/slam/odom_marker` | `visualization_msgs/msg/Marker` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/slam/odom_transform` | `geometry_msgs/msg/TransformStamped` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | [[basic_controller]]/my_bt_node (+ węzły BT) |  |
| `/slam/tracked_detections_raw` | `l2_interfaces/msg/TrackedDetectionsVector` | — | [[Pakiety/sbt_transforms|sbt_transforms]]/sbt_axis_normalizer | ⚠️ brak nadawcy w ~/solar |
| `/slam/trajectory_extrapolation` | `geometry_msgs/msg/PoseArray` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | [[colision_detector]]/colision_detector_node |  |
| `/l3/pause_wp_driver` | `std_msgs/msg/Bool` | — | [[trajectory_planner]]/TrajectoryPlannerNode | ⚠️ brak nadawcy w ~/solar |
| `/l3/waypoint_reached` | `std_msgs/msg/Bool` | [[trajectory_planner]]/TrajectoryPlannerNode | [[basic_controller]]/my_bt_node (+ węzły BT)<br>[[trajectory_planner]]/TrajectoryPlannerNode<br>[[waypoint_sequencer]]/waypoint_sequencer_node |  |
| `/lidar_camera_projection` | `sensor_msgs/msg/Image` | [[fusion_visualization]]/fusion_visualization | — | ⚠️ nikt nie słucha |
| `/lidar_points` | `sensor_msgs/msg/PointCloud2` | — | [[agh-dashboard-live]]/dashboard_pointcloud_downsampler<br>[[detection_pointcloud_fusion]]/DetectionPointCloudFusion<br>[[fusion_visualization]]/fusion_visualization<br>[[obstacle_detector]]/ObstacleDetector<br>[[pointcloud_to_laserscan]]/pointcloud_to_laserscan<br>[[pose_graph]]/feature_extractor (+GlobalMapBuilder)<br>[[ros2_camera_lidar_fusion]]/lidar_camera_projection<br>[[ros2_camera_lidar_fusion]]/save_data | ⚠️ brak nadawcy w ~/solar |
| `/zed/zed_node/left/image_rect_color` | `sensor_msgs/msg/Image` | — | [[ros2_camera_lidar_fusion]]/lidar_camera_projection<br>[[ros2_camera_lidar_fusion]]/save_data | ⚠️ brak nadawcy w ~/solar |
| `/zed/zed_node/point_cloud/cloud_registered` | `sensor_msgs/msg/PointCloud2` | [[zed-ros2-wrapper-humble-v5.1.0]]//zed/zed_node | [[vision_detector]]/buoy_detector |  |
| `/zed/zed_node/rgb/color/rect/camera_info` | `sensor_msgs/msg/CameraInfo` | [[zed-ros2-wrapper-humble-v5.1.0]]//zed/zed_node | — | ⚠️ nikt nie słucha |
| `/zed/zed_node/rgb/color/rect/image` | `sensor_msgs/msg/Image` | [[vision_detector]]/image_publisher<br>[[zed-ros2-wrapper-humble-v5.1.0]]//zed/zed_node | [[agh-dashboard-live]]/ros_image_to_rtsp<br>[[fusion_visualization]]/fusion_visualization<br>[[images_collector]]/images_collector<br>[[vision_detector]]/buoy_detector<br>[[zed-service]]/zed-pls-dont-die.sh (watchdog, ros2 topic echo --once)<br>[[zed_controller]]/zed_exposure_controller |  |
| `/zed_exposure/diag` | `std_msgs/msg/Float32MultiArray` | [[zed_controller]]/zed_exposure_controller | — | ⚠️ nikt nie słucha |
| `/bms/data` | `l0_interfaces/msg/BmsData` | [[can_driver]]/CanDriver | [[agh-dashboard-live]]/battery_status_publisher<br>[[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs) |  |
| `/bms/voltage` | `l0_interfaces/msg/BmsVoltages` | [[can_driver]]/CanDriver | [[agh-dashboard-live]]/battery_status_publisher<br>[[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs) |  |
| `/boat_mark` | `visualization_msgs/msg/Marker` | [[mapper]]/mapper | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs) |  |
| `/bow_thrusters` | `std_msgs/msg/Int32` | [[basic_controller]]/my_bt_node (+ węzły BT) | — | ⚠️ nikt nie słucha |
| `/bt_tui_state` | `(auto)`<br>`l3_interfaces/msg/TuiState` | [[basic_controller]]/my_bt_node (+ węzły BT) | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs)<br>[[basic_controller]]/bt_tui_viewer | ⚠️ różne typy |
| `/canonical/fused_detection` | `l1_interfaces/msg/VectorOfFusedDetections` | [[Pakiety/sbt_transforms|sbt_transforms]]/sbt_axis_normalizer | — | ⚠️ nikt nie słucha |
| `/canonical/fused_detection_map` | `l1_interfaces/msg/VectorOfFusedDetections` | [[Pakiety/sbt_transforms|sbt_transforms]]/sbt_axis_normalizer | — | ⚠️ nikt nie słucha |
| `/canonical/obstacle_detection` | `l1_interfaces/msg/VectorOfClusters` | [[Pakiety/sbt_transforms|sbt_transforms]]/sbt_axis_normalizer | — | ⚠️ nikt nie słucha |
| `/canonical/odom` | `nav_msgs/msg/Odometry` | [[Pakiety/sbt_transforms|sbt_transforms]]/sbt_axis_normalizer | — | ⚠️ nikt nie słucha |
| `/celka/temporary/transition` | `std_msgs/msg/Bool` | [[basic_controller]]/my_bt_node (+ węzły BT) | — | ⚠️ nikt nie słucha |
| `/character_points` | `visualization_msgs/msg/MarkerArray` | [[mapper]]/mapper | — | ⚠️ nikt nie słucha |
| `/choose_dock` | `geometry_msgs/msg/Point` | [[basic_controller]]/my_bt_node (+ węzły BT) | [[basic_controller]]/my_bt_node (+ węzły BT)<br>[[segment_scan]]/segment_scan_node |  |
| `/clock` | `rosgraph_msgs/msg/Clock` | — | [[cpp_nodes]]/imu_odometry_node (odo_cpp_node) | ⚠️ brak nadawcy w ~/solar |
| `/cmd_vel` | `geometry_msgs/msg/Twist` | [[boat_teleop]]/boat_teleop | [[cpp_nodes]]/vel_node (vel_to_thrusters)<br>[[new_diff_thrust_driver]]/NewDiffThrustDriver |  |
| `/colision_detector` | `geometry_msgs/msg/Point` | [[colision_detector]]/colision_detector_node<br>[[obstacle_avoidance]]/obstacle_avoidance_node | [[basic_controller]]/my_bt_node (+ węzły BT) |  |
| `/colision_direction` | `std_msgs/msg/String` | [[colision_detector]]/colision_detector_node | [[basic_controller]]/my_bt_node (+ węzły BT) |  |
| `/costmap` | `nav_msgs/msg/OccupancyGrid` | [[mapper]]/mapper | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs)<br>[[ascii_map_viz2]]/ascii_costmap_viz |  |
| `/course/data_sim` | `std_msgs/msg/Float32` | [[gps_to_course_converter]]/GpsToCourseConverter | — | ⚠️ nikt nie słucha |
| `/dashboard/battery_status` | `std_msgs/msg/String` | [[agh-dashboard-live]]/battery_status_publisher | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs) |  |
| `/dashboard/lidar_points` | `sensor_msgs/msg/PointCloud2` | [[agh-dashboard-live]]/dashboard_pointcloud_downsampler | — | ⚠️ nikt nie słucha |
| `/debug/deskewed` | `sensor_msgs/msg/PointCloud2` | — | [[fast_scan]]/fast_cloud_to_scan_clustered | ⚠️ brak nadawcy w ~/solar |
| `/debug_points` | `visualization_msgs/msg/MarkerArray` | [[fast_scan]]/fast_cloud_to_scan_clustered | — | ⚠️ nikt nie słucha |
| `/delivery` | `l3_interfaces/msg/Delivery` | [[basic_controller]]/my_bt_node (+ węzły BT) | [[message_protobuf]]/protobuf |  |
| `/detected_signals` | `l2_interfaces/msg/BlastDetection`<br>`std_msgs/msg/String` | [[blast_detection]]/blast_detector_node<br>[[signals_detector]]/signals_detector_node | [[basic_controller]]/my_bt_node (+ węzły BT) | ⚠️ różne typy |
| `/deuse_detections` | `std_msgs/msg/Bool` | — | [[mapper]]/mapper<br>[[pose_graph]]/feature_extractor (+GlobalMapBuilder) | ⚠️ brak nadawcy w ~/solar |
| `/dock/side` | `std_msgs/msg/String` | — | [[message_protobuf]]/protobuf | ⚠️ brak nadawcy w ~/solar |
| `/dock/slip` | `std_msgs/msg/Float32` | — | [[message_protobuf]]/protobuf | ⚠️ brak nadawcy w ~/solar |
| `/dock_detection` | `l2_interfaces/msg/Dock` | [[segment_scan]]/segment_scan_node | [[basic_controller]]/my_bt_node (+ węzły BT) |  |
| `/dock_marker` | `visualization_msgs/msg/Marker` | [[segment_scan]]/segment_scan_node | — | ⚠️ nikt nie słucha |
| `/dock_polygon` | `geometry_msgs/msg/PolygonStamped` | [[segment_scan]]/segment_scan_node | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) |  |
| `/echologger/depth` | `sensor_msgs/msg/Range` | [[sonar]]/echologger_range_publisher | [[sonar]]/boat_echo_csv_logger |  |
| `/echologger/raw_echo` | `std_msgs/msg/String` | [[sonar]]/echologger_range_publisher | [[sonar]]/boat_echo_csv_logger |  |
| `/features/debris` | `sensor_msgs/msg/PointCloud2` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/features/edges` | `sensor_msgs/msg/PointCloud2` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) |  |
| `/features/planes` | `sensor_msgs/msg/PointCloud2` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) |  |
| `/from_can_bus_fd` | `ros2_socketcan_msgs/msg/FdFrame` | [[ros2_socketcan]]/socket_can_receiver | — | ⚠️ nikt nie słucha |
| `/fused_detection` | `l1_interfaces/msg/VectorOfFusedDetections` | [[detection_fusion]]/DetectionFusion<br>[[detection_fusion_projection]]/DetectionFusionProjection<br>[[detection_pointcloud_fusion]]/DetectionPointCloudFusion | [[Pakiety/sbt_transforms|sbt_transforms]]/sbt_axis_audit<br>[[Pakiety/sbt_transforms|sbt_transforms]]/sbt_axis_normalizer<br>[[mapper]]/mapper<br>[[message_protobuf]]/protobuf<br>[[pose_graph]]/feature_extractor (+GlobalMapBuilder)<br>[[tracking_detection]]/TrackingDetection |  |
| `/fusion_projection` | `sensor_msgs/msg/Image` | [[ros2_camera_lidar_fusion]]/lidar_camera_projection | — | ⚠️ nikt nie słucha |
| `/fusion_visualization` | `visualization_msgs/msg/MarkerArray` | [[detection_fusion]]/DetectionFusion<br>[[detection_pointcloud_fusion]]/DetectionPointCloudFusion | — | ⚠️ nikt nie słucha |
| `/global/clusters` | `l1_interfaces/msg/VectorOfClusters` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | [[colision_detector]]/colision_detector_node |  |
| `/global_map` | `grid_map_msgs/msg/GridMap` | [[mapper]]/mapper | — | ⚠️ nikt nie słucha |
| `/gnss` | `sensor_msgs/msg/NavSatFix` | — | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs)<br>[[basic_controller]]/my_bt_node (+ węzły BT)<br>[[message_protobuf]]/protobuf<br>[[odometry]]/imu_odometry_node<br>[[sonar]]/boat_echo_csv_logger | ⚠️ brak nadawcy w ~/solar |
| `/gnss_pose` | `geometry_msgs/msg/PoseStamped` | — | [[odometry]]/imu_odometry_node | ⚠️ brak nadawcy w ~/solar |
| `/goal` | `geometry_msgs/msg/PoseArray` | [[basic_controller]]/my_bt_node (+ węzły BT) | [[planner]]/planner |  |
| `/gps/metric_position` | `l2_interfaces/msg/MetricPosition` | [[gps_calculator]]/GpsConverter | — | ⚠️ nikt nie słucha |
| `/gps/nav_sat_fix` | `sensor_msgs/msg/NavSatFix` | — | [[gps_calculator]]/GpsConverter | ⚠️ brak nadawcy w ~/solar |
| `/ground_cloud` | `sensor_msgs/msg/PointCloud2` | [[obstacle_detector]]/ObstacleDetector | — | ⚠️ nikt nie słucha |
| `/gui_waypoint_reached` | `geometry_msgs/msg/Point` | [[trajectory_planner]]/TrajectoryPlannerNode | — | ⚠️ nikt nie słucha |
| `/hazard_buoy` | `geometry_msgs/msg/PointStamped` | [[basic_controller]]/my_bt_node (+ węzły BT) | — | ⚠️ nikt nie słucha |
| `/invalid_waypoints` | `geometry_msgs/msg/PoseArray` | [[planner]]/planner | — | ⚠️ nikt nie słucha |
| `/local_map` | `grid_map_msgs/msg/GridMap` | [[mapper]]/mapper | — | ⚠️ nikt nie słucha |
| `/local_voxel_map` | `sensor_msgs/msg/PointCloud2` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/map_clusters` | `visualization_msgs/msg/MarkerArray` | [[pose_graph]]/feature_extractor (+GlobalMapBuilder) | — | ⚠️ nikt nie słucha |
| `/map_detections` | `visualization_msgs/msg/MarkerArray` | [[mapper]]/mapper<br>[[pose_graph]]/feature_extractor (+GlobalMapBuilder) | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs)<br>[[viz_correction]]/swap_xy_node |  |
| `/map_detections_2` | `visualization_msgs/msg/MarkerArray` | [[viz_correction]]/swap_xy_node | — | ⚠️ nikt nie słucha |
| `/map_trajectory` | `visualization_msgs/msg/Marker` | [[mapper]]/mapper | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs) |  |
| `/marina_entry` | `geometry_msgs/msg/PoseArray` | [[segment_scan]]/segment_scan_node | [[basic_controller]]/my_bt_node (+ węzły BT) |  |
| `/marina_possible_entry` | `geometry_msgs/msg/PoseArray` | [[segment_scan]]/segment_scan_node | — | ⚠️ nikt nie słucha |
| `/marina_sides` | `l3_interfaces/msg/MarinaSides` | [[segment_scan]]/segment_scan_node | — | ⚠️ nikt nie słucha |
| `/melody` | `std_msgs/msg/Int16` | [[basic_controller]]/my_bt_node (+ węzły BT) | — | ⚠️ nikt nie słucha |
| `/mission_state/data` | `l0_interfaces/msg/MissionState` | [[basic_controller]]/my_bt_node (+ węzły BT)<br>[[trajectory_planner]]/TrajectoryPlannerNode | [[basic_controller]]/my_bt_node (+ węzły BT)<br>[[trajectory_planner]]/TrajectoryPlannerNode |  |
| `/obstacle_markers` | `visualization_msgs/msg/MarkerArray` | [[obstacle_detector]]/ObstacleDetector | — | ⚠️ nikt nie słucha |
| `/odom` | `nav_msgs/msg/Odometry` | [[odometry]]/imu_odometry_node | [[Pakiety/sbt_transforms|sbt_transforms]]/sbt_axis_audit<br>[[Pakiety/sbt_transforms|sbt_transforms]]/sbt_axis_normalizer<br>[[Pakiety/sbt_transforms|sbt_transforms]]/sbt_tf_bridge<br>[[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs)<br>[[ascii_map_viz2]]/ascii_costmap_viz<br>[[basic_controller]]/my_bt_node (+ węzły BT)<br>[[mapper]]/mapper<br>[[obstacle_avoidance]]/obstacle_avoidance_node<br>[[pose_graph]]/feature_extractor (+GlobalMapBuilder)<br>[[tracking_detection]]/TrackingDetection |  |
| `/odom2` | `nav_msgs/msg/Odometry` | [[cpp_nodes]]/imu_odometry_node (odo_cpp_node) | [[odom_to_tf_sim]]/odom_to_tf_node |  |
| `/output_lidar_points` | `sensor_msgs/msg/PointCloud2` | [[ros2_camera_lidar_fusion]]/lidar_camera_projection<br>[[ros2_camera_lidar_fusion]]/save_data | [[ros2_camera_lidar_fusion]]/lidar_camera_projection<br>[[ros2_camera_lidar_fusion]]/save_data |  |
| `/output_zed` | `sensor_msgs/msg/Image` | [[ros2_camera_lidar_fusion]]/lidar_camera_projection | [[ros2_camera_lidar_fusion]]/get_intrinsic_camera_calibration<br>[[ros2_camera_lidar_fusion]]/lidar_camera_projection |  |
| `/path` | `nav_msgs/msg/Path` | [[planner]]/planner | [[ascii_map_viz2]]/ascii_costmap_viz<br>[[viz_correction]]/swap_xy_node |  |
| `/path_2` | `nav_msgs/msg/Path` | [[viz_correction]]/swap_xy_node | — | ⚠️ nikt nie słucha |
| `/path_markers` | `visualization_msgs/msg/MarkerArray` | [[planner]]/planner | — | ⚠️ nikt nie słucha |
| `/path_points` | `geometry_msgs/msg/PoseArray` | [[planner]]/planner | [[regulator]]/Regulator<br>[[trajectory_planner]]/TrajectoryPlannerNode |  |
| `/peripheral` | `std_msgs/msg/Float32MultiArray` | [[can_driver]]/CanDriver | — | ⚠️ nikt nie słucha |
| `/pick_dock` | `?` | — | [[basic_controller]]/my_bt_node (+ węzły BT) | ⚠️ brak nadawcy w ~/solar |
| `/protobuf_message` | `std_msgs/msg/UInt8MultiArray` | [[message_protobuf]]/protobuf | — | ⚠️ nikt nie słucha |
| `/rc_chan` | `std_msgs/msg/Float32MultiArray` | [[can_driver]]/CanDriver | — | ⚠️ nikt nie słucha |
| `/regulator_or_thrust_driver` | `std_msgs/msg/String` | [[basic_controller]]/my_bt_node (+ węzły BT) | [[diff_thrust_driver]]/DiffThrustDriver<br>[[regulator]]/Regulator |  |
| `/regulator_speed` | `std_msgs/msg/Float64` | [[basic_controller]]/my_bt_node (+ węzły BT) | [[regulator]]/Regulator |  |
| `/robot/speed_challenge` | `?`<br>`SpeedReport` | — | [[basic_controller]]/my_bt_node (+ węzły BT)<br>[[message_protobuf]]/protobuf | ⚠️ brak nadawcy w ~/solar; ⚠️ różne typy |
| `/robot/state` | `std_msgs/msg/String` | [[can_driver]]/CanDriver | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs)<br>[[message_protobuf]]/protobuf |  |
| `/robot/task` | `std_msgs/msg/String` | [[basic_controller]]/my_bt_node (+ węzły BT) | [[message_protobuf]]/protobuf |  |
| `/scan` | `sensor_msgs/msg/LaserScan` | [[fast_scan]]/fast_cloud_to_scan_clustered<br>[[pointcloud_to_laserscan]]/pointcloud_to_laserscan<br>[[scan_rotator]]/scan_rotator | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs)<br>[[segment_scan]]/segment_scan_node |  |
| `/scan_high` | `sensor_msgs/msg/LaserScan` | — | [[segment_scan]]/segment_scan_node | ⚠️ brak nadawcy w ~/solar |
| `/scan_prerotated` | `sensor_msgs/msg/LaserScan` | — | [[scan_rotator]]/scan_rotator | ⚠️ brak nadawcy w ~/solar |
| `/scanchuj` | `sensor_msgs/msg/LaserScan` | — | [[mapper]]/mapper | ⚠️ brak nadawcy w ~/solar |
| `/sim/imu/course` | `std_msgs/msg/Float32` | [[course_imu]]/CourseImuConverter | — | ⚠️ nikt nie słucha |
| `/sim/imu/velocity` | `std_msgs/msg/Float32` | [[course_imu]]/CourseImuConverter | — | ⚠️ nikt nie słucha |
| `/sound_signal` | `std_msgs/msg/String` | — | [[message_protobuf]]/protobuf | ⚠️ brak nadawcy w ~/solar |
| `/spatial_points` | `visualization_msgs/msg/MarkerArray` | — | [[viz_correction]]/swap_xy_node | ⚠️ brak nadawcy w ~/solar |
| `/start_pose` | `geometry_msgs/msg/Point` | [[odometry]]/imu_odometry_node | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs)<br>[[basic_controller]]/my_bt_node (+ węzły BT)<br>[[message_protobuf]]/protobuf |  |
| `/status` | `xsens_mti_ros2_driver/msg/XsStatusWord` | — | [[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs) | ⚠️ brak nadawcy w ~/solar |
| `/survivor_buoy` | `geometry_msgs/msg/PointStamped` | [[basic_controller]]/my_bt_node (+ węzły BT) | — | ⚠️ nikt nie słucha |
| `/temporary/transition` | `std_msgs/msg/Bool` | [[waypoint_sequencer]]/waypoint_sequencer_node | — | ⚠️ nikt nie słucha |
| `/thrusters/setpoints` | `std_msgs/msg/Float32MultiArray` | [[can_driver]]/CanDriver | — | ⚠️ nikt nie słucha |
| `/to_can_bus_fd` | `ros2_socketcan_msgs/msg/FdFrame` | — | [[ros2_socketcan]]/socket_can_sender | ⚠️ brak nadawcy w ~/solar |
| `/tracked_detections` | `l2_interfaces/msg/TrackedDetectionsVector` | [[Pakiety/sbt_transforms|sbt_transforms]]/sbt_axis_normalizer<br>[[mapper]]/mapper<br>[[pose_graph]]/feature_extractor (+GlobalMapBuilder) | [[Pakiety/sbt_transforms|sbt_transforms]]/sbt_axis_audit<br>[[agh-dashboard-live]]/rosbridge_websocket (+ live-rosbridge-publisher.mjs)<br>[[ascii_map_viz2]]/ascii_costmap_viz<br>[[basic_controller]]/my_bt_node (+ węzły BT) |  |
| `/tracking_detection` | `l1_interfaces/msg/VectorOfFusedDetections` | [[tracking_detection]]/TrackingDetection | — | ⚠️ nikt nie słucha |
| `/tracking_visualization` | `visualization_msgs/msg/MarkerArray` | [[tracking_detection]]/TrackingDetection | — | ⚠️ nikt nie słucha |
| `/trajectory` | `geometry_msgs/msg/PoseArray` | [[odometry]]/imu_odometry_node | [[mapper]]/mapper<br>[[obstacle_avoidance]]/obstacle_avoidance_node |  |
| `/true_heading/data` | `l0_interfaces/msg/Course` | — | [[Pakiety/sbt_transforms|sbt_transforms]]/sbt_axis_audit | ⚠️ brak nadawcy w ~/solar |
| `/tunnel_marker` | `visualization_msgs/msg/Marker` | [[colision_detector]]/colision_detector_node<br>[[obstacle_avoidance]]/obstacle_avoidance_node | — | ⚠️ nikt nie słucha |
| `/used_buoy_id` | `std_msgs/msg/Int32` | [[basic_controller]]/my_bt_node (+ węzły BT) | — | ⚠️ nikt nie słucha |
| `/used_detections` | `std_msgs/msg/Int32` | [[basic_controller]]/my_bt_node (+ węzły BT) | [[mapper]]/mapper<br>[[pose_graph]]/feature_extractor (+GlobalMapBuilder) |  |
| `/wamv/sensors/cameras/front_left_camera_sensor/image_raw` | `sensor_msgs/msg/Image` | — | [[vision_detector]]/buoy_detector_simulation | ⚠️ brak nadawcy w ~/solar |
| `/wamv/sensors/cameras/front_left_camera_sensor/points` | `sensor_msgs/msg/PointCloud2` | — | [[vision_detector]]/buoy_detector_simulation | ⚠️ brak nadawcy w ~/solar |
| `/wamv/sensors/gps/gps/fix` | `sensor_msgs/msg/NavSatFix` | — | [[cpp_nodes]]/imu_odometry_node (odo_cpp_node)<br>[[gps_to_course_converter]]/GpsToCourseConverter | ⚠️ brak nadawcy w ~/solar |
| `/wamv/sensors/imu/imu/data` | `sensor_msgs/msg/Imu` | — | [[course_imu]]/CourseImuConverter<br>[[cpp_nodes]]/imu_odometry_node (odo_cpp_node)<br>[[imu_rotator]]/imu_rotator<br>[[imu_to_yaw]]/imu_to_yaw | ⚠️ brak nadawcy w ~/solar |
| `/wamv/sensors/imu/imu/data_rotated` | `sensor_msgs/msg/Imu` | [[imu_rotator]]/imu_rotator | — | ⚠️ nikt nie słucha |
| `/wamv/thrusters/left/thrust` | `std_msgs/msg/Float64` | [[cpp_nodes]]/vel_node (vel_to_thrusters)<br>[[sim_can_adapter]]/SimCanAdapter<br>[[sim_thrust_driver]]/SimThrustDriver<br>[[test_thrust]]/test_thrust_node | — | ⚠️ nikt nie słucha |
| `/wamv/thrusters/right/thrust` | `std_msgs/msg/Float64` | [[cpp_nodes]]/vel_node (vel_to_thrusters)<br>[[sim_can_adapter]]/SimCanAdapter<br>[[sim_thrust_driver]]/SimThrustDriver<br>[[test_thrust]]/test_thrust_node | — | ⚠️ nikt nie słucha |
| `/which_map` | `std_msgs/msg/Bool` | [[basic_controller]]/my_bt_node (+ węzły BT) | [[planner]]/planner |  |
| `/wp_distance` | `std_msgs/msg/Float32` | [[trajectory_planner]]/TrajectoryPlannerNode | — | ⚠️ nikt nie słucha |
| `/wp_reached` | `std_msgs/msg/Bool` | [[planner]]/planner | [[basic_controller]]/my_bt_node (+ węzły BT) |  |
| `dbscan_viz` | `sensor_msgs/msg/PointCloud2` | [[obstacle_detector]]/minimal_subscriber | — | ⚠️ nikt nie słucha |
| `dongle_statistic/traffic_stats` | `TrafficStats (wg README)` | [[dongle_statistic]]/dongle_statistic_node | — | ⚠️ nikt nie słucha |
| `w kodzie wzgledne 'dongle_statistic/traffic_stats'` | `dongle_statistic/msg/TrafficStats` | [[dongle_statistic]]/dongle_statistic_node | — | ⚠️ nikt nie słucha |
| `w kodzie wzgledne 'from_can_bus'` | `can_msgs/msg/Frame` | [[ros2_socketcan]]/socket_can_receiver | — | ⚠️ nikt nie słucha |
| `w kodzie wzgledne 'to_can_bus'` | `can_msgs/msg/Frame` | — | [[ros2_socketcan]]/socket_can_sender | ⚠️ brak nadawcy w ~/solar |
| `wamv/sensors/lidars/lidar_wamv_sensor/points` | `sensor_msgs/msg/PointCloud2` | — | [[obstacle_detector]]/minimal_subscriber | ⚠️ brak nadawcy w ~/solar |

## Serwisy i akcje

| Nazwa | Typ | Serwer | Klient |
|---|---|---|---|
| `/srv/cannon_control` | `l0_interfaces/srv/CannonControl` | [[peripherals_handler]]/PeripheralsHandler | [[basic_controller]]/my_bt_node (+ węzły BT)<br>[[waypoint_sequencer]]/waypoint_sequencer_node |
| `/srv/make_detections` | `l1_interfaces/srv/MakeDetectionsParams` | [[images_collector]]/images_collector | — |
| `/srv/make_video` | `l1_interfaces/srv/MakeVideoParams` | [[images_collector]]/images_collector | — |
| `/srv/pump_control` | `l0_interfaces/srv/PumpControl` | [[peripherals_handler]]/PeripheralsHandler | [[basic_controller]]/my_bt_node (+ węzły BT)<br>[[waypoint_sequencer]]/waypoint_sequencer_node |
| `/srv/remove_wp` | `std_srvs/srv/Empty` | [[trajectory_planner]]/TrajectoryPlannerNode | — |
| `/srv/save_images` | `l1_interfaces/srv/SaveImagesParams` | [[images_collector]]/images_collector | — |
| `/srv/set_home_boat_curr_position` | `std_srvs/srv/Trigger` | [[gps_calculator]]/GpsConverter | — |
| `/srv/set_home_keyboard_input` | `l2_interfaces/srv/SetHome` | [[gps_calculator]]/GpsConverter | — |
| `/srv/waypoint_navigation_state` | `l2_interfaces/srv/WaypointNavigationState` | [[trajectory_planner]]/TrajectoryPlannerNode | [[basic_controller]]/my_bt_node (+ węzły BT)<br>[[waypoint_sequencer]]/waypoint_sequencer_node |
| `/srv/zed_exposure_mode` | `l1_interfaces/srv/RequestCMD` | [[zed_controller]]/zed_exposure_controller | — |
| `/zed/zed_node/set_parameters` | `rcl_interfaces/srv/SetParameters` | [[zed-ros2-wrapper-humble-v5.1.0]]//zed/zed_node | [[zed_controller]]/zed_exposure_controller |

## Do sprawdzenia (automatycznie wykryte niezgodności)

102 topików. Część to normalne wejścia z zewnątrz (driver Xsens, Hesai, ZED) albo wyjścia do wizualizacji/rosbag — reszta to potencjalne błędy połączeń (literówki w nazwach, martwe gałęzie kodu).

| Topik | Problem | Typy |
|---|---|---|
| `/from_can_bus` | ⚠️ brak nadawcy w ~/solar | can_msgs/msg/Frame |
| `/filter/positionlla` | ⚠️ brak nadawcy w ~/solar | geometry_msgs/msg/Vector3Stamped |
| `/filter/velocity` | ⚠️ brak nadawcy w ~/solar | geometry_msgs/msg/Vector3Stamped |
| `/filtered_cloud` | ⚠️ nikt nie słucha | sensor_msgs/msg/PointCloud2 |
| `/imu/data` | ⚠️ brak nadawcy w ~/solar, ⚠️ różne typy | sensor_msgs/msg/Imu, std_msgs/msg/Float32 |
| `/l1/detection_buoy_detector_simulation` | ⚠️ nikt nie słucha | l1_interfaces/msg/VectorOfDetections |
| `/l1/visualization/bb_320_compressed_buoy_detector_simulation` | ⚠️ nikt nie słucha | sensor_msgs/msg/Image |
| `/slam/boat_marker` | ⚠️ nikt nie słucha | visualization_msgs/msg/Marker |
| `/slam/debug/candidate_lidar_pose` | ⚠️ nikt nie słucha | nav_msgs/msg/Odometry |
| `/slam/debug/candidate_scan_map` | ⚠️ nikt nie słucha | sensor_msgs/msg/PointCloud2 |
| `/slam/debug/deskewed_scan` | ⚠️ nikt nie słucha | sensor_msgs/msg/PointCloud2 |
| `/slam/debug/frame_axes` | ⚠️ nikt nie słucha | visualization_msgs/msg/Marker |
| `/slam/debug/map_to_odom` | ⚠️ nikt nie słucha | nav_msgs/msg/Odometry |
| `/slam/debug/predicted_scan_map` | ⚠️ nikt nie słucha | sensor_msgs/msg/PointCloud2 |
| `/slam/debug/raw_scan` | ⚠️ nikt nie słucha | sensor_msgs/msg/PointCloud2 |
| `/slam/dock_polygon_map` | ⚠️ nikt nie słucha | geometry_msgs/msg/PolygonStamped |
| `/slam/global_map_points` | ⚠️ nikt nie słucha | sensor_msgs/msg/PointCloud2 |
| `/slam/odom_marker` | ⚠️ nikt nie słucha | visualization_msgs/msg/Marker |
| `/slam/tracked_detections_raw` | ⚠️ brak nadawcy w ~/solar | l2_interfaces/msg/TrackedDetectionsVector |
| `/l3/pause_wp_driver` | ⚠️ brak nadawcy w ~/solar | std_msgs/msg/Bool |
| `/lidar_camera_projection` | ⚠️ nikt nie słucha | sensor_msgs/msg/Image |
| `/lidar_points` | ⚠️ brak nadawcy w ~/solar | sensor_msgs/msg/PointCloud2 |
| `/zed/zed_node/left/image_rect_color` | ⚠️ brak nadawcy w ~/solar | sensor_msgs/msg/Image |
| `/zed/zed_node/rgb/color/rect/camera_info` | ⚠️ nikt nie słucha | sensor_msgs/msg/CameraInfo |
| `/zed_exposure/diag` | ⚠️ nikt nie słucha | std_msgs/msg/Float32MultiArray |
| `/bow_thrusters` | ⚠️ nikt nie słucha | std_msgs/msg/Int32 |
| `/bt_tui_state` | ⚠️ różne typy | (auto), l3_interfaces/msg/TuiState |
| `/canonical/fused_detection` | ⚠️ nikt nie słucha | l1_interfaces/msg/VectorOfFusedDetections |
| `/canonical/fused_detection_map` | ⚠️ nikt nie słucha | l1_interfaces/msg/VectorOfFusedDetections |
| `/canonical/obstacle_detection` | ⚠️ nikt nie słucha | l1_interfaces/msg/VectorOfClusters |
| `/canonical/odom` | ⚠️ nikt nie słucha | nav_msgs/msg/Odometry |
| `/celka/temporary/transition` | ⚠️ nikt nie słucha | std_msgs/msg/Bool |
| `/character_points` | ⚠️ nikt nie słucha | visualization_msgs/msg/MarkerArray |
| `/clock` | ⚠️ brak nadawcy w ~/solar | rosgraph_msgs/msg/Clock |
| `/course/data_sim` | ⚠️ nikt nie słucha | std_msgs/msg/Float32 |
| `/dashboard/lidar_points` | ⚠️ nikt nie słucha | sensor_msgs/msg/PointCloud2 |
| `/debug/deskewed` | ⚠️ brak nadawcy w ~/solar | sensor_msgs/msg/PointCloud2 |
| `/debug_points` | ⚠️ nikt nie słucha | visualization_msgs/msg/MarkerArray |
| `/detected_signals` | ⚠️ różne typy | l2_interfaces/msg/BlastDetection, std_msgs/msg/String |
| `/deuse_detections` | ⚠️ brak nadawcy w ~/solar | std_msgs/msg/Bool |
| `/dock/side` | ⚠️ brak nadawcy w ~/solar | std_msgs/msg/String |
| `/dock/slip` | ⚠️ brak nadawcy w ~/solar | std_msgs/msg/Float32 |
| `/dock_marker` | ⚠️ nikt nie słucha | visualization_msgs/msg/Marker |
| `/features/debris` | ⚠️ nikt nie słucha | sensor_msgs/msg/PointCloud2 |
| `/from_can_bus_fd` | ⚠️ nikt nie słucha | ros2_socketcan_msgs/msg/FdFrame |
| `/fusion_projection` | ⚠️ nikt nie słucha | sensor_msgs/msg/Image |
| `/fusion_visualization` | ⚠️ nikt nie słucha | visualization_msgs/msg/MarkerArray |
| `/global_map` | ⚠️ nikt nie słucha | grid_map_msgs/msg/GridMap |
| `/gnss` | ⚠️ brak nadawcy w ~/solar | sensor_msgs/msg/NavSatFix |
| `/gnss_pose` | ⚠️ brak nadawcy w ~/solar | geometry_msgs/msg/PoseStamped |
| `/gps/metric_position` | ⚠️ nikt nie słucha | l2_interfaces/msg/MetricPosition |
| `/gps/nav_sat_fix` | ⚠️ brak nadawcy w ~/solar | sensor_msgs/msg/NavSatFix |
| `/ground_cloud` | ⚠️ nikt nie słucha | sensor_msgs/msg/PointCloud2 |
| `/gui_waypoint_reached` | ⚠️ nikt nie słucha | geometry_msgs/msg/Point |
| `/hazard_buoy` | ⚠️ nikt nie słucha | geometry_msgs/msg/PointStamped |
| `/invalid_waypoints` | ⚠️ nikt nie słucha | geometry_msgs/msg/PoseArray |
| `/local_map` | ⚠️ nikt nie słucha | grid_map_msgs/msg/GridMap |
| `/local_voxel_map` | ⚠️ nikt nie słucha | sensor_msgs/msg/PointCloud2 |
| `/map_clusters` | ⚠️ nikt nie słucha | visualization_msgs/msg/MarkerArray |
| `/map_detections_2` | ⚠️ nikt nie słucha | visualization_msgs/msg/MarkerArray |
| `/marina_possible_entry` | ⚠️ nikt nie słucha | geometry_msgs/msg/PoseArray |
| `/marina_sides` | ⚠️ nikt nie słucha | l3_interfaces/msg/MarinaSides |
| `/melody` | ⚠️ nikt nie słucha | std_msgs/msg/Int16 |
| `/obstacle_markers` | ⚠️ nikt nie słucha | visualization_msgs/msg/MarkerArray |
| `/path_2` | ⚠️ nikt nie słucha | nav_msgs/msg/Path |
| `/path_markers` | ⚠️ nikt nie słucha | visualization_msgs/msg/MarkerArray |
| `/peripheral` | ⚠️ nikt nie słucha | std_msgs/msg/Float32MultiArray |
| `/pick_dock` | ⚠️ brak nadawcy w ~/solar | ? |
| `/protobuf_message` | ⚠️ nikt nie słucha | std_msgs/msg/UInt8MultiArray |
| `/rc_chan` | ⚠️ nikt nie słucha | std_msgs/msg/Float32MultiArray |
| `/robot/speed_challenge` | ⚠️ brak nadawcy w ~/solar, ⚠️ różne typy | ?, SpeedReport |
| `/scan_high` | ⚠️ brak nadawcy w ~/solar | sensor_msgs/msg/LaserScan |
| `/scan_prerotated` | ⚠️ brak nadawcy w ~/solar | sensor_msgs/msg/LaserScan |
| `/scanchuj` | ⚠️ brak nadawcy w ~/solar | sensor_msgs/msg/LaserScan |
| `/sim/imu/course` | ⚠️ nikt nie słucha | std_msgs/msg/Float32 |
| `/sim/imu/velocity` | ⚠️ nikt nie słucha | std_msgs/msg/Float32 |
| `/sound_signal` | ⚠️ brak nadawcy w ~/solar | std_msgs/msg/String |
| `/spatial_points` | ⚠️ brak nadawcy w ~/solar | visualization_msgs/msg/MarkerArray |
| `/status` | ⚠️ brak nadawcy w ~/solar | xsens_mti_ros2_driver/msg/XsStatusWord |
| `/survivor_buoy` | ⚠️ nikt nie słucha | geometry_msgs/msg/PointStamped |
| `/temporary/transition` | ⚠️ nikt nie słucha | std_msgs/msg/Bool |
| `/thrusters/setpoints` | ⚠️ nikt nie słucha | std_msgs/msg/Float32MultiArray |
| `/to_can_bus_fd` | ⚠️ brak nadawcy w ~/solar | ros2_socketcan_msgs/msg/FdFrame |
| `/tracking_detection` | ⚠️ nikt nie słucha | l1_interfaces/msg/VectorOfFusedDetections |
| `/tracking_visualization` | ⚠️ nikt nie słucha | visualization_msgs/msg/MarkerArray |
| `/true_heading/data` | ⚠️ brak nadawcy w ~/solar | l0_interfaces/msg/Course |
| `/tunnel_marker` | ⚠️ nikt nie słucha | visualization_msgs/msg/Marker |
| `/used_buoy_id` | ⚠️ nikt nie słucha | std_msgs/msg/Int32 |
| `/wamv/sensors/cameras/front_left_camera_sensor/image_raw` | ⚠️ brak nadawcy w ~/solar | sensor_msgs/msg/Image |
| `/wamv/sensors/cameras/front_left_camera_sensor/points` | ⚠️ brak nadawcy w ~/solar | sensor_msgs/msg/PointCloud2 |
| `/wamv/sensors/gps/gps/fix` | ⚠️ brak nadawcy w ~/solar | sensor_msgs/msg/NavSatFix |
| `/wamv/sensors/imu/imu/data` | ⚠️ brak nadawcy w ~/solar | sensor_msgs/msg/Imu |
| `/wamv/sensors/imu/imu/data_rotated` | ⚠️ nikt nie słucha | sensor_msgs/msg/Imu |
| `/wamv/thrusters/left/thrust` | ⚠️ nikt nie słucha | std_msgs/msg/Float64 |
| `/wamv/thrusters/right/thrust` | ⚠️ nikt nie słucha | std_msgs/msg/Float64 |
| `/wp_distance` | ⚠️ nikt nie słucha | std_msgs/msg/Float32 |
| `dbscan_viz` | ⚠️ nikt nie słucha | sensor_msgs/msg/PointCloud2 |
| `dongle_statistic/traffic_stats` | ⚠️ nikt nie słucha | TrafficStats (wg README) |
| `w kodzie wzgledne 'dongle_statistic/traffic_stats'` | ⚠️ nikt nie słucha | dongle_statistic/msg/TrafficStats |
| `w kodzie wzgledne 'from_can_bus'` | ⚠️ nikt nie słucha | can_msgs/msg/Frame |
| `w kodzie wzgledne 'to_can_bus'` | ⚠️ brak nadawcy w ~/solar | can_msgs/msg/Frame |
| `wamv/sensors/lidars/lidar_wamv_sensor/points` | ⚠️ brak nadawcy w ~/solar | sensor_msgs/msg/PointCloud2 |

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
