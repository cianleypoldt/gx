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

unsigned int indices[]  = { 0, 1, 2 };
float        vertices[] = {
        -5.0f,
        -5.0f,
        0.0f,
        /*... */  // 1.0f, 0.0f, 0.0f, /*... */
        5.0f,
        -5.0f,
        0.0f,
        /*... */  // 0.0f, 1.0f, 0.0f, /*... */
        0.0f,
        5.0f,
        0.0f,
        /*... */  // 0.0f, 0.0f, 1.0f /*... */ };
};

int main() {
        gx_ctx* gx_context = gx_ctx_init(800, 600);

        float camera_position[3] = { 0, 10, 0 };
        gx_camera_set_position(gx_context, camera_position);

        float camera_rotation[4] = { 0, 0, 0, 1 };
        gx_camera_set_rotation(gx_context, camera_rotation);

        gx_layout_attrib pos_color_attrs[] = {
                { .type = GX_TYPE_FLOAT, 3, 0, 0 },
        };

        gx_layout l1 = gx_layout_create(gx_context, pos_color_attrs, 1,
                                        3 * sizeof(float));

        gx_shader basic_shader = gx_shader_create(gx_context, "shaders/vs.glsl",
                                                  "shaders/fs.glsl");

        gx_mesh_desc mesh_desc = {
                .index_count      = 3,
                .index_data_size  = 3 * sizeof(unsigned int),
                .vertex_data_size = 3 * 6 * sizeof(float),
                .index_data       = indices,
                .vertex_data      = vertices,
                .layout           = l1,
        };

        //gx_mesh mesh1 = gx_mesh_create(gx_context, mesh_desc);
        gx_mesh mesh2 = gx_mesh_create(gx_context, mesh_desc);

        gx_mesh_render(gx_context, mesh2, basic_shader);

        while (!gx_should_close(gx_context)) {
                gx_present(gx_context);
        }

        gx_ctx_drop(gx_context);
}
