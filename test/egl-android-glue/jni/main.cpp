/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>
//#include <dlfcn.h>
#include <string.h>

#include <stdlib.h>
#include <unistd.h>

#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GLES2/gl2.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android-dl.h>

#define LOG(prio, tag, a, args...) __android_log_print(prio, tag, "[%s::%d]"#a"",__FUNCTION__, __LINE__, ##args);
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

typedef void (*fty_glutMainLoopEvent)();
typedef void (*fty_glutInit)(int*, char**);
typedef void (*fty_glutInitDisplayMode)(unsigned int);
typedef int (*fty_glutCreateWindow)(const char*);
typedef void (*fty_glutDestroyWindow)(int);
typedef void (*fty_glutPostRedisplay)();
typedef void (*fty_glutSwapBuffers)();
typedef int (*fty_glutGetWindow)();
typedef void (*fty_glutSetWindow)(int);
typedef void (*fty_glutReshapeWindow)(int ,int);
typedef void (*fty_glutDisplayFunc)(void (*)());
typedef void (*fty_glutReshapeFunc)(void (*)(int, int));
typedef void (*fty_glutTimerFunc)(unsigned int, void (*)(int), int);
typedef int (*fty_glutGet)(unsigned int);
typedef void (*fty_glutKeyboardFunc)(void (*)(unsigned char, int, int));
typedef void (*fty_glutMouseFunc)(void (*)(int, int, int, int));
typedef void (*fty_glutMouseWheelFunc)(void (*)(int, int, int, int));
typedef void (*fty_glutSetWindowTitle)(char const*);
typedef void (*fty_glutIdleFunc)(void(*)());
typedef void (*fty_glutInitWindowSize)(int, int);
typedef int (*fty_glutGetModifiers)();


#define REGISTER_FUNCTION(lib, function)\
    void (*reg_fn_##function)(fty_##function);\
    *(void**)(&reg_fn_##function) = dlsym(lib, "reg_fn_" #function);\
    if (function == NULL) {\
        LOGW("could not find reg_fn_" #function " from " #lib);\
        return;\
    } else {\
        LOGI("loaded reg_fn_" #function " from " #lib);\
        reg_fn_##function(function);\
        LOGI("registerd "#function);\
    }\

static void init_servo()
{
    LOGI("init_servo");

    setenv("RUST_LOG", "egl,glut,eglglut,opengles,std,rt,extra", 1);
    setenv("SERVO_URL", "/mnt/sdcard/html/demo.html", 1);
    
//    char* size_stack = getenv("RUST_MIN_STACK");
//    char* rust_log = getenv("RUST_LOG");
//    char* servo_url = getenv("SERVO_URL");

//    LOGI("Stack Size is : %s", size_stack);
//    LOGI("RUST_LOG flag is : %s", rust_log);
//    LOGI("loading url is : %s", servo_url);
    
/*
    void* librustuv = android_dlopen("/data/data/com.example.ServoAndroid/lib/librustuv-d4277cd5f62aa99-0.9-pre.so");
    if (librustuv == NULL) {
        LOGW("failed to load rustuv lib: %s", dlerror());
        return;
    }
*/

    LOGI("load EGL with GLUT library");
    void* libegl = android_dlopen("/data/data/com.samsung.EGLAndroid/lib/libeglglut-a14dfd7762d6fb4b-0.1.so");
    if (libegl == NULL) {
    	LOGW("failed to load servo lib: %s", dlerror());
    	return;
    }

    LOGI("load rust-glut library");
    void* libglut = android_dlopen("/data/data/com.samsung.EGLAndroid/lib/libglut-102129e09d96658-0.1.so");
    if (libglut == NULL) {
        LOGW("failed to load rust-glut lib: %s", dlerror());
        return;
    }

    REGISTER_FUNCTION(libglut, glutMainLoopEvent);
    REGISTER_FUNCTION(libglut, glutInit);
    REGISTER_FUNCTION(libglut, glutInitDisplayMode);
    REGISTER_FUNCTION(libglut, glutCreateWindow);
    REGISTER_FUNCTION(libglut, glutDestroyWindow);
    REGISTER_FUNCTION(libglut, glutPostRedisplay);
    REGISTER_FUNCTION(libglut, glutSwapBuffers);
    REGISTER_FUNCTION(libglut, glutGetWindow);
    REGISTER_FUNCTION(libglut, glutSetWindow);
    REGISTER_FUNCTION(libglut, glutReshapeWindow);
    REGISTER_FUNCTION(libglut, glutDisplayFunc);
    REGISTER_FUNCTION(libglut, glutReshapeFunc);
    REGISTER_FUNCTION(libglut, glutTimerFunc);
    REGISTER_FUNCTION(libglut, glutGet);
    REGISTER_FUNCTION(libglut, glutKeyboardFunc);
    REGISTER_FUNCTION(libglut, glutMouseFunc);
    REGISTER_FUNCTION(libglut, glutMouseWheelFunc);
    REGISTER_FUNCTION(libglut, glutSetWindowTitle);
    REGISTER_FUNCTION(libglut, glutIdleFunc);
    REGISTER_FUNCTION(libglut, glutInitWindowSize);
    REGISTER_FUNCTION(libglut, glutGetModifiers);

    void (*amain)(int, char**);
    *(void**)(&amain) = dlsym(libegl, "amain");
    if (amain) {
        LOGI("go into amain()");
        static char* argv[] = {"servo"};
        (*amain)(1, argv);
        return;
    }
    LOGW("could not find amain() from servo");
}

const int W = 800;
const int H = 600;

/**
 * Initialize an EGL context for the current display.
 */
static int init_display() {
    // initialize OpenGL ES and EGL

    int argc = 1;
    char* argv[] = {"servo"};

	LOGI("initialize GLUT START");

    glutInitWindowSize(W, H);
	LOGI("initialize 1");
//    glutInitWindowPosition(40,40);
	LOGI("initialize 2");
//    glutInit(&argc, argv);
	LOGI("initialize 3");
//    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	LOGI("initialize 4");
    
//    glutCreateWindow("Servo Android");
    
	LOGI("initialize OpenGL END");
    return 0;
}

int main(int argc, char* argv[])
{
    init_display();
    init_servo();

    return 0;
}
