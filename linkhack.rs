// Some crumminess to make sure we link correctly

#[cfg(target_os = "android")]
#[link_args = "-L. -lEGL"]
#[nolink]
extern { }
