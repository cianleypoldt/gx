#include "gx.h"

// Endziele:
// - 3d dreieck modelle mit textur oder einfarbig
//      1. Basis objekte erstellen können: ABO, VBO, EBO ...
//      2. Laden von punktkoordinaten, eigentlich mit unterstützung für verschiedene datenarrangements
//      3. Shader Laden & Kompilieren, verschiedene Programme müssen möglch sein; Uniform variablen setzen können
//
//
// - dann Schatten
// - dann Transparenz

int main() {
        gx_ctx* gx_context = gx_ctx_init(800, 600);

        gx_shader basic_shader = gx_shader_create(gx_context, "shaders/vs.glsl",
                                                  "shaders/fs.glsl");

        gx_ctx_drop(gx_context);
}
