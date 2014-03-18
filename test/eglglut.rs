#[link(name = "eglglut", vers = "0.1")];
#[crate_type = "lib"];

#[feature(globs)];

extern crate std;

extern crate egl;
extern crate opengles;
extern crate glut;

use egl::egl::{SwapBuffers, GetCurrentSurface, GetCurrentDisplay};
use egl::egl;

use opengles::gl2::{clear_color, clear, flush, GL_COLOR_BUFFER_BIT};

use glut::glut;

#[cfg(not(test))]
#[start]
fn start(argc: int, argv: **u8) -> int {
    do std::rt::start_on_main_thread(argc, argv) {
        run()
    }
}

fn run() {
    glut::init();
    glut::init_display_mode(glut::DOUBLE);
    glut::init_window_size(800, 600);
    glut::create_window(~"EGL Android");

    let display = GetCurrentDisplay();
    let surface = GetCurrentSurface(egl::EGL_DRAW);

    clear_color(1.0, 1.0, 0.0, 1.0);
    clear(GL_COLOR_BUFFER_BIT);
    flush();
    SwapBuffers(display, surface);
}
