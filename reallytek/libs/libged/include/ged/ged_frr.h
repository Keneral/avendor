#ifndef __GED_GLES_H__
#define __GED_GLES_H__

#include "ged_type.h"
#include <GLES2/gl2.h>

#if defined (__cplusplus)
extern "C" {
#endif

GED_FRR_HANDLE ged_frr_create(void);

void ged_frr_destroy(GED_FRR_HANDLE hGedGles);

GED_BOOL ged_frr_is_null_driver_enabled(GED_FRR_HANDLE hGedGles);

GED_BOOL ged_frr_is_null_swapbuffers_enabled(GED_FRR_HANDLE hGedGles);

GED_ERROR ged_frr_glClear_CB(GED_FRR_HANDLE hGedGles);

GED_ERROR ged_frr_glDrawXXXX_CB(GED_FRR_HANDLE hGedGles);

GED_ERROR ged_frr_glBindFramebuffer_CB(GED_FRR_HANDLE hGedGles, GLenum target, GLuint framebuffer);

GED_ERROR ged_frr_eglSwapBuffers_CB(GED_FRR_HANDLE hGedGles);

GED_ERROR ged_frr_eglQueryContext_HWUI_CB(GED_FRR_HANDLE hGedGles);
GED_ERROR ged_frr_eglQueryContext_GIFT_CB(GED_FRR_HANDLE hGedGles, int value);


#if defined (__cplusplus)
}
#endif

#endif

