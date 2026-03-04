#include <stdio.h>
#include "render/gx.h"
int main()
{
	gxctx *ctx = gx_init();
	gx_deinit(ctx);
}
