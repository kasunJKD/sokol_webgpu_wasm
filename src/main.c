#include <emscripten.h>
#include <stdbool.h>

#define SOKOL_IMPL
#define SOKOL_WGPU
#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_log.h"
#include "sokol_glue.h"

sg_pass_action pass_action;

int width = 640;
int heigth = 320;

EMSCRIPTEN_KEEPALIVE
void setResolution(int w, int h) {
    width = w;
    heigth = h;
}

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

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .width = width,
        .height = heigth,
        .window_title = "Clear (sokol app)",
        .icon.sokol_default = true,
        .html5_canvas_resize = true,
        .html5_canvas_selector = "#canvas",
        .high_dpi = false 
    };
}
