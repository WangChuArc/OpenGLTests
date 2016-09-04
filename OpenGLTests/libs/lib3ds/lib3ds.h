/* -*- c -*- */
#ifndef INCLUDED_LIB3DS_H
#define INCLUDED_LIB3DS_H
/*
    Copyright (C) 1996-2008 by Jan Eric Kyprianidis <www.kyprianidis.com>
    All rights reserved.
    
    This program is free  software: you can redistribute it and/or modify 
    it under the terms of the GNU Lesser General Public License as published 
    by the Free Software Foundation, either version 2.1 of the License, or 
    (at your option) any later version.

    Thisprogram  is  distributed in the hope that it will be useful, 
    but WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
    GNU Lesser General Public License for more details.
    
    You should  have received a copy of the GNU Lesser General Public License
    along with  this program; If not, see <http://www.gnu.org/licenses/>. 
 */

#include <stddef.h>

#ifndef LIB3DSAPI
    #ifdef _MSC_VER
        #ifdef LIB3DS_EXPORTS
            #define LIB3DSAPI __declspec(dllexport)
        #else               
            #define LIB3DSAPI __declspec(dllimport)
        #endif           
    #else
        #define LIB3DSAPI
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup api API */
/** @{ */

#include "lib3ds_types.h"

extern LIB3DSAPI Lib3dsFile* lib3ds_file_open(const char *filename);
extern LIB3DSAPI int lib3ds_file_save(Lib3dsFile *file, const char *filename);
extern LIB3DSAPI Lib3dsFile* lib3ds_file_new();
extern LIB3DSAPI void lib3ds_file_free(Lib3dsFile *file);
extern LIB3DSAPI void lib3ds_file_eval(Lib3dsFile *file, float t);
extern LIB3DSAPI int lib3ds_file_read(Lib3dsFile *file, Lib3dsIo *io);
extern LIB3DSAPI int lib3ds_file_write(Lib3dsFile *file, Lib3dsIo *io);
extern LIB3DSAPI void lib3ds_file_reserve_materials(Lib3dsFile *file, int size, int force);
extern LIB3DSAPI void lib3ds_file_insert_material(Lib3dsFile *file, Lib3dsMaterial *material, int index);
extern LIB3DSAPI void lib3ds_file_remove_material(Lib3dsFile *file, int index);
extern LIB3DSAPI int lib3ds_file_material_by_name(Lib3dsFile *file, const char *name);
extern LIB3DSAPI void lib3ds_file_reserve_cameras(Lib3dsFile *file, int size, int force);
extern LIB3DSAPI void lib3ds_file_insert_camera(Lib3dsFile *file, Lib3dsCamera *camera, int index);
extern LIB3DSAPI void lib3ds_file_remove_camera(Lib3dsFile *file, int index);
extern LIB3DSAPI int lib3ds_file_camera_by_name(Lib3dsFile *file, const char *name);
extern LIB3DSAPI void lib3ds_file_reserve_lights(Lib3dsFile *file, int size, int force);
extern LIB3DSAPI void lib3ds_file_insert_light(Lib3dsFile *file, Lib3dsLight *light, int index);
extern LIB3DSAPI void lib3ds_file_remove_light(Lib3dsFile *file, int index);
extern LIB3DSAPI int lib3ds_file_light_by_name(Lib3dsFile *file, const char *name);
extern LIB3DSAPI void lib3ds_file_reserve_meshes(Lib3dsFile *file, int size, int force);
extern LIB3DSAPI void lib3ds_file_insert_mesh(Lib3dsFile *file, Lib3dsMesh *mesh, int index);
extern LIB3DSAPI void lib3ds_file_remove_mesh(Lib3dsFile *file, int index);
extern LIB3DSAPI int lib3ds_file_mesh_by_name(Lib3dsFile *file, const char *name);
extern LIB3DSAPI Lib3dsMesh* lib3ds_file_mesh_for_node(Lib3dsFile *file, Lib3dsNode *node);
extern LIB3DSAPI Lib3dsNode* lib3ds_file_node_by_name(Lib3dsFile *file, const char* name, Lib3dsNodeType type);
extern LIB3DSAPI Lib3dsNode* lib3ds_file_node_by_id(Lib3dsFile *file, unsigned short node_id);
extern LIB3DSAPI void lib3ds_file_append_node(Lib3dsFile *file, Lib3dsNode *node, Lib3dsNode *parent);
extern LIB3DSAPI void lib3ds_file_insert_node(Lib3dsFile *file, Lib3dsNode *node, Lib3dsNode *at);
extern LIB3DSAPI void lib3ds_file_remove_node(Lib3dsFile *file, Lib3dsNode *node);
extern LIB3DSAPI void lib3ds_file_minmax_node_id(Lib3dsFile *file, unsigned short *min_id, unsigned short *max_id);
extern LIB3DSAPI void lib3ds_file_create_nodes_for_meshes(Lib3dsFile *file);


