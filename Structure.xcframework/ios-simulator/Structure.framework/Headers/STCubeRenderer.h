/*
    This file is part of the Structure SDK.
    Copyright © 2022 XRPro LLC. All rights reserved.
    http://structure.io
*/

#pragma once

#import <Structure/StructureBase.h>
#import <Structure/STDepthFrame.h>

#import <GLKit/GLKVector3.h>

#pragma mark - STCubeRender API

/** STCubeRenderer is an utility class for cuboid rendering.

STCubeRenderer can render a wireframe outline of a cube, and also highlight the part of scene which fits in the given
cube. This can be used to better visualize where the current cuboid scanning volume is located.
*/
ST_GLES_DEPRECATED
@interface STCubeRenderer : NSObject

/** Initialize with required properties.

@param glContext The EAGLContext.
*/
- (instancetype)initWithContext:(EAGLContext*)glContext;

/** A depth frame is required before using renderHighlightedDepth.

@param depthFrame The depth frame.
*/
- (void)setDepthFrame:(STDepthFrame*)depthFrame;

/** Whether the cube has a support plane. Rendering will be adjusted in that case.

@param hasSupportPlane The boolean to enable adjustment of support plane in rendering.
*/
- (void)setCubeHasSupportPlane:(BOOL)hasSupportPlane;

/** Specify the cube size.

@param sizeInMeters The current volume size in meters.
*/
- (void)adjustCubeSize:(const GLKVector3)sizeInMeters;

/** Highlight the depth frame area which fits inside the cube.

@param cameraPose the viewpoint to use for rendering.
@param alpha transparency factor between 0 (fully transparent) and 1 (fully opaque)
*/
- (void)renderHighlightedDepthWithCameraPose:(GLKMatrix4)cameraPose alpha:(float)alpha;

/**
Render the cube wireframe outline at the given pose.

@param cameraPose the viewpoint to use for rendering.
@param depthTestEnabled whether the lines should be drawn with `GL_DEPTH_TEST` enabled. This should typically be
disabled if used in combination with renderHighlightedDepthWithCameraPose: to avoid having the lines occluded, but
enabled if a mesh is also being rendered in the scene.
@param occlusionTestEnabled whether to use the current depth frame to do occlusion testing. You can turn this off for
better performance.
*/
- (void)renderCubeOutlineWithCameraPose:(GLKMatrix4)cameraPose
                       depthTestEnabled:(BOOL)depthTestEnabled
                   occlusionTestEnabled:(BOOL)occlusionTestEnabled;
/**
 Set custom colors used for depth rendering.

 @param baseColors Pointer to the array of base colors for gradient.
    Colors are specified in RGB space using float values in range [0, 1]. Outlier will be clamped to that range.
    These colors will be spaced evenly and the final color will be lineary interpolated between nearest neighbours.
 @param numColors Number of colors to read from the array.
 */
- (void)setDepthRenderingColors:(GLKVector3*)baseColors numColors:(int)numColors;

/**
 Set the orientation matrix colors for the cube rendering.

 @param orientation Affine transformation matrix.
 */
- (void)setOrientation:(GLKMatrix2)orientation;

@end
