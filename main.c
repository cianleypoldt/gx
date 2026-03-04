#include <stdio.h>
#include "render/UBO_mgr.h"
int main()
{
	ubomgr_t *ubomgr = ubomgr_create();
	ubomgr_destroy(ubomgr);

	printf("loool");
}
