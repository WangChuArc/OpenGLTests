/* -*- c -*- */
#ifndef INCLUDED_LIB3DS_TYPES_H
#define INCLUDED_LIB3DS_TYPES_H
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

	Modified by cheungmine 2009-2
 */

typedef enum Lib3dsIoSeek {
    LIB3DS_SEEK_SET     = 0,
    LIB3DS_SEEK_CUR     = 1,
    LIB3DS_SEEK_END     = 2
} Lib3dsIoSeek;

typedef enum Lib3dsLogLevel {
    LIB3DS_LOG_ERROR    = 0,
    LIB3DS_LOG_WARN     = 1,
    LIB3DS_LOG_INFO     = 2,
    LIB3DS_LOG_DEBUG    = 3
} Lib3dsLogLevel;

typedef struct Lib3dsIo {
    void*   impl;
    void*   self;
    long    (*seek_func) (void *self, long offset, Lib3dsIoSeek origin);
    long    (*tell_func) (void *self);
    size_t  (*read_func) (void *self, void *buffer, size_t size);
    size_t  (*write_func)(void *self, const void *buffer, size_t size);
    void    (*log_func)  (void *self, Lib3dsLogLevel level, int indent, const char *msg);
} Lib3dsIo;

/* Atmosphere settings */
typedef struct Lib3dsAtmosphere {
    int         use_fog;
    float       fog_color[3];
    int         fog_background;
    float       fog_near_plane;
    float       fog_near_density;
    float       fog_far_plane;
    float       fog_far_density;
    int         use_layer_fog;
    unsigned    layer_fog_flags;
    float       layer_fog_color[3];
    float       layer_fog_near_y;
    float       layer_fog_far_y;
    float       layer_fog_density;
    int         use_dist_cue;
    int         dist_cue_background;     /* bool */
    float       dist_cue_near_plane;
    float       dist_cue_near_dimming;
    float       dist_cue_far_plane;
    float       dist_cue_far_dimming;
} Lib3dsAtmosphere;

typedef struct Lib3dsVector {
	float   v[3];
} Lib3dsVector;

typedef struct Lib3dsMatrix {
	float   m[4][4];        /**< Transformation matrix for mesh data */
} Lib3dsMatrix;

/* Background settings */
typedef struct Lib3dsBackground {
    int         use_bitmap;
    char        bitmap_name[64];
    int         use_solid;
    float       solid_color[3];
    int         use_gradient;
    float       gradient_percent;
    float       gradient_top[3];
    float       gradient_middle[3];
    float       gradient_bottom[3];
} Lib3dsBackground;

/* Shadow settings */
typedef struct Lib3dsShadow {
    short       map_size;           /* Global shadow map size that ranges from 10 to 4096 */
    float       low_bias;           /* Global shadow low bias */
    float       hi_bias;            /* Global shadow hi bias */
    float       filter;             /* Global shadow filter that ranges from 1 (lowest) to 10 (highest) */
    float       ray_bias;           /* Global raytraced shadow bias */
} Lib3dsShadow;

/* Layout view types */
typedef enum Lib3dsViewType {
    LIB3DS_VIEW_NOT_USED   = 0,
    LIB3DS_VIEW_TOP        = 1,
    LIB3DS_VIEW_BOTTOM     = 2,
    LIB3DS_VIEW_LEFT       = 3,
    LIB3DS_VIEW_RIGHT      = 4,
    LIB3DS_VIEW_FRONT      = 5,
    LIB3DS_VIEW_BACK       = 6,
    LIB3DS_VIEW_USER       = 7,
    LIB3DS_VIEW_SPOTLIGHT  = 18,
    LIB3DS_VIEW_CAMERA     = 65535
} Lib3dsViewType;

