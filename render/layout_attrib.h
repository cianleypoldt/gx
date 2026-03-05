#ifndef LAYOUT_ATTRIB_H
#define LAYOUT_ATTRIB_H

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
