#ifndef __GED_GLES_H__
#define __GED_GLES_H__

#include "ged_type.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

#if defined (__cplusplus)
extern "C" {
#endif

GED_GLES_HANDLE ged_gles_create(void);

void ged_gles_destroy(GED_GLES_HANDLE hGedGles);

GED_BOOL ged_gles_is_null_driver_enabled(GED_GLES_HANDLE hGedGles);

int ged_gles_ged_frame_id(GED_GLES_HANDLE hGedGles);

GED_ERROR ged_gles_glClear_CB(GED_GLES_HANDLE hGedGles);

GED_ERROR ged_gles_glDrawXXXX_CB(GED_GLES_HANDLE hGedGles);

GED_ERROR ged_gles_glBindFramebuffer_CB(GED_GLES_HANDLE hGedGles, GLenum target, GLuint framebuffer);

GED_ERROR ged_gles_eglSwapBuffers_CB(GED_GLES_HANDLE hGedGles, EGLSurface draw);

GED_ERROR ged_gles_GPUFrameCompletion_CB(GED_GLES_HANDLE hGedGles, EGLSurface draw, int i32FrameID);

GED_ERROR ged_gles_eglQueryContext_CB(GED_GLES_HANDLE hGedGles);

#if defined (__cplusplus)
}
#endif

#endif

