#ifndef INCLUDE_RMMOBILE_RM_MOBILE_COMMON_H_
#define INCLUDE_RMMOBILE_RM_MOBILE_COMMON_H_

/// @defgroup RM_common RM common
/// @brief common definitions for RM libs
/// @{


#ifdef _MSC_VER
#	ifdef __cplusplus
#		ifdef RM_RMATIC_LIB
#			define RM_SDK_API  extern "C"
#		else
#			ifdef SDK_EXPORTS
#				define RM_SDK_API extern "C" __declspec(dllexport)
#			else
#				define RM_SDK_API extern "C" __declspec(dllimport)
#			endif
#		endif
#	else
#		ifdef RM_RMATIC_LIB
#			define RM_SDK_API
#		else
#			ifdef SDK_EXPORTS
#				define RM_SDK_API __declspec(dllexport)
#			else
#				define RM_SDK_API __declspec(dllimport)
#			endif
#		endif
#	endif
#else /* _MSC_VER */
#	ifdef __cplusplus
#		ifdef SDK_EXPORTS
#			define RM_SDK_API extern "C" __attribute__((visibility ("default")))
#		else
#			define RM_SDK_API extern "C"
#		endif
#	else
#		ifdef SDK_EXPORTS
#			define RM_SDK_API __attribute__((visibility ("default")))
#		else
#			define RM_SDK_API
#		endif
#	endif
#endif

/// rm handle declearation
typedef void *rm_handle_t;

/// rm result declearation
typedef int   rm_result_t;

#define	RM_OK 0					///< 正常运行

#define	RM_E_INVALIDARG -1			///< 无效参数
#define	RM_E_HANDLE -2				///< 句柄错误
#define	RM_E_OUTOFMEMORY -3			///< 内存不足
#define	RM_E_FAIL -4				///< 内部错误
#define	RM_E_DELNOTFOUND -5			///< 定义缺失
#define	RM_E_INVALID_PIXEL_FORMAT -6		///< 不支持的图像格式
#define	RM_E_FILE_NOT_FOUND -7			///< 模型文件不存在
#define	RM_E_INVALID_FILE_FORMAT -8		///< 模型格式不正确导致加载失败
#define	RM_E_FILE_EXPIRE -9			///< 模型文件过期

#define	RM_E_INVALID_AUTH -13			///< license不合法
#define	RM_E_INVALID_APPID -14			///< 包名错误
#define	RM_E_AUTH_EXPIRE -15			///< license过期
#define	RM_E_UUID_MISMATCH -16			///< UUID不匹配
#define	RM_E_ONLINE_AUTH_CONNECT_FAIL -17	///< 在线验证连接失败
#define	RM_E_ONLINE_AUTH_TIMEOUT -18		///< 在线验证超时
#define	RM_E_ONLINE_AUTH_INVALID -19		///< 在线签发服务器端返回错误
#define	RM_E_LICENSE_IS_NOT_ACTIVABLE -20	///< license不可激活
#define	RM_E_ACTIVE_FAIL -21			///< license激活失败
#define	RM_E_ACTIVE_CODE_INVALID -22		///< 激活码无效

#define RM_E_NO_CAPABILITY -23			///< license文件没有提供这个能力
#define RM_E_GET_UDID_FAIL -24			///< 在线签发绑定机器码的license时，无法获取机器唯一标识
#define RM_E_SOCKET_INIT_FAILED -25		///< socket初始化失败
#define RM_E_SOCKET_NO_VERSION_TWO_POINT_TWO -26///< Windows下特定错误，没有socket2.2版本
#define RM_E_GET_HORM_ERROR -27			///< 无法解析验证服务器的地址

#ifndef CHECK_FLAG
#define CHECK_FLAG(action,flag) (((action)&(flag)) == flag)
#endif


/// rm rectangle definition
typedef struct rm_rect_t {
	int left;   ///< 矩形最左边的坐标
	int top;    ///< 矩形最上边的坐标
	int right;  ///< 矩形最右边的坐标
	int bottom; ///< 矩形最下边的坐标
} rm_rect_t;