/* Layout styles */
typedef enum Lib3dsLayoutStyle {
    LIB3DS_LAYOUT_SINGLE                    = 0,
    LIB3DS_LAYOUT_TWO_PANE_VERT_SPLIT       = 1,
    LIB3DS_LAYOUT_TWO_PANE_HORIZ_SPLIT      = 2,
    LIB3DS_LAYOUT_FOUR_PANE                 = 3,
    LIB3DS_LAYOUT_THREE_PANE_LEFT_SPLIT     = 4,
    LIB3DS_LAYOUT_THREE_PANE_BOTTOM_SPLIT   = 5,
    LIB3DS_LAYOUT_THREE_PANE_RIGHT_SPLIT    = 6,
    LIB3DS_LAYOUT_THREE_PANE_TOP_SPLIT      = 7,
    LIB3DS_LAYOUT_THREE_PANE_VERT_SPLIT     = 8,
    LIB3DS_LAYOUT_THREE_PANE_HORIZ_SPLIT    = 9,
    LIB3DS_LAYOUT_FOUR_PANE_LEFT_SPLIT      = 10,
    LIB3DS_LAYOUT_FOUR_PANE_RIGHT_SPLIT     = 11
} Lib3dsLayoutStyle;

/* Layout view settings */
typedef struct Lib3dsView {
    int         type;
    unsigned    axis_lock;
    short       position[2];
    short       size[2];
    float       zoom;
    float       center[3];
    float       horiz_angle;
    float       vert_angle;
    char        camera[11];
} Lib3dsView;

#define LIB3DS_LAYOUT_MAX_VIEWS 32

/* Viewport and default view settings */
typedef struct Lib3dsViewport {
    int             layout_style;
    int             layout_active;
    int             layout_swap;
    int             layout_swap_prior;
    int             layout_swap_view;
    unsigned short  layout_position[2];
    unsigned short  layout_size[2];
    int             layout_nviews;
    Lib3dsView      layout_views[LIB3DS_LAYOUT_MAX_VIEWS];
    int             default_type;
    float           default_position[3];
    float           default_width;
    float           default_horiz_angle;
    float           default_vert_angle;
    float           default_roll_angle;
    char            default_camera[64];
} Lib3dsViewport;

/* Material texture map flags */
typedef enum Lib3dsTextureMapFlags {
    LIB3DS_TEXTURE_DECALE       = 0x0001,
    LIB3DS_TEXTURE_MIRROR       = 0x0002,
    LIB3DS_TEXTURE_NEGATE       = 0x0008,
    LIB3DS_TEXTURE_NO_TILE      = 0x0010,
    LIB3DS_TEXTURE_SUMMED_AREA  = 0x0020,
    LIB3DS_TEXTURE_ALPHA_SOURCE = 0x0040,
    LIB3DS_TEXTURE_TINT         = 0x0080,
    LIB3DS_TEXTURE_IGNORE_ALPHA = 0x0100,
    LIB3DS_TEXTURE_RGB_TINT     = 0x0200
} Lib3dsTextureMapFlags;

/* Material texture map */
typedef struct Lib3dsTextureMap {
    unsigned    user_id;
    void*       user_ptr;
    char        name[64];
    unsigned    flags;
    float       percent;
    float       blur;
    float       scale[2];
    float       offset[2];
    float       rotation;
    float       tint_1[3];
    float       tint_2[3];
    float       tint_r[3];
    float       tint_g[3];
    float       tint_b[3];
} Lib3dsTextureMap;

/* Auto reflection texture map flags */
typedef enum Lib3dsAutoReflMapFlags {
    LIB3DS_AUTOREFL_USE                     = 0x0001,
    LIB3DS_AUTOREFL_READ_FIRST_FRAME_ONLY   = 0x0004,
    LIB3DS_AUTOREFL_FLAT_MIRROR             = 0x0008 
} Lib3dsAutoReflMapFlags;

/* Material shading type */
typedef enum Lib3dsShading {
    LIB3DS_SHADING_WIRE_FRAME = 0,
    LIB3DS_SHADING_FLAT       = 1, 
    LIB3DS_SHADING_GOURAUD    = 2, 
    LIB3DS_SHADING_PHONG      = 3, 
    LIB3DS_SHADING_METAL      = 4
} Lib3dsShading; 

