/*
    This file is part of the Structure SDK.
    Copyright © 2022 XRPro LLC. All rights reserved.
    http://structure.io
*/

#pragma once


#import <Structure/STBackgroundTask.h>
#import <Structure/STMesh+Types.h>

#import <GLKit/GLKMatrix4.h>

#import <ARKit/ARKit.h>

#pragma mark - STMesh API

/** Reference to face-vertex triangle mesh data.

 Stores mesh data as a collection of vertices and faces. STMesh objects are references, and access to the underlying
 data should be protected by locks in case multiple threads may be accessing it.

 Since OpenGL ES only supports 16 bits unsigned short for face indices, meshes larger than 65535 faces have to be split
 into smaller sub-meshes. STMesh is therefore a reference to a collection of partial meshes, each of them having less
 than 65k faces.
*/
@interface STMesh : NSObject

/// Number of partial meshes.
- (int)numberOfMeshes;

/** Number of faces of a given submesh.

 @param meshIndex Index to the partial mesh.
*/
- (int)numberOfMeshFaces:(int)meshIndex;

/** Number of vertices of a given submesh.

 @param meshIndex Index to the partial mesh.
*/
- (int)numberOfMeshVertices:(int)meshIndex;

/** Number of lines (edges) of a given submesh.

 @param meshIndex Index to the partial mesh.
*/
- (int)numberOfMeshLines:(int)meshIndex;

/// Whether per-vertex normals are available.
- (BOOL)hasPerVertexNormals;

/// Whether per-vertex colors are available.
- (BOOL)hasPerVertexColors;

/// Whether per-vertex UV texture coordinates are available.
- (BOOL)hasPerVertexUVTextureCoords;

/** Pointer to a contiguous chunk of `numberOfMeshVertices:meshIndex` `GLKVector3` values representing (x, y, z) vertex
 coordinates.

 @param meshIndex Index to the partial mesh.
*/
- (GLKVector3*)meshVertices:(int)meshIndex;

/** Pointer to a contiguous chunk of `numberOfMeshVertices:meshIndex` `GLKVector3` values representing (nx, ny, nz)
 per-vertex normals.

 @note Returns `nullptr` is there are no per-vertex normals.

 @param meshIndex Index to the partial mesh.
*/
- (GLKVector3*)meshPerVertexNormals:(int)meshIndex;

/** Pointer to a contiguous chunk of `numberOfMeshVertices:meshIndex` `GLKVector3` values representing (r, g, b)
 vertices colors.

 @note Returns `nullptr` is there are no per-vertex colors.

 @param meshIndex Index to the partial mesh.
*/
- (GLKVector3*)meshPerVertexColors:(int)meshIndex;

/** Pointer to a contiguous chunk of `numberOfMeshVertices:meshIndex` `GLKVector2` values representing normalized (u, v)
 texture coordinates.

 @note Returns `nullptr` is there are no per-vertex texture coordinates.

 @param meshIndex Index to the partial mesh.
*/
- (GLKVector2*)meshPerVertexUVTextureCoords:(int)meshIndex;

/** Pointer to a contiguous chunk of `(3 * numberOfMeshFaces:meshIndex)` 16 bits `unsigned short` values representing
 vertex indices. Each face is represented by three vertex indices.

 @param meshIndex Index to the partial mesh.
*/
- (unsigned int*)meshFaces:(int)meshIndex;

/** Optional texture associated with the mesh.

 The pixel buffer is encoded using `kCVPixelFormatType_420YpCbCr8BiPlanarFullRange`.
*/
- (CVPixelBufferRef)meshYCbCrTexture;

/** Pointer to a contiguous chunk of `(2 * numberOfMeshLines:meshIndex)` 16 bits `unsigned short` values representing
 vertex indices.

 Each line is represented by two vertex indices. These lines can be used for wireframe rendering, using GL_LINES.

 @param meshIndex Index to the partial mesh.
*/
- (unsigned int*)meshLines:(int)meshIndex;