/// rm float type point definition
typedef struct rm_pointf_t {
	float x;    ///< 点的水平方向坐标，为浮点数
	float y;    ///< 点的竖直方向坐标，为浮点数
} rm_pointf_t;

/// rm integer type point definition
typedef struct rm_pointi_t {
	int x;      ///< 点的水平方向坐标，为整数
	int y;      ///< 点的竖直方向坐标，为整数
} rm_pointi_t;

/// RM pixel format definition
typedef enum {
	RM_PIX_FMT_GRAY8,   ///< Y    1        8bpp ( 单通道8bit灰度像素 )
	RM_PIX_FMT_YUV420P, ///< YUV  4:2:0   12bpp ( 3通道, 一个亮度通道, 另两个为U分量和V分量通道, 所有通道都是连续的 )
	RM_PIX_FMT_NV12,    ///< YUV  4:2:0   12bpp ( 2通道, 一个通道是连续的亮度通道, 另一通道为UV分量交错 )
	RM_PIX_FMT_NV21,    ///< YUV  4:2:0   12bpp ( 2通道, 一个通道是连续的亮度通道, 另一通道为VU分量交错 )
	RM_PIX_FMT_BGRA8888,///< BGRA 8:8:8:8 32bpp ( 4通道32bit BGRA 像素 )
	RM_PIX_FMT_BGR888,  ///< BGR  8:8:8   24bpp ( 3通道24bit BGR 像素 )
	RM_PIX_FMT_RGBA8888 ///< BGRA 8:8:8:8 32bpp ( 4通道32bit RGBA 像素 )
} rm_pixel_format;

/// image rotate type definition
typedef enum {
	RM_CLOCKWISE_ROTATE_0 = 0,  ///< 图像不需要旋转，图像中的人脸为正脸
	RM_CLOCKWISE_ROTATE_90 = 1, ///< 图像需要顺时针旋转90度，使图像中的人脸为正
	RM_CLOCKWISE_ROTATE_180 = 2,///< 图像需要顺时针旋转180度，使图像中的人脸为正
	RM_CLOCKWISE_ROTATE_270 = 3 ///< 图像需要顺时针旋转270度，使图像中的人脸为正
} rm_rotate_type;

/// @brief 时间戳定义
typedef struct RM_time_t {
	long int tv_sec;	///< 秒
	long int tv_usec;	///< 微妙
}rm_time_t;

/// 图像格式定义
typedef struct rm_image_t {
	unsigned char *data;		///< 图像数据指针
	rm_pixel_format pixel_format;	///< 像素格式
	int width;			///< 宽度(以像素为单位)
	int height;			///< 高度(以像素为单位)
	int stride;			///< 跨度, 即每行所占的字节数
	rm_time_t time_stamp;		///< 时间戳
} rm_image_t;

/// @brief 供106点使用
typedef struct rm_mobile_106_t {
    rm_rect_t rect;         ///< 代表面部的矩形区域
    float score;            ///< 置信度
    rm_pointf_t points_array[106];  ///< 人脸106关键点的数组
    float yaw;              ///< 水平转角，真实度量的左负右正
    float pitch;            ///< 俯仰角，真实度量的上负下正
    float roll;             ///< 旋转角，真实度量的左负右正
    float eye_dist;         ///< 两眼间距
    int ID;                 ///< faceID: 每个检测到的人脸拥有唯一的faceID。人脸跟踪丢失以后重新被检测到，会有一个新的faceID
} rm_mobile_106_t,*p_rm_mobile_106_t;

/// @brief track106配置选项，对应RM_mobile_tracker_106_create和RM_mobile_human_action_create中的config参数，具体配置如下：
// 使用单线程或双线程track：处理图片必须使用单线程，处理视频建议使用多线程
#define RM_MOBILE_TRACKING_MULTI_THREAD         0x00000000  ///< 多线程，功耗较多，卡顿较少
#define RM_MOBILE_TRACKING_SINGLE_THREAD        0x00010000  ///< 单线程，功耗较少，对于性能弱的手机，会偶尔有卡顿现象
#define RM_MOBILE_TRACKING_ENABLE_DEBOUNCE      0x00000010  ///< 打开去抖动
#define RM_MOBILE_TRACKING_ENABLE_FACE_ACTION   0x00000020  ///< 检测脸部动作：张嘴、眨眼、抬眉、点头、摇头

