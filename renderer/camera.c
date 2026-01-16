#include "glad/glad.h"
#include "gx.h"
#include "renderer/objects.h"

#include <math.h>

void init_UBO(gx_ctx* ctx, struct Camera* camera) {
        glGenBuffers(1, &camera->UBO);
        glBindBuffer(GL_UNIFORM_BUFFER, camera->UBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(camera->ubo_data),
                     &camera->ubo_data, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera->UBO);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void update_UBO(gx_ctx* ctx, struct Camera* camera) {
        glBindBuffer(GL_UNIFORM_BUFFER, camera->UBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(camera->ubo_data),
                     &camera->ubo_data, GL_DYNAMIC_DRAW);
}
