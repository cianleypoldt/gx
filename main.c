#include "gx.h"

// Pläne:
//      1. Universal anwendbare Grundstruktur für Shaderkompilierung
//              -> Am wichtigsten ist die funktion für char* -> Kompilierter Shader
//              ->
//      2.

int main() {
        gx_ctx* gx_context = gx_ctx_init(800, 600);

        gx_ctx_drop(gx_context);
}