/// face信息及face上的相关动作
typedef struct rm_mobile_face_action_t {
	struct rm_mobile_106_t face;    /// 人脸信息，包含矩形、106点、pose信息等
	unsigned int face_action;       /// 脸部动作
} rm_mobile_face_action_t;

// face action 0x00000000-0x00000020
#define RM_MOBILE_FACE_DETECT      0x00000001    ///<  人脸检测
#define RM_MOBILE_EYE_BLINK        0x00000002    ///<  眨眼
#define RM_MOBILE_MOUTH_AH         0x00000004    ///<  嘴巴大张
#define RM_MOBILE_HEAD_YAW         0x00000008    ///<  摇头
#define RM_MOBILE_HEAD_PITCH       0x00000010    ///<  点头
#define RM_MOBILE_BROW_JUMP        0x00000020    ///<  眉毛挑动

/// @brief 设置眨眼动作的阈值，置信度为[0,1], 默认阈值为0.5
RM_SDK_API void
rm_mobile_set_eyeblink_threshold(
	float threshold
);
/// @brief 设置张嘴动作的阈值，置信度为[0,1], 默认阈值为0.5
RM_SDK_API void
rm_mobile_set_mouthah_threshold(
	float threshold
);
/// @brief 设置摇头动作的阈值，置信度为[0,1], 默认阈值为0.5
RM_SDK_API void
rm_mobile_set_headyaw_threshold(
	float threshold
);
/// @brief 设置点头动作的阈值，置信度为[0,1], 默认阈值为0.5
RM_SDK_API void
rm_mobile_set_headpitch_threshold(
	float threshold
);
/// @brief 设置挑眉毛动作的阈值，置信度为[0,1], 默认阈值为0.5
RM_SDK_API void
rm_mobile_set_browjump_threshold(
	float threshold
);

