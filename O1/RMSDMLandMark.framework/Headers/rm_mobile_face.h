#ifndef INCLUDE_RMMOBILE_RM_MOBILE_FACE_H_
#define INCLUDE_RMMOBILE_RM_MOBILE_FACE_H_

#include "rm_mobile_common.h"


/// @defgroup rm_mobile_tracker_106
/// @brief face 106 points tracking interfaces
///
/// This set of interfaces processing face 106 points tracking routines
///
/// @{

/// @brief tracking配置选项，对应rm_mobile_tracker_106_create中的config参数，具体配置如下：

// 默认tracking配置，使用多线程，默认打开head_pose去抖动
#define RM_MOBILE_TRACKING_DEFAULT_CONFIG       RM_MOBILE_TRACKING_MULTI_THREAD | RM_MOBILE_TRACKING_ENABLE_DEBOUNCE

/// @brief 创建实时人脸106关键点跟踪句柄
/// @param[in] model_path 模型文件的绝对路径或相对路径,例如models/track.tar
/// @param[in] config 配置选项 例如RM_MOBILE_TRACKING_DEFAULT_CONFIG，默认使用双线程跟踪，实时视频预览建议使用该配置。
//     使用单线程算法建议选择（RM_MOBILE_TRACKING_SINGLE_THREAD)
/// @parma[out] handle 人脸跟踪句柄，失败返回NULL
/// @return 成功返回RM_OK, 失败返回其他错误码,错误码定义在rm_mobile_common.h中，如RM_E_FAIL等
RM_SDK_API rm_result_t
rm_mobile_tracker_106_create(const char *model_path,
                             unsigned int config,
                             rm_handle_t *handle);

/// @brief 设置检测到的最大人脸数目N，持续track已检测到的N个人脸直到人脸数小于N再继续做detect.
/// @param[in] handle 已初始化的关键点跟踪句柄
/// @param[in] max_facecount 设置为1即是单脸跟踪，有效范围为[1, 32]
/// @return 成功返回RM_OK, 失败返回其他错误码,错误码定义在rm_mobile_common.h中，如RM_E_FAIL等
RM_SDK_API rm_result_t
st_mobile_tracker_106_set_facelimit(rm_handle_t handle,
                                    int max_facecount);

/// @brief 设置tracker每多少帧进行一次detect.
/// @param[in] handle 已初始化的关键点跟踪句柄
/// @param[in] val  有效范围[1, -)
/// @return 成功返回RM_OK,失败返回其他错误码,错误码定义在rm_mobile_common.h中，如ST_E_FAIL等
RM_SDK_API
rm_result_t st_mobile_tracker_106_set_detect_interval(
	rm_handle_t handle,
	int val
);

/// @brief 重置实时人脸106关键点跟踪，清空track造成的缓存，重新检测下一帧图像中的人脸并跟踪，建议在两帧图片相差较大时使用
/// @param [in] handle 已初始化的实时目标人脸106关键点跟踪句柄
/// @return 成功返回RM_OK, 失败返回其他错误码,错误码定义在rm_mobile_common.h中，如ST_E_FAIL等
RM_SDK_API rm_result_t
rm_mobile_tracker_106_reset(
	rm_handle_t handle
);

/// @brief 对连续视频帧进行实时快速人脸106关键点跟踪
/// @param handle 已初始化的实时人脸跟踪句柄
/// @param image 用于检测的图像数据
/// @param pixel_format 用于检测的图像数据的像素格式,都支持，不推荐BGRA和BGR，会慢
/// @param image_width 用于检测的图像的宽度(以像素为单位)
/// @param image_height 用于检测的图像的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位)，即每行的字节数；目前仅支持字节对齐的padding，不支持roi
/// @param orientation 视频中人脸的方向
/// @param p_faces_array 检测到的人脸信息数组，api负责管理内存，会覆盖上一次调用获取到的数据
/// @param p_faces_count 检测到的人脸数量
/// @return 成功返回RM_OK，失败返回其他错误码,错误码定义在rm_mobile_common.h中，如ST_E_FAIL等
RM_SDK_API rm_result_t
rm_mobile_tracker_106_track(rm_handle_t handle,
                            const unsigned char *image,
                            rm_pixel_format pixel_format,
                            int image_width,
                            int image_height,
                            int image_stride,
                            rm_rotate_type orientation,
                            rm_mobile_106_t **p_faces_array,
                            int *p_faces_count);

/// @brief 对连续视频帧进行实时快速人脸106关键点跟踪，并检测脸部动作
/// @param handle 已初始化的实时人脸跟踪句柄
/// @param image 用于检测的图像数据
/// @param pixel_format 用于检测的图像数据的像素格式,都支持，不推荐BGRA和BGR，会慢
/// @param image_width 用于检测的图像的宽度(以像素为单位)
/// @param image_height 用于检测的图像的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位)，即每行的字节数；目前仅支持字节对齐的padding，不支持roi
/// @param orientation 视频中人脸的方向
/// @param p_face_action_array 检测到的人脸106点信息和脸部动作的数组，api负责管理内存，会覆盖上一次调用获取到的数据
/// @param p_faces_count 检测到的人脸数量
/// @return 成功返回RM_OK，失败返回其他错误码,错误码定义在rm_mobile_common.h中，如ST_E_FAIL等
RM_SDK_API rm_result_t
rm_mobile_tracker_106_track_face_action(
	rm_handle_t handle,
	const unsigned char *image,
	rm_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	rm_rotate_type orientation,
	rm_mobile_face_action_t **p_face_action_array,
	int *p_faces_count
);

