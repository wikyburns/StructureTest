/*
    This file is part of the Structure SDK.
    Copyright © 2022 XRPro LLC. All rights reserved.
    http://structure.io
*/

#pragma once

#import <Structure/StructureBase.h>
#import <Structure/STDepthFrame.h>
#import <Structure/STTracker+Types.h>
#import <Structure/STScene.h>

#import <CoreMotion/CoreMotion.h>

#pragma mark - STSLAMManager API

/** An STSLAMManager instance maps and tracks the 3D position and data of the Structure Sensor.

Using STSLAMManager does not require the use of an STMapper object.

*/
@interface STSLAMManager : NSObject

/// STScene object storing common SLAM information.
@property(nonatomic, retain) STScene* scene;

/// The initial camera pose. Tracking will use this as the first frame pose.
@property(nonatomic) GLKMatrix4 initialCameraPose;

/** The current tracker hints.

 See also:
 - STTrackerHints
 - [STTracker poseAccuracy]
*/
@property(nonatomic, readonly) STTrackerHints trackerHints;

/** The current tracker pose accuracy.

 See also:
 - STTrackerPoseAccuracy
 - [STTracker trackerHints]
*/
@property(nonatomic, readonly) STTrackerPoseAccuracy poseAccuracy;

/** STSLAMManager initialization.

 STSLAMManager cannot be used until an STScene has been provided.

 Sample usage:

    NSDictionary* trackerOptions = @{
        kSTMapperVolumeResolutionKey: @(0.003),
        kSTMapperVolumeBoundsKey: @[
                @(_slamState.volumeSizeInMeters.x),
                @(_slamState.volumeSizeInMeters.y),
                @(_slamState.volumeSizeInMeters.z)
        ],
    };
    STSLAMManager* manager = [[STSLAMManager alloc] initWithScene:myScene options:options];

 @param scene The STScene context.
 @param options Dictionary of options. The valid keys are:

 - `kSTMapperVolumeResolutionKey`:
  - Specifies the volume resolution as the size of each voxel in meters.
  - `NSNumber` floating point value.
  - Required.
 - `kSTMapperVolumeBoundsKey`:
  - The extents of the bounding volume in meters along each dimension.
  - `NSArray` of 3 floating point values.
  - Defaults to `@[ @(0.0), @(0.0), @(0.0) ]`.
*/
- (instancetype)initWithScene:(STScene*)scene options:(NSDictionary*)options;

/// Reset the tracker and mapper state. This will also stop any background processing.
- (void)reset;

/** Update the camera pose estimate and integrate data into the model using the given depth frame.

 @param depthFrame The STDepthFrame depth frame.
 @param colorFrame The STColorFrame color frame from iOS.
 @param error On return, if it fails, points to an NSError describing the failure.

 @return TRUE if success, FALSE otherwise, filling error with the explanation.
*/
- (BOOL)updateCameraPoseWithDepthFrame:(STDepthFrame*)depthFrame
                            colorFrame:(STColorFrame*)colorFrame
                                 error:(NSError* __autoreleasing*)error;

/** Update the current pose estimates using the provided motion data.

 @param motionData Provided motion data.
*/
- (void)updateCameraPoseWithMotion:(CMDeviceMotion*)motionData;

/** Update the current pose estimates with the raw gyroscope data.

 @param motionData Provided raw gyroscope data.
*/
- (void)updateCameraPoseWithGyro:(CMGyroData*)motionData;

/** Update the current pose estimates with the raw accelerometer data.

 @param motionData Provided raw accelerometer data.
*/
- (void)updateCameraPoseWithAccel:(CMAccelerometerData*)motionData;

/// Return the most recent camera pose estimate.
- (GLKMatrix4)lastFrameCameraPose;

/// Wait until ongoing processing in the background queue finishes, and build the final triangle mesh.
- (void)finalizeTriangleMesh;

/// Merges all bundles together (slow), but less accurate than the final mesh
- (STMesh*)currentMesh;

/// Mesh from the single current bundle, works significantly faster than access to the whole merged mesh.
- (STMesh*)currentBundleMesh;

@end
