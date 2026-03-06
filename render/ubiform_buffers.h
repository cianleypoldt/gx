#ifndef UNIFORM_BUFFERS_H
#define UNIFORM_BUFFERS_H

struct CameraUBOData {
	float mat4_proj[16];
	float mat4_view[16];
};

struct MaterialUBOData {
	float metallic;
	float roughness;
};

#endif
