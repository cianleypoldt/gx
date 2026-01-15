#include "core/utils.h"
#include "glad/glad.h"
#include "gx.h"
#include "renderer/objects.h"

gx_mesh gx_mesh_create(gx_ctx* ctx, gx_mesh_desc mesh_desc) {
        struct MeshObj mesh = { 0 };

        mesh.shader_program = mesh_desc.shader;
        mesh.index_count    = mesh_desc.index_count;

        struct LayoutTemplate* layout =
                array_at(&ctx->glob_resources.layouts, mesh_desc.layout);

        glCreateBuffers(1, &mesh.VBO);
        glNamedBufferStorage(mesh.VBO, mesh_desc.vertex_data_size,
                             mesh_desc.vertex_data, 0);

        glCreateBuffers(1, &mesh.EBO);
        glNamedBufferStorage(mesh.EBO, mesh_desc.index_data_size,
                             mesh_desc.index_data, 0);

        glCreateVertexArrays(1, &mesh.VAO);

        // bind vertex buffer to binding index 0
        glVertexArrayVertexBuffer(mesh.VAO, 0, mesh.VBO, 0, layout->stride);

        // bind index buffer
        glVertexArrayElementBuffer(mesh.VAO, mesh.EBO);

        // apply layout template
        for (GLuint i = 0; i < layout->attrib_count; ++i) {
                const gx_layout_attrib* attr = &layout->attributes[i];

                GLenum gl_type;
                switch (attr->type) {
                        case GX_TYPE_FLOAT:
                                gl_type = GL_FLOAT;
                                break;
                        case GX_TYPE_INT:
                                gl_type = GL_INT;
                                break;
                        case GX_TYPE_UNSIGNED_INT:
                                gl_type = GL_UNSIGNED_INT;
                                break;
                        default:
                                gl_type = GL_FLOAT;
                                break;
                }

                glEnableVertexArrayAttrib(mesh.VAO, i);

                glVertexArrayAttribFormat(mesh.VAO, i, /* attribute location */
                                          attr->count, gl_type,
                                          attr->normalized, attr->offset);

                glVertexArrayAttribBinding(mesh.VAO, i, /* attribute location */
                                           0            /* binding index */
                );
        }

        mesh.gx_id = ++ctx->gx_id_tracker;
        array_append(&ctx->glob_resources.mesh_objs, &mesh);
        return mesh.gx_id;
}

void gx_mesh_delete(gx_ctx* ctx, gx_mesh mesh) {
        for (int i = 0; i < ctx->glob_resources.mesh_objs.count; i++) {
                struct MeshObj* m = array_at(&ctx->glob_resources.mesh_objs, i);
                if (m->gx_id != mesh) {
                        continue;
                }
                glDeleteBuffers(1, &m->VBO);
                glDeleteBuffers(1, &m->EBO);
                glDeleteVertexArrays(1, &m->VAO);

                array_remove_at(&ctx->glob_resources.mesh_objs, i);
        }
}
