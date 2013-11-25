#ifndef ANDROID_NATIVE_WINDOW_JNI_H
#define ANDROID_NATIVE_WINDOW_JNI_H

struct ANativeWindow;
typedef struct ANativeWindow ANativeWindow;

//#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

ANativeWindow *android_createDisplaySurface(void);

#ifdef __cplusplus
};
#endif

#endif // ANDROID_NATIVE_WINDOW_H
