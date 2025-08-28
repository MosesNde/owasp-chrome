static void init_vaapi_pic ( VAPictureH264 * va_pic ) {
 va_pic -> picture_id = VA_INVALID_ID ;
 va_pic -> flags = VA_PICTURE_H264_INVALID ;
 va_pic -> TopFieldOrderCnt = 0 ;
 va_pic -> BottomFieldOrderCnt = 0 ;
}

#include <va/va.h>
#include <va/va_drm.h>

int initialize_vaapi() {
    
    
    return 0;
}