/**
    This function computes the bounding box of meshes, cameras 
    and lights defined in the 3D editor.

    \param file             The Lib3dsFile object to be examined.
    \param include_meshes   Include meshes in bounding box calculation.
    \param include_cameras  Include cameras in bounding box calculation.
    \param include_lights   Include lights in bounding box calculation.
    \param bmin             Returned minimum x,y,z values.
    \param bmax             Returned maximum x,y,z values.
 */
extern LIB3DSAPI void lib3ds_file_bounding_box_of_objects(
    Lib3dsFile *file, 
    int include_meshes, 
    int include_cameras,
    int include_lights,
    float bmin[3],
    float bmax[3]);

/**
    This function computes the bounding box of mesh, camera 
    and light instances defined in the Keyframer.
    
    \param file             The Lib3dsFile object to be examined.
    \param include_meshes   Include meshes in bounding box calculation.
    \param include_cameras  Include cameras in bounding box calculation.
    \param include_lights   Include lights in bounding box calculation.
    \param bmin             Returned minimum x,y,z values.
    \param bmax             Returned maximum x,y,z values.
    \param matrix           A matrix describing the coordinate system to
                            calculate the bounding box in.
 */
extern LIB3DSAPI void lib3ds_file_bounding_box_of_nodes(
    Lib3dsFile *file, 
    int include_meshes, 
    int include_cameras, 
    int include_lights, 
    float bmin[3], 
    float bmax[3], 
    float matrix[4][4]);

extern LIB3DSAPI Lib3dsMaterial* lib3ds_material_new(const char *name);
extern LIB3DSAPI void lib3ds_material_free(Lib3dsMaterial *material);
extern LIB3DSAPI Lib3dsCamera* lib3ds_camera_new(const char *name);
extern LIB3DSAPI void lib3ds_camera_free(Lib3dsCamera *mesh);
extern LIB3DSAPI Lib3dsLight* lib3ds_light_new(const char *name);
extern LIB3DSAPI void lib3ds_light_free(Lib3dsLight *mesh);
extern LIB3DSAPI Lib3dsMesh* lib3ds_mesh_new(const char *name);
extern LIB3DSAPI void lib3ds_mesh_free(Lib3dsMesh *mesh);
extern LIB3DSAPI void lib3ds_mesh_resize_vertices(Lib3dsMesh *mesh, int nvertices, int use_texcos, int use_flags);
extern LIB3DSAPI void lib3ds_mesh_resize_faces(Lib3dsMesh *mesh, int nfaces);
extern LIB3DSAPI void lib3ds_mesh_bounding_box(Lib3dsMesh *mesh, float bmin[3], float bmax[3]);
extern LIB3DSAPI void lib3ds_mesh_calculate_face_normals(Lib3dsMesh *mesh, float (*face_normals)[3]);
extern LIB3DSAPI void lib3ds_mesh_calculate_vertex_normals(Lib3dsMesh *mesh, float (*normals)[3]);