/** Material */
typedef struct Lib3dsMaterial {
    unsigned            user_id;
    void*               user_ptr;
    char                name[64];           /* Material name */
    float               ambient[3];         /* Material ambient reflectivity */
    float               diffuse[3];         /* Material diffuse reflectivity */
    float               specular[3];        /* Material specular reflectivity */
    float               shininess;          /* Material specular exponent */
    float               shin_strength;
    int                 use_blur;
    float               blur;
    float               transparency;
    float               falloff;
    int                 is_additive;
    int                 self_illum_flag; /* bool */
    float               self_illum;
    int                 use_falloff;
    int                 shading;
    int                 soften;         /* bool */
    int                 face_map;       /* bool */
    int                 two_sided;      /* Material visible from back */
    int                 map_decal;      /* bool */
    int                 use_wire;
    int                 use_wire_abs;
    float               wire_size;
    Lib3dsTextureMap    texture1_map;
    Lib3dsTextureMap    texture1_mask;
    Lib3dsTextureMap    texture2_map;
    Lib3dsTextureMap    texture2_mask;
    Lib3dsTextureMap    opacity_map;
    Lib3dsTextureMap    opacity_mask;
    Lib3dsTextureMap    bump_map;
    Lib3dsTextureMap    bump_mask;
    Lib3dsTextureMap    specular_map;
    Lib3dsTextureMap    specular_mask;
    Lib3dsTextureMap    shininess_map;
    Lib3dsTextureMap    shininess_mask;
    Lib3dsTextureMap    self_illum_map;
    Lib3dsTextureMap    self_illum_mask;
    Lib3dsTextureMap    reflection_map;
    Lib3dsTextureMap    reflection_mask;
    unsigned            autorefl_map_flags;
    int                 autorefl_map_anti_alias;  /* 0=None, 1=Low, 2=Medium, 3=High */
    int                 autorefl_map_size;
    int                 autorefl_map_frame_step;
} Lib3dsMaterial;

/** Object flags for cameras, lights and meshes */
typedef enum Lib3dsObjectFlags {
    LIB3DS_OBJECT_HIDDEN          = 0x01, 
    LIB3DS_OBJECT_VIS_LOFTER      = 0x02, 
    LIB3DS_OBJECT_DOESNT_CAST     = 0x04, 
    LIB3DS_OBJECT_MATTE           = 0x08, 
    LIB3DS_OBJECT_DONT_RCVSHADOW  = 0x10, 
    LIB3DS_OBJECT_FAST            = 0x20, 
    LIB3DS_OBJECT_FROZEN          = 0x40 
} Lib3dsObjectFlags;

/** Camera object */
typedef struct Lib3dsCamera {
    unsigned    user_id;
    void*       user_ptr;
    char        name[64];
    unsigned    object_flags; /*< @see Lib3dsObjectFlags */ 
    float       position[3];
    float       target[3];
    float       roll;
    float       fov;
    int         see_cone;
    float       near_range;
    float       far_range;
} Lib3dsCamera; 

/** Light object */
typedef struct Lib3dsLight {
    unsigned    user_id;
    void*       user_ptr;
    char        name[64];
    unsigned    object_flags; 
    int         spot_light;     /* bool */
    int         see_cone;
    float       color[3];
    float       position[3];
    float       target[3];
    float       roll;
    int         off;              /* bool */
    float       outer_range;
    float       inner_range;
    float       multiplier;
    /*const char**  excludes;*/
    float       attenuation;
    int         rectangular_spot;   /* bool */
    int         shadowed;           /* bool */
    float       shadow_bias;
    float       shadow_filter;
    int         shadow_size;
    float       spot_aspect;
    int         use_projector;
    char        projector[64];
    int         spot_overshoot;      /* bool */
    int         ray_shadows;         /* bool */
    float       ray_bias;
    float       hotspot;
    float       falloff;
} Lib3dsLight; 

/* Texture map projection */
typedef enum {
  LIB3DS_MAP_NONE           = -1,
  LIB3DS_MAP_PLANAR         = 0,
  LIB3DS_MAP_CYLINDRICAL    = 1,
  LIB3DS_MAP_SPHERICAL      = 2
} Lib3dsMapType;

/**  Meaning of Lib3dsFace::flags. ABC are points of the current face 
    (A: is 1st vertex, B is 2nd vertex, C is 3rd vertex) */
