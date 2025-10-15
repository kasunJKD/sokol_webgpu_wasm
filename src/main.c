/**
*
* TODO
* add state to main
* add arena and initialise renderers
* render and test in sokol
*
*
* */


#include <stdbool.h>

#ifdef __EMSCRIPTEN__
#define SOKOL_IMPL
#define SOKOL_WGPU
#else
#define SOKOL_IMPL
#define SOKOL_GLCORE
#endif

#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_log.h"
#include "sokol_glue.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


int width = 640;
int height = 320;

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
void setResolution(int w, int h) {
    width = w;
    height = h;
}
#endif

typedef struct { 
} State;

static void init(void) {
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });
    pass_action = (sg_pass_action) {
        .colors[0] = {
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = { 1.0f, 0.0f, 0.0f, 1.0f }
        }
    };
}

static void frame(void) {
    float g = pass_action.colors[0].clear_value.g + 0.01f;
    pass_action.colors[0].clear_value.g = (g > 1.0f) ? 0.0f : g;
    sg_begin_pass(&(sg_pass){ .action = pass_action, .swapchain = sglue_swapchain() });
    sg_end_pass();
    sg_commit();
}

static void cleanup(void) {
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

#ifdef __EMSCRIPTEN__
    // Web mode
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .window_title = "Clear (sokol app)",
        .icon.sokol_default = true,
        .html5_canvas_resize = true,        // let browser drive size
        .html5_canvas_selector = "#canvas", // match your HTML <canvas id="canvas">
        .high_dpi = false,
    };
#else
    // Native desktop mode
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .width = width,
        .height = height,
        .window_title = "Clear (sokol app)",
        .icon.sokol_default = true,
        .high_dpi = false,
    };
#endif
}