extern LIB3DSAPI Lib3dsNode* lib3ds_node_new(Lib3dsNodeType type);
extern LIB3DSAPI Lib3dsAmbientColorNode* lib3ds_node_new_ambient_color(float color0[3]);
extern LIB3DSAPI Lib3dsMeshInstanceNode* lib3ds_node_new_mesh_instance(Lib3dsMesh *mesh, const char* instance_name, float pos0[3], float scl0[3], float rot0[4]);
extern LIB3DSAPI Lib3dsCameraNode* lib3ds_node_new_camera(Lib3dsCamera *camera);
extern LIB3DSAPI Lib3dsTargetNode* lib3ds_node_new_camera_target(Lib3dsCamera *camera);
extern LIB3DSAPI Lib3dsOmnilightNode* lib3ds_node_new_omnilight(Lib3dsLight *light);
extern LIB3DSAPI Lib3dsSpotlightNode* lib3ds_node_new_spotlight(Lib3dsLight *light);
extern LIB3DSAPI Lib3dsTargetNode* lib3ds_node_new_spotlight_target(Lib3dsLight *light);
extern LIB3DSAPI void lib3ds_node_free(Lib3dsNode *node);
extern LIB3DSAPI void lib3ds_node_eval(Lib3dsNode *node, float t);
extern LIB3DSAPI Lib3dsNode* lib3ds_node_by_name(Lib3dsNode *node, const char* name, Lib3dsNodeType type);
extern LIB3DSAPI Lib3dsNode* lib3ds_node_by_id(Lib3dsNode *node, unsigned short node_id);

extern LIB3DSAPI Lib3dsTrack* lib3ds_track_new(Lib3dsTrackType type, int nkeys);
extern LIB3DSAPI void lib3ds_track_free(Lib3dsTrack *track);
extern LIB3DSAPI void lib3ds_track_resize(Lib3dsTrack *track, int nkeys);
extern LIB3DSAPI void lib3ds_track_eval_bool(Lib3dsTrack *track, int *b, float t);
extern LIB3DSAPI void lib3ds_track_eval_float(Lib3dsTrack *track, float *f, float t);
extern LIB3DSAPI void lib3ds_track_eval_vector(Lib3dsTrack *track, float v[3], float t);
extern LIB3DSAPI void lib3ds_track_eval_quat(Lib3dsTrack *track, float q[4], float t);

/** 
    Calculates the ease in/out function. See Lib3dsKey for details. 
    
    \param fp           Previous frame number.
    \param fc           Current frame number.
    \param fn           Next frame number. 
    \param ease_from    Ease in value [0, 1.0] 
    \param ease_to      Ease out value [0, 1.0]
*/
extern LIB3DSAPI float lib3ds_math_ease(
    float fp,
    float fc,
    float fn,
    float ease_from,
    float ease_to);

/**  
    Computes a point on a n-dimensional cubic hermite spline. 
    \param v   
        [out] Result 
    \param a
        [in] First point of the spline.
    \param p   
        [in] Tangent at the first point of the spline.
    \param q
        [in] Tangent at the second point of the spline.
    \param b
        [in] Second point of the spline.
    \param n
        [in] Dimension
    \param t
        [in] Parameter value [0...1] 
*/
extern LIB3DSAPI void lib3ds_math_cubic_interp(
    float *v, 
    float *a,
    float *p,
    float *q,
    float *b,
    int n,
    float t);

extern LIB3DSAPI void lib3ds_vector_make(
    float c[3],
    float x,
    float y,
    float z);

/** 
    Sets all components of a vector to zero. 
    \param c
        The Pointer to the vector.
*/
extern LIB3DSAPI void lib3ds_vector_zero(
    float c[3]);

/** 
    Copies all components of a vector to another vector. 
    \param dst
        [out] The destination vector.
    \param src
        [in] The source vector.
*/
extern LIB3DSAPI void lib3ds_vector_copy( 
    float dst[3],  
    float src[3]);

/** 
    Negates all components of a vector. 
    \param c
        The Pointer to the vector.
*/
extern LIB3DSAPI void lib3ds_vector_neg(
    float c[3]);

