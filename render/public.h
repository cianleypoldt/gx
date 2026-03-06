#ifndef GX_PUBLIC_H
#define GX_PUBLIC_H

struct Transform {
	float pos[3];
	float rot[4];
};

struct CameraParameters {
	float fov, aspect, near_plane, far_plane;
};

struct MaterialDesc {
	float metallic;
	float roughness;
};

enum layout_attrib_type {
	GX_FLOAT_ATTRIB,
	GX_INT_ATTRIB,
	GX_UNSIGNED_INT_ATTRIB
};

struct LayoutAttribute {
	enum layout_attrib_type type;
	int count;
	int normalized;
};

#endif
