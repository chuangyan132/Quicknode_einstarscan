#ifndef SN3D_SDK_SHARED_MEMORY_CONTROLLER_H
#define SN3D_SDK_SHARED_MEMORY_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(SDK_SMC_LIBRARY)
#define SMC_EXPORT __declspec(dllexport)
#else
#define SMC_EXPORT __declspec(dllimport)
#endif

#define SMC_MK_CAM_PREFIX                       "cam"
#define SMC_MK_CURRENT_PC                       "currentPointCloud"
#define SMC_MK_CURRENT_MARKER                   "currentMarker"
#define SMC_MK_WHOLE_PC                         "wholePointCloud"
#define SMC_MK_FAILED_PC                        "failedPointCloud"
#define SMC_MK_FRAME_MARKER_POINT               "frameMarkerPoint"
#define SMC_MK_WHOLE_MARKER_POINT               "wholeMarkerPoint"
#define SMC_MK_MESH_DATA                        "meshData"
#define SMC_MK_EDIT_APPLY_DATA                  "editApplyData"
#define SMC_MK_MEASURE_NODE                     "measureEditLine"
#define SMC_MK_MULTIPLE_PORJECT_DATA            "multipleProjectData" 
#define SMC_MK_BODY_SCAN                        "bodyScanPointCloud"
#define SMC_MK_MULTI_PROJECT_MARKER_NODE        "MarkerGroupNode"
#define SMC_MK_MULTI_PROJECT_MARKER_NODE_ALIGN  "MarkerGroupNodeAlign"
#define SMC_MK_HOLE_LINE_DATA                   "holeline"
#define SMC_MK_FILL_HOLE_MESH_DATA              "fillholemesh"
#define SMC_MK_HOLE_GROUP                       "holeGroup"

#define SMC_MT_POINT_CLOUD                 0
#define SMC_MT_DELETE_POINTS               1
#define SMC_MT_MARKERS                     2
#define SMC_MT_TRI_MESH                    3
#define SMC_MT_VIDEO_DATA                  4
#define SMC_MT_RANGE_DATA                  5
#define SMC_MT_HOLE_LINE                   6
#define SMC_MT_FILL_HOLE_MESH              7
#define SMC_MT_MULTI_PROJECT_POINT_CLOUD   8
#define SMC_MT_BODY_SCAN_POINT_CLOUD       9
#define SMC_MT_GENERATE_POINT_CLOUD_EDIT   10


#define SMC_ERR_NO_ERROR            0x00 /**< No error occurred. */
#define SMC_ERR_INVALID_HANDLE      0x01 /**< The shared memory handle is invalid. */
#define SMC_ERR_KEY_ERROR           0x02 /**< The operation failed because of an invalid key. */
#define SMC_ERR_PERMISSION_DENIED   0x03 /**< The operation failed because the caller didn't have the required permissions. */
#define SMC_ERR_LOCK_ERROR          0x04 /**< The attempt to lock() the shared memory segment failed because create() or attach() failed and returned false. */
#define SMC_ERR_UNLOCK_ERROR        0x05 /**< The attempt to unlock() the shared memory segment failed because create() or attach() failed and returned false. */
#define SMC_ERR_INVALID_SIZE        0x06 /**< A create operation failed because the requested size was invalid. */
#define SMC_ERR_ALREADY_INITIALIZED 0x07 /**< The library has already been initialized. */
#define SMC_ERR_KEY_TYPE_MISMATCH   0x08 /**< The type doesn't match with the opend shared memory */
#define SMC_ERR_MEMORY_TYPE_ERROR   0x09 /**< Invalid memory type. */
#define SMC_ERR_ATTACH_ERROR        0x0a /**< Failed to attach the shared memory. */
#define SMC_ERR_DETACH_ERROR        0x0b /**< Failed to detach the shared memory. */
#define SMC_ERR_INVALID_CAMERA_ID   0x0c
#define SMC_ERR_INVALID_DATA_BUFFER 0x0d
#define SMC_ERR_VIDEO_UNCHANGED     0x0e
#define SMC_ERR_INVALID_VIDEO_SIZE  0x0f
#define SMC_ERR_UNKNOWN             0xff /**< Something else happened and it was bad. */

typedef int SMC_HANDLE;
#define SMC_INVALID_HANDLE -1

#define SMC_MAX_BUF_SIZE    256
#define SMC_MAX_CAMERA_COUNT 4

#define SMC_TRUE    1
#define SMC_FALSE   0

/**
 * @brief Get the version of the library.
 */
SMC_EXPORT void sn3d_smc_version(int *major, int *minor, int *patch);

SMC_EXPORT int sn3d_smc_error();

SMC_EXPORT int sn3d_smc_initialize();
SMC_EXPORT void sn3d_smc_cleanup();

SMC_EXPORT int sn3d_smc_lock();
SMC_EXPORT int sn3d_smc_unlock();

typedef struct tagSMC_CAMERA_VIDEO
{
    int             is_handled;
    int             width;
    int             height;
    int             channel;
    char*           data_buffer;
}SMC_CAMERA_VIDEO, *LPSMC_CAMERA_VIDEO;

SMC_EXPORT int sn3d_smc_begin_processing_camera_video(int camera_id, LPSMC_CAMERA_VIDEO camera_video);
SMC_EXPORT int sn3d_smc_finish_processing_camera_video(int camera_id);

typedef struct tagSMC_POINTS
{
    int     is_opened;
    char    name[SMC_MAX_BUF_SIZE];
    int     align_index;
    char    group_name[SMC_MAX_BUF_SIZE];
    int     point_count;
    int     has_texture;
    int     has_normal;
    int     is_incremental;
    int 	has_markers;
    int		marker_count;
    int 	is_used;
    int 	has_texure_picture;
    int		face_count;
    int		texture_image_width;
    int		texture_image_height;
    int		texture_uv_count;
    int 	has_face_normal;
    int		pack_id;
    int		total_packs;
    int		data_size;
    char*	data_buffer;
}SMC_POINTS, *LPSMC_POINTS;

SMC_EXPORT int sn3d_smc_begin_processing_points(const char *key, LPSMC_POINTS points);
SMC_EXPORT int sn3d_smc_finish_processing_points(const char *key);


typedef struct tagSMC_EDIT_DATA
{
    int     name_count;  //input param
    char**  data_names;   //input param,nameCount project path
    int*    data_counts;  //input param,nameCount int for project deleted data
    char*	data_buffer; //ouput param for user fill data as document 
}SMC_EDIT_DATA, *LPSMC_EDIT_DATA;

SMC_EXPORT int sn3d_smc_begin_processing_edit_data(LPSMC_EDIT_DATA edit_data);
SMC_EXPORT int sn3d_smc_finish_processing_edit_data();

typedef struct tagSMC_MESH
{
    char*	  data_buffer; //ouput param 
}SMC_MESH, *LPSMC_MESH;

SMC_EXPORT int sn3d_smc_begin_processing_mesh(LPSMC_MESH mesh);
SMC_EXPORT int sn3d_smc_finish_processing_mesh();

#ifdef __cplusplus
}
#endif

#endif//SN3D_SDK_SHARED_MEMORY_CONTROLLER_H