extern LIB3DSAPI void lib3ds_vector_make(float c[3], float x, float y, float z);
extern LIB3DSAPI void lib3ds_vector_zero(float c[3]);
extern LIB3DSAPI void lib3ds_vector_add(float c[3], float a[3], float b[3]);
extern LIB3DSAPI void lib3ds_vector_sub(float c[3], float a[3], float b[3]);
extern LIB3DSAPI void lib3ds_vector_scalar_mul(float c[3], float a[3], float k);
extern LIB3DSAPI void lib3ds_vector_cross(float c[3], float a[3], float b[3]);
extern LIB3DSAPI float lib3ds_vector_dot(float a[3], float b[3]);
extern LIB3DSAPI float lib3ds_vector_length(float c[3]);
extern LIB3DSAPI void lib3ds_vector_normalize(float c[3]);
extern LIB3DSAPI void lib3ds_vector_normal(float n[3], float a[3], float b[3], float c[3]);
extern LIB3DSAPI void lib3ds_vector_min(float c[3], float a[3]);
extern LIB3DSAPI void lib3ds_vector_max(float c[3], float a[3]);
extern LIB3DSAPI void lib3ds_vector_transform(float c[3], float m[4][4], float a[3]);

extern LIB3DSAPI void lib3ds_quat_identity(float c[4]);
extern LIB3DSAPI void lib3ds_quat_copy(float dest[4], float src[4]);
extern LIB3DSAPI void lib3ds_quat_axis_angle(float c[4], float axis[3], float angle);
extern LIB3DSAPI void lib3ds_quat_neg(float c[4]);
extern LIB3DSAPI void lib3ds_quat_cnj(float c[4]);
extern LIB3DSAPI void lib3ds_quat_mul(float c[4], float a[4], float b[4]);
extern LIB3DSAPI void lib3ds_quat_scalar(float c[4], float k);
extern LIB3DSAPI void lib3ds_quat_normalize(float c[4]);
extern LIB3DSAPI void lib3ds_quat_inv(float c[4]);
extern LIB3DSAPI float lib3ds_quat_dot(float a[4], float b[4]);
extern LIB3DSAPI float lib3ds_quat_norm(float c[4]);
extern LIB3DSAPI void lib3ds_quat_ln(float c[4]);
extern LIB3DSAPI void lib3ds_quat_ln_dif(float c[4], float a[4], float b[4]);
extern LIB3DSAPI void lib3ds_quat_exp(float c[4]);
extern LIB3DSAPI void lib3ds_quat_slerp(float c[4], float a[4], float b[4], float t);
extern LIB3DSAPI void lib3ds_quat_squad(float c[4], float a[4], float p[4], float q[4], float b[4], float t);
extern LIB3DSAPI void lib3ds_quat_tangent(float c[4], float p[4], float q[4], float n[4]);

extern LIB3DSAPI void lib3ds_matrix_zero(float m[4][4]);
extern LIB3DSAPI void lib3ds_matrix_identity(float  m[4][4]);
extern LIB3DSAPI void lib3ds_matrix_copy(float dest[4][4], float src[4][4]);
extern LIB3DSAPI void lib3ds_matrix_neg(float m[4][4]);
extern LIB3DSAPI void lib3ds_matrix_transpose(float m[4][4]);
extern LIB3DSAPI void lib3ds_matrix_add(float m[4][4], float a[4][4], float b[4][4]);
extern LIB3DSAPI void lib3ds_matrix_sub(float m[4][4], float a[4][4], float b[4][4]);
extern LIB3DSAPI void lib3ds_matrix_mult(float m[4][4], float a[4][4], float b[4][4]);
extern LIB3DSAPI void lib3ds_matrix_scalar(float m[4][4], float k);
extern LIB3DSAPI float lib3ds_matrix_det(float m[4][4]);
extern LIB3DSAPI int lib3ds_matrix_inv(float m[4][4]);
extern LIB3DSAPI void lib3ds_matrix_translate(float m[4][4], float x, float y, float z);
extern LIB3DSAPI void lib3ds_matrix_scale(float m[4][4], float x, float y, float z);
extern LIB3DSAPI void lib3ds_matrix_rotate_quat(float m[4][4], float q[4]);
extern LIB3DSAPI void lib3ds_matrix_rotate(float m[4][4], float angle, float ax, float ay, float az);
extern LIB3DSAPI void lib3ds_matrix_camera(float m[4][4], float pos[3], float tgt[3], float roll);

/** @} */
#ifdef __cplusplus
}
#endif
#endif

