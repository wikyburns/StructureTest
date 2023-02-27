/*
    This file is part of the Structure SDK.
    Copyright © 2022 XRPro LLC. All rights reserved.
    http://structure.io
*/

#pragma once

#import <Structure/StructureBase.h>
#import <Structure/STDepthFrame.h>
#import <Structure/STNormalFrame.h>

#pragma mark - STNormalEstimator API

/** Helper class to estimate surface normals.

STNormalEstimator instances calculate a unit vector representing the surface normals for each depth pixel.

See also:

- STNormalFrame
*/
@interface STNormalEstimator : NSObject

/** Calculate normals with a depth frame. Pixels without depth will have NaN values.

@param floatDepthFrame The depth frame.

@return A STNormalFrame normal frame object.
*/
- (STNormalFrame*)calculateNormalsWithDepthFrame:(STDepthFrame*)floatDepthFrame;

@end
