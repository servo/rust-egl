#include "egl.h"

// use this type to return error codes
typedef int32_t status_t;

status_t selectConfigForNativeWindow(
        EGLDisplay dpy,
        EGLint const* attrs,
        EGLNativeWindowType window,
        EGLConfig* outConfig);