/** Applies transformation to the mesh
 @param transform  The transformation matrix.
*/
- (void)applyTransform:(GLKMatrix4)transform;

/** Save the STMesh to a file.

 Sample usage:

    NSError* error;
    [myMesh writeToFile:@"/path/to/mesh.obj"
                options:@{kSTMeshWriteOptionFileFormatKey: STMeshWriteOptionFileFormatObjFile}
                  error:&error];

 @param filePath Path to output file.
 @param options Dictionary of options. The valid keys are:

 - `kSTMeshWriteOptionFileFormatKey`: STMeshWriteOptionFileFormat value to specify the output file format. Required.
 - `kSTMeshWriteOptionUseXRightYUpConventionKey`: Sets the exported mesh coordinate frame to be X right, Y Up, and Z
 inwards (right handed).
 - `kSTMeshWriteOptionUseScaleKey`: Adjusts the exported mesh by given scale, such as allowing for centimeter scale
 export.

 @param error will contain detailed information if the provided options are incorrect.
*/
- (BOOL)writeToFile:(NSString*)filePath options:(NSDictionary*)options error:(NSError* __autoreleasing*)error;

/** Create a copy of the current mesh.

 @param mesh The mesh from which to copy.
*/
- (instancetype)initWithMesh:(STMesh*)mesh;

/** Create an STMesh from an existing, populated ARSession.

 @param session The ARSession to copy data from.
 @param completionBlock Block function to return the retrieved and allocated mesh.
 */
+ (void)meshFromARSession:(ARSession*)session
          completionBlock:(void (^)(STMesh*))completionBlock API_AVAILABLE(ios(13.4));

/** Return an asynchronous task to create a decimated low-poly version of the given mesh with a maximal target number of
faces.

Sample usage:

    __block STMesh* outputMesh;
    STBackgroundTask* task;
    task = [STMesh newDecimateTaskWithMesh:myMesh
                                  numFaces:2000
                         completionHandler:^(STMesh *result, NSError *error) { outputMesh = result; };
    [task start];
    [task waitUntilCompletion];

 @note If the target number of faces is larger than the current mesh number of faces, no processing is done.

 @param inputMesh Input mesh to decimate.
 @param numFaces Target number of faces to decimate.
 @param completionHandler Block to execute once the task is finished or cancelled.
*/
+ (STBackgroundTask*)newDecimateTaskWithMesh:(STMesh*)inputMesh
                                    numFaces:(unsigned int)numFaces
                           completionHandler:(void (^)(STMesh* result, NSError* error))completionHandler;

/** Return an asynchronous task to create a version of the given mesh with holes filled.

 @note Additionally, the output will result in a smoother mesh, with non-manifold faces removed.

 @param inputMesh Input mesh to fill holes.
 @param completionHandler Block to execute once the task is finished or cancelled.
*/
+ (STBackgroundTask*)newFillHolesTaskWithMesh:(STMesh*)inputMesh
                            completionHandler:(void (^)(STMesh* result, NSError* error))completionHandler;


/** Return an asynchronous task to create a version of the given mesh with holes filled with the Poisson algorithm.

 @note Additionally, the output will result in a smoother mesh, with non-manifold faces removed.

 @param inputMesh Input mesh to fill holes.
 @param algorithm The hole-filling algorithm.
 @param options Dictionary of options. The valid keys are:

 - `kSTMeshFillHoleMaxPatchAreaKey`: The maximum area of a hole to be filled for the Liepa algorithm in the current
 units (e.g. square meters). 1.f by default.
 - `kSTMeshFillHolePoissonStrategyKey`: STMeshFillHolePoissonStrategy value to specify the strategy of the Poisson
 algorithm. Watertight by default.

 @param completionHandler Block to execute once the task is finished or cancelled.
*/
+ (STBackgroundTask*)newFillHolesTaskWithMesh:(STMesh*)inputMesh
                                    algorithm:(STMeshFillHoleAlgorithm)algorithm
                                      options:(NSDictionary*)options
                            completionHandler:(void (^)(STMesh* result, NSError* error))completionHandler;

@end