/// @brief 设置需要检测的脸部动作。若不设置，默认检测所有脸部动作
/// @param[in] handle 已初始化的实时人脸跟踪句柄
/// @param[in] actions 需要检测的脸部动作。在rm_mobile_common.h中定义：ST_MOBILE_EYE_BLINK | ST_MOBILE_MOUTH_AH | ST_MOBILE_HEAD_YAW | ST_MOBILE_HEAD_PITCH | ST_MOBILE_BROW_JUMP
/// @return 成功返回RM_OK，失败返回其他错误码，错误码定义在rm_mobile_common.h中，如ST_E_INVALIDARG等
RM_SDK_API rm_result_t
rm_mobile_tracker_106_set_detect_actions(
	rm_handle_t handle,
	unsigned int actions
);

/// @brief 设置106点平滑的阈值。若不设置，使用默认值
/// @param[in] handle 已初始化的实时人脸跟踪句柄
/// @param[in] threshold  默认值0.5，建议取值范围：[0.0, 1.0]。阈值越大，去抖动效果越好，跟踪延时越大
/// @return 成功返回RM_OK，失败返回其他错误码，错误码定义在rm_mobile_common.h中，如ST_E_INVALIDARG等
RM_SDK_API rm_result_t
rm_mobile_tracker_106_set_smooth_threshold(
	rm_handle_t handle,
	float threshold
);

/// @brief 设置head_pose去抖动的阈值。若不设置，使用默认值
/// @param[in] handle 已初始化的实时人脸跟踪句柄
/// @param[in] threshold  默认值0.5，建议取值范围：[0.0, 1.0]。阈值越大，去抖动效果越好，跟踪延时越大
/// @return 成功返回RM_OK，失败返回其他错误码，错误码定义在rm_mobile_common.h中，如ST_E_INVALIDARG等
RM_SDK_API rm_result_t
rm_mobile_tracker_106_set_headpose_threshold(
	rm_handle_t handle,
	float threshold
);

/// @brief 销毁已初始化的track106句柄
/// @param[in] handle 已初始化的人脸跟踪句柄
RM_SDK_API void
rm_mobile_tracker_106_destroy(
	rm_handle_t handle
);
/// @}

/// @defgroup st_mobile_face_detection
/// @brief face detection interfaces
///
/// This set of interfaces processing face detection routines
///
/// @{

/// @brief detect配置开关，对应rm_mobile_face_detection_create中的config参数，具体配置如下：

// 选择将图像缩小后进行track，最后再将结果处理为源图像对应结果。如果都不选择，直接处理原图。缩小后可提高处理速度。

// 默认detect配置，直接处理原图,可以最大限度的检测到相应人脸
#define ST_MOBILE_DETECT_DEFAULT_CONFIG     0x00000000


/// @brief 创建人脸检测句柄
/// @param[in] model_path 模型文件的绝对路径或相对路径，例如models/track.tar，可以与track106模型使用相同模型。
/// 模型内文件支持detect; detect+align;detect+align+pose三种模型. detect模型只检测人脸框；detect+align模型检测人脸框和106关键点位置；detect+align+pose检测人脸框、106关键点位置、pose信息
/// @param[in] config 配置选项，例如RM_MOBILE_DETECT_DEFAULT_CONFIG等
/// @parma[out] handle 人脸检测句柄，失败返回NULL
/// @return 成功返回RM_OK, 失败返回其他错误码,错误码定义在rm_mobile_common.h中，如ST_E_FAIL等
RM_SDK_API rm_result_t
rm_mobile_face_detection_create(
	const char *model_path,
	unsigned int config,
	rm_handle_t *handle
);

/// @param[in] handle 已初始化的人脸检测句柄
/// @param[in] image 用于检测的图像数据
/// @param[in] pixel_format 用于检测的图像数据的像素格式,都支持，不推荐BGRA和BGR，会慢
/// @param[in] image_width 用于检测的图像的宽度(以像素为单位)
/// @param[in] image_height 用于检测的图像的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位)，即每行的字节数；目前仅支持字节对齐的padding，不支持roi
/// @param[in] orientation 图像中人脸的方向
/// @param[out] p_faces_array 检测到的人脸信息数组，api负责分配内存，需要调用st_mobile_face_detection_release_result函数释放
/// @param[out] p_faces_count 检测到的人脸数量
/// @return 成功返回RM_OK，失败返回其他错误码,错误码定义在rm_mobile_common.h中，如ST_E_FAIL等
RM_SDK_API rm_result_t
st_mobile_face_detection_detect(
	rm_handle_t handle,
	const unsigned char *image,
	rm_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	rm_rotate_type orientation,
	rm_mobile_106_t **p_faces_array,
	int *p_faces_count
);

/// @brief 释放人脸检测结果
/// @param[in] faces_array 跟踪到到的人脸信息数组
/// @param[in] faces_count 跟踪到的人脸数量
RM_SDK_API void
rm_mobile_face_detection_release_result(
	rm_mobile_106_t *faces_array,
	int faces_count
);

/// @brief 销毁已初始化的人脸检测句柄
/// @param[in] handle 已初始化的句柄
RM_SDK_API void
rm_mobile_face_detection_destroy(
	rm_handle_t handle
);
/// @}


#endif  // INCLUDE_RMMOBILE_RM_MOBILE_FACE_H_
