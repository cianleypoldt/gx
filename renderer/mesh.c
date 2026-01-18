#include "core/utils.h"
#include "glad/glad.h"
#include "gx.h"
#include "renderer/objects.h"
#include "renderer/renderer.h"

void init_mesh_data_ubo(gx_ctx* ctx) {
        glGenBuffers(1, &ctx->mesh_data_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ctx->mesh_data_ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(struct mesh_ubo_data), NULL,
                     GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, ctx->mesh_data_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void destroy_mesh_data_ubo(gx_ctx* ctx) {
        glDeleteBuffers(1, &ctx->mesh_data_ubo);
}

void sync_mesh_ubo_data(gx_ctx* ctx, struct mesh_ubo_data* ubo_data) {
        glBindBuffer(GL_UNIFORM_BUFFER, ctx->mesh_data_ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(struct mesh_ubo_data),
                        ubo_data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

gx_mesh gx_mesh_create(gx_ctx* ctx, gx_mesh_desc mesh_desc) {
        struct MeshObj mesh = { 0 };

        mesh.index_count = mesh_desc.index_count;

        assign_identity(mesh.ubo_data.transform);

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
                if (gl_type == GL_FLOAT) {
                        glVertexArrayAttribFormat(
                                mesh.VAO, i, /* attribute location */
                                attr->count, gl_type, attr->normalized,
                                attr->offset);
                } else {
                        glVertexArrayAttribIFormat(
                                mesh.VAO, i, /* attribute location */
                                attr->count, gl_type, attr->offset);
                }

                glVertexArrayAttribBinding(mesh.VAO, i, /* attribute location */
                                           0            /* binding index */
                );
        }

        mesh.gx_id = ++ctx->gx_id_tracker;
        array_append(&ctx->glob_resources.mesh_objs, &mesh);
        return mesh.gx_id;
}

void gx_mesh_delete(gx_ctx* ctx, gx_mesh mesh) {
        for (size_t i = 0; i < ctx->glob_resources.mesh_objs.count; i++) {
                struct MeshObj* m = array_at(&ctx->glob_resources.mesh_objs, i);
                if (m->gx_id != mesh) {
                        continue;
                }

                glDeleteBuffers(1, &m->VBO);
                glDeleteBuffers(1, &m->EBO);
                glDeleteVertexArrays(1, &m->VAO);

                array_remove_at(&ctx->glob_resources.mesh_objs, i);
                break;
        }
}

void gx_mesh_render(gx_ctx* ctx, gx_mesh mesh, gx_shader shader) {
        struct MeshObj* m = NULL;
        for (size_t i = 0; i < ctx->glob_resources.mesh_objs.count; i++) {
                struct MeshObj* temp =
                        array_at(&ctx->glob_resources.mesh_objs, i);
                if (temp->gx_id == mesh) {
                        m = temp;
                        break;
                }
        }
        if (!m) {
                return;
        }
        compute_transform(m->ubo_data.transform, m->position, m->quat_rotation);
        sync_mesh_ubo_data(ctx, &m->ubo_data);

        glUseProgram(*(unsigned int*) array_at(
                &ctx->glob_resources.shader_programs, shader));
        glBindVertexArray(m->VAO);

        glDrawElements(GL_TRIANGLES, m->index_count, GL_UNSIGNED_INT,
                       (const void*) 0);
}

void gx_mesh_set_position(gx_ctx* ctx, gx_mesh mesh, float position[3]) {
        struct MeshObj* m = NULL;
        for (size_t i = 0; i < ctx->glob_resources.mesh_objs.count; i++) {
                struct MeshObj* temp =
                        array_at(&ctx->glob_resources.mesh_objs, i);
                if (temp->gx_id == mesh) {
                        m = temp;
                        break;
                }
        }
        if (!m) {
                return;
        }
        m->position[0] = position[0];
        m->position[1] = position[1];
        m->position[2] = position[2];
}

void gx_mesh_set_rotation(gx_ctx* ctx, gx_mesh mesh, float quat_rotation[4]) {
        struct MeshObj* m = NULL;
        for (size_t i = 0; i < ctx->glob_resources.mesh_objs.count; i++) {
                struct MeshObj* temp =
                        array_at(&ctx->glob_resources.mesh_objs, i);
                if (temp->gx_id == mesh) {
                        m = temp;
                        break;
                }
        }
        if (!m) {
                return;
        }
        m->quat_rotation[0] = quat_rotation[0];
        m->quat_rotation[1] = quat_rotation[1];
        m->quat_rotation[2] = quat_rotation[2];
        m->quat_rotation[3] = quat_rotation[3];
}