/// 支持的颜色转换格式
typedef enum {
	RM_BGRA_YUV420P = 0,    ///< RM_PIX_FMT_BGRA8888到RM_PIX_FMT_YUV420P转换
	RM_BGR_YUV420P = 1,     ///< RM_PIX_FMT_BGR888到RM_PIX_FMT_YUV420P转换
	RM_BGRA_NV12 = 2,       ///< RM_PIX_FMT_BGRA8888到RM_PIX_FMT_NV12转换
	RM_BGR_NV12 = 3,        ///< RM_PIX_FMT_BGR888到RM_PIX_FMT_NV12转换
	RM_BGRA_NV21 = 4,       ///< RM_PIX_FMT_BGRA8888到RM_PIX_FMT_NV21转换
	RM_BGR_NV21 = 5,        ///< RM_PIX_FMT_BGR888到RM_PIX_FMT_NV21转换
	RM_YUV420P_BGRA = 6,    ///< RM_PIX_FMT_YUV420P到RM_PIX_FMT_BGRA8888转换
	RM_YUV420P_BGR = 7,     ///< RM_PIX_FMT_YUV420P到RM_PIX_FMT_BGR888转换
	RM_NV12_BGRA = 8,       ///< RM_PIX_FMT_NV12到RM_PIX_FMT_BGRA8888转换
	RM_NV12_BGR = 9,        ///< RM_PIX_FMT_NV12到RM_PIX_FMT_BGR888转换
	RM_NV21_BGRA = 10,      ///< RM_PIX_FMT_NV21到RM_PIX_FMT_BGRA8888转换
	RM_NV21_BGR = 11,       ///< RM_PIX_FMT_NV21到RM_PIX_FMT_BGR888转换
	RM_BGRA_GRAY = 12,      ///< RM_PIX_FMT_BGRA8888到RM_PIX_FMT_GRAY8转换
	RM_BGR_BGRA = 13,       ///< RM_PIX_FMT_BGR888到RM_PIX_FMT_BGRA8888转换
	RM_BGRA_BGR = 14,       ///< RM_PIX_FMT_BGRA8888到RM_PIX_FMT_BGR888转换
	RM_YUV420P_GRAY = 15,   ///< RM_PIX_FMT_YUV420P到RM_PIX_FMT_GRAY8转换
	RM_NV12_GRAY = 16,      ///< RM_PIX_FMT_NV12到RM_PIX_FMT_GRAY8转换
	RM_NV21_GRAY = 17,      ///< RM_PIX_FMT_NV21到RM_PIX_FMT_GRAY8转换
	RM_BGR_GRAY = 18,       ///< RM_PIX_FMT_BGR888到RM_PIX_FMT_GRAY8转换
	RM_GRAY_YUV420P = 19,   ///< RM_PIX_FMT_GRAY8到RM_PIX_FMT_YUV420P转换
	RM_GRAY_NV12 = 20,      ///< RM_PIX_FMT_GRAY8到RM_PIX_FMT_NV12转换
	RM_GRAY_NV21 = 21,      ///< RM_PIX_FMT_GRAY8到RM_PIX_FMT_NV21转换
	RM_NV12_YUV420P = 22,   ///< RM_PIX_FMT_GRAY8到RM_PIX_FMT_NV21转换
	RM_NV21_YUV420P = 23,   ///< RM_PIX_FMT_GRAY8到RM_PIX_FMT_NV21转换
	RM_NV21_RGBA = 24,      ///< RM_PIX_FMT_NV21到RM_PIX_FMT_RGBA转换
	RM_BGR_RGBA = 25,       ///< RM_PIX_FMT_BGR到RM_PIX_FMT_RGBA转换
	RM_BGRA_RGBA = 26,      ///< RM_PIX_FMT_BGRA到RM_PIX_FMT_RGBA转换
	RM_RGBA_BGRA = 27,      ///< RM_PIX_FMT_RGBA到RM_PIX_FMT_BGRA转换
	RM_GRAY_BGR = 28,       ///< RM_PIX_FMT_GRAY8到RM_PIX_FMT_BGR888转换
	RM_GRAY_BGRA = 29,      ///< RM_PIX_FMT_GRAY8到RM_PIX_FMT_BGRA8888转换
	RM_NV12_RGBA = 30,      ///< RM_PIX_FMT_NV12到RM_PIX_FMT_RGBA8888转换
	RM_NV12_RGB = 31,       ///< RM_PIX_FMT_NV12到RM_PIX_FMT_RGB888转换
	RM_RGBA_NV12 = 32,      ///< RM_PIX_FMT_RGBA8888到RM_PIX_FMT_NV12转换
	RM_RGB_NV12 = 33,       ///< RM_PIX_FMT_RGB888到RM_PIX_FMT_NV12转换
	RM_RGBA_BGR = 34,       ///< RM_PIX_FMT_RGBA888到RM_PIX_FMT_BGR888转换
	RM_BGRA_RGB = 35,       ///< RM_PIX_FMT_BGRA888到RM_PIX_FMT_RGB888转换
	RM_RGBA_GRAY = 36,
	RM_RGB_GRAY = 37,
} rm_color_convert_type;

/// @brief 进行颜色格式转换, 不建议使用关于YUV420P的转换，速度较慢
/// @param image_src 用于待转换的图像数据
/// @param image_dRM 转换后的图像数据
/// @param image_width 用于转换的图像的宽度(以像素为单位)
/// @param image_height 用于转换的图像的高度(以像素为单位)
/// @param type 需要转换的颜色格式
/// @return 正常返回RM_OK，否则返回错误类型
RM_SDK_API rm_result_t
rm_mobile_color_convert(
	const unsigned char *image_src,
	unsigned char *image_dst,
	int image_width,
	int image_height,
	rm_color_convert_type type
);

/// @}

#endif  // INCLUDE_RMMOBILE_RM_MOBILE_COMMON_H_