typedef enum Lib3dsFaceFlags {
  LIB3DS_FACE_VIS_AC    = 0x01,       /**< Bit 0: Edge visibility AC */
  LIB3DS_FACE_VIS_BC    = 0x02,       /**< Bit 1: Edge visibility BC */
  LIB3DS_FACE_VIS_AB    = 0x04,       /**< Bit 2: Edge visibility AB */
  LIB3DS_FACE_WRAP_U    = 0x08,       /**< Bit 3: Face is at tex U wrap seam */
  LIB3DS_FACE_WRAP_V    = 0x10,       /**< Bit 4: Face is at tex V wrap seam */
  LIB3DS_FACE_SELECT_3  = (1<<13),    /**< Bit 13: Selection of the face in selection 3*/
  LIB3DS_FACE_SELECT_2  = (1<<14),    /**< Bit 14: Selection of the face in selection 2*/
  LIB3DS_FACE_SELECT_1  = (1<<15),    /**< Bit 15: Selection of the face in selection 1*/
} Lib3dsFaceFlags;

typedef struct Lib3dsFace {
    unsigned short  index[3];
    unsigned short  flags;
    int             material;
    unsigned        smoothing_group;
} Lib3dsFace; 

/* Triangular mesh object */
typedef struct Lib3dsMesh {
    unsigned        user_id;
    void*           user_ptr;
    char            name[64];            /**< Mesh name. Don't use more than 8 characters  */
    unsigned        object_flags;        /**< @see Lib3dsObjectFlags */ 
    int             color;               /**< Index to editor palette [0..255] */
    float           matrix[4][4];        /**< Transformation matrix for mesh data */
    unsigned short  nvertices;           /**< Number of vertices in vertex array (max. 65535) */
    float           (*vertices)[3];
    float           (*texcos)[2];
    unsigned short* vflags;
    unsigned short  nfaces;              /**< Number of faces in face array (max. 65535) */
    Lib3dsFace*     faces;               
    char            box_front[64];
    char            box_back[64];
    char            box_left[64];
    char            box_right[64];
    char            box_top[64];
    char            box_bottom[64];
    int             map_type;
    float           map_pos[3];
    float           map_matrix[4][4];
    float           map_scale;
    float           map_tile[2];
    float           map_planar_size[2];
    float           map_cylinder_height;
} Lib3dsMesh; 

typedef enum Lib3dsNodeType {
    LIB3DS_NODE_AMBIENT_COLOR   = 0,
    LIB3DS_NODE_MESH_INSTANCE   = 1,
    LIB3DS_NODE_CAMERA          = 2,
    LIB3DS_NODE_CAMERA_TARGET   = 3,
    LIB3DS_NODE_OMNILIGHT       = 4,
    LIB3DS_NODE_SPOTLIGHT       = 5,
    LIB3DS_NODE_SPOTLIGHT_TARGET= 6
} Lib3dsNodeType;

typedef enum Lib3dsNodeFlags{
    LIB3DS_NODE_HIDDEN           = 0x000800,
    LIB3DS_NODE_SHOW_PATH        = 0x010000,
    LIB3DS_NODE_SMOOTHING        = 0x020000,
    LIB3DS_NODE_MOTION_BLUR      = 0x100000,
    LIB3DS_NODE_MORPH_MATERIALS  = 0x400000
} Lib3dsNodeFlags;

typedef struct Lib3dsNode {
    unsigned            user_id;
    void*               user_ptr;
    struct Lib3dsNode*  next;
    struct Lib3dsNode*  childs;
    struct Lib3dsNode*  parent;
    Lib3dsNodeType      type;
    unsigned short      node_id;            /**< 0..65535 */
    char                name[64];
    unsigned            flags;
    float               matrix[4][4];
} Lib3dsNode;

typedef enum Lib3dsKeyFlags {
    LIB3DS_KEY_USE_TENS         = 0x01,
    LIB3DS_KEY_USE_CONT         = 0x02,
    LIB3DS_KEY_USE_BIAS         = 0x04,
    LIB3DS_KEY_USE_EASE_TO      = 0x08,
    LIB3DS_KEY_USE_EASE_FROM    = 0x10
} Lib3dsKeyFlags;

typedef struct Lib3dsKey {
    int         frame;
    unsigned    flags;
    float       tens;
    float       cont;
    float       bias;
    float       ease_to;
    float       ease_from;
    float       value[4];
} Lib3dsKey;

