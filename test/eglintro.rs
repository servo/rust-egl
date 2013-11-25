#[feature(globs)];

extern mod std;

extern mod egl;
extern mod opengles;

use egl::egl::{EGLConfig, EGLint, EGLBoolean};
use egl::egl::{EGL_NO_DISPLAY, EGL_DEFAULT_DISPLAY, EGL_NO_CONTEXT, EGL_CONTEXT_CLIENT_VERSION};
use egl::egl::{EGLDisplay, EGL_TRUE, EGL_FALSE};
use egl::egl::{EGL_RED_SIZE, EGL_BLUE_SIZE, EGL_GREEN_SIZE,
                EGL_NONE, EGL_NO_SURFACE};
use egl::egl::{GetDisplay, Initialize, CreateContext, MakeCurrent, ChooseConfig};
use egl::egl::{CreateDisplaySurface, CreateWindowSurface, SwapBuffers};

use opengles::gl2::{clear_color, clear, flush, GL_COLOR_BUFFER_BIT};

use std::libc::c_void;
use std::vec;

fn main() {

    println("1) EGL Init");
    // EGL Init
//    let mut myconfig : EGLConfig = 0 as *c_void;
    let mut myconfig : EGLConfig = std::ptr::null();
    let mut majorVersion : EGLint = 0;
    let mut minorVersion : EGLint = 0;
    let mut numconfig : EGLint = 0;
    let mut returnValue : EGLBoolean;


    println("1) EGL Init 0");
    let display: EGLDisplay = GetDisplay(EGL_DEFAULT_DISPLAY as *c_void);
    if display == (EGL_NO_DISPLAY as *c_void) {
        println("eglGetDisplay returned EGL_NO_DISPLAY.");
    }

    if Initialize(display, &mut majorVersion, &mut minorVersion) == EGL_TRUE { 
        println!("EGL version. {:?}.{:?}", majorVersion as int, minorVersion as int);
    }else {
        println("eglInitialize failed");
    }

    let attr_list = ~[ EGL_RED_SIZE, 1,
                       EGL_GREEN_SIZE, 1,
                       EGL_BLUE_SIZE, 1,
                       EGL_NONE];
    let attr_list = vec::raw::to_ptr(attr_list) as *i32;

    let ret = ChooseConfig(display, attr_list, &mut myconfig, 1, &mut numconfig);
    if ret == EGL_TRUE { println!("myconfig: {:?}", ret); }
    else { println!("Couldn't choose config: {:?}", ret); }

    let window = CreateDisplaySurface();
    let attribute_list = 0 as *i32;
    let surface = CreateWindowSurface(display, myconfig, window, attribute_list);
    if surface == (EGL_NO_SURFACE as *c_void) {
        println("gelCreateWindowSurface failed.\n");
    }

    println("2) EGL Configuration");
    // EGL Configuration
    let attr_list = ~[EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE];
    let attr_list = vec::raw::to_ptr(attr_list) as *i32;
    let context = CreateContext(display, myconfig, EGL_NO_CONTEXT as *c_void, attr_list);

    if context == (EGL_NO_CONTEXT as *c_void) {
        println("eglCreateContext failed\n");
    }
    returnValue = MakeCurrent(display, surface, surface, context);

    if returnValue != EGL_TRUE {
        println("eglMakeCurrent failed\n");
    }

    println("GL Drawing");
    clear_color(1.0, 1.0, 0.0, 1.0);
    clear(GL_COLOR_BUFFER_BIT);
    flush();

    SwapBuffers(display, surface);
    loop {}
}
