#include "common.h"
#include <SDL2/SDL_surface.h>
#include <sys/types.h>

void updateAndRender(RenderState state, DeviceInput *deviceInput,
                     SDL_Surface *surfaceToDrawOn, uint msSinceLastWorldUpdate);
