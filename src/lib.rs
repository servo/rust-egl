#![crate_name = "egl"]
#![crate_type = "rlib"]

#![feature(globs)]

#![allow(non_snake_case_functions)]

extern crate libc;

pub mod egl;
pub mod eglext;