typedef enum Lib3dsTrackType {
    LIB3DS_TRACK_BOOL   = 0,
    LIB3DS_TRACK_FLOAT  = 1,
    LIB3DS_TRACK_VECTOR = 3,
    LIB3DS_TRACK_QUAT   = 4
} Lib3dsTrackType;

typedef enum {
    LIB3DS_TRACK_REPEAT   = 0x0001,
    LIB3DS_TRACK_SMOOTH   = 0x0002,
    LIB3DS_TRACK_LOCK_X   = 0x0008,
    LIB3DS_TRACK_LOCK_Y   = 0x0010,
    LIB3DS_TRACK_LOCK_Z   = 0x0020,
    LIB3DS_TRACK_UNLINK_X = 0x0100,
    LIB3DS_TRACK_UNLINK_Y = 0x0200,
    LIB3DS_TRACK_UNLINK_Z = 0x0400
} Lib3dsTrackFlags;

typedef struct Lib3dsTrack {
    unsigned        flags;
    Lib3dsTrackType type; 
    int             nkeys;
    Lib3dsKey*      keys;   
} Lib3dsTrack;

typedef struct Lib3dsAmbientColorNode {
    Lib3dsNode      base;
    float           color[3];
    Lib3dsTrack     color_track;
} Lib3dsAmbientColorNode;

typedef struct Lib3dsMeshInstanceNode {
    Lib3dsNode      base;
    float           pivot[3];
    char            instance_name[64];
    float           bbox_min[3];
    float           bbox_max[3];
    int             hide;
    float           pos[3];
    float           rot[4];
    float           scl[3];
    float           morph_smooth;
    char            morph[64];
    Lib3dsTrack     pos_track;
    Lib3dsTrack     rot_track;
    Lib3dsTrack     scl_track;
    Lib3dsTrack     hide_track;
} Lib3dsMeshInstanceNode;

typedef struct Lib3dsCameraNode {
    Lib3dsNode      base;
    float           pos[3];
    float           fov;
    float           roll;
    Lib3dsTrack     pos_track;
    Lib3dsTrack     fov_track;
    Lib3dsTrack     roll_track;
} Lib3dsCameraNode;

typedef struct Lib3dsTargetNode {
    Lib3dsNode      base;
    float           pos[3];
    Lib3dsTrack     pos_track;
} Lib3dsTargetNode;

typedef struct Lib3dsOmnilightNode {
    Lib3dsNode      base;
    float           pos[3];
    float           color[3];
    Lib3dsTrack     pos_track;
    Lib3dsTrack     color_track;
} Lib3dsOmnilightNode;

typedef struct Lib3dsSpotlightNode {
    Lib3dsNode      base;
    float           pos[3];
    float           color[3];
    float           hotspot;
    float           falloff;
    float           roll;
    Lib3dsTrack     pos_track;
    Lib3dsTrack     color_track;
    Lib3dsTrack     hotspot_track;
    Lib3dsTrack     falloff_track;
    Lib3dsTrack     roll_track;
} Lib3dsSpotlightNode;

typedef struct Lib3dsFile {
    unsigned            user_id;
    void*               user_ptr;
    unsigned            mesh_version;
    unsigned            keyf_revision;
    char                name[12+1];
    float               master_scale;
    float               construction_plane[3];
    float               ambient[3];
    Lib3dsShadow        shadow;
    Lib3dsBackground    background;
    Lib3dsAtmosphere    atmosphere;
    Lib3dsViewport      viewport;
    Lib3dsViewport      viewport_keyf;
    int                 frames;
    int                 segment_from;
    int                 segment_to;
    int                 current_frame;
    int                 materials_size;
    int                 nmaterials;         
    Lib3dsMaterial**    materials;        
    int                 cameras_size;  
    int                 ncameras;                      
    Lib3dsCamera**      cameras;
    int                 lights_size;
    int                 nlights;                      
    Lib3dsLight**       lights;
    int                 meshes_size;
    int                 nmeshes;                      
    Lib3dsMesh**        meshes;                         
    Lib3dsNode*         nodes;
} Lib3dsFile, Lib3dsModel;

typedef struct Lib3dsBBox {
	float  bmin[3];
	float  bmax[3];
} Lib3dsBBox;

/* INCLUDED_LIB3DS_TYPES_H */
#endif  

