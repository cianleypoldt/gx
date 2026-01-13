#include "external/glad/glad.h"
#include "GLFW/glfw3.h"

// plattform

typedef struct {
        GLFWwindow* window_ptr;
        int         frame_width;
        int         frame_height;
} gx_ctx;

gx_ctx* gx_ctx_init(int height, int width);
void    gx_ctx_drop(gx_ctx* ctx);
