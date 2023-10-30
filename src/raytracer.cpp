#include <SDL2/SDL.h>
#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_scancode.h>
#include <future>
#include <initializer_list>

#include "./common.h"
#include "./math.h"
#include "./raytracer.h"

#include "./input.cpp"
#include "./input.h"

#include "./render.cpp"
#include "./render.h"

struct App {
  SDL_Renderer *renderer;
  SDL_Window *window;
};
App app;

void initSDL(void) {
  int rendererFlags, windowFlags;

  rendererFlags = SDL_RENDERER_ACCELERATED;

  windowFlags = 0;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  app.window = SDL_CreateWindow("Handmade_minecraft", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, IMAGE_WIDTH,
                                IMAGE_HEIGHT, windowFlags);

  if (!app.window) {
    printf("Failed to open %d x %d window: %s\n", IMAGE_WIDTH, IMAGE_HEIGHT,
           SDL_GetError());
    exit(1);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

  if (!app.renderer) {
    exit(1);
  }
}

void blitToScreen(SDL_Surface *surface) {
  SDL_RenderClear(app.renderer);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, surface);

  SDL_RenderCopy(app.renderer, texture, NULL, NULL);
  SDL_DestroyTexture(texture);

  SDL_RenderPresent(app.renderer);
}

int main(int argc, char *argv[]) {
  initSDL();

  TimePoint oldFrameStartTime = Time::now();
  TimePoint oldFrameEndTime = Time::now();
  TimePoint newFrameStartTime = Time::now();

  int maxFramerate = 120;
  float correspondingMsPerFrame = (float)1000 / (float)maxFramerate;

  // TODO FINAL EVOLUTION DECOUPLE SIMULATION FROM RENDERING

  // loadGameLibrary();
  // hasFileUpdated();

  DeviceInput oldDeviceInput = DeviceInput{};
  DeviceInput newDeviceInput = DeviceInput{};

  auto camera_position = Vec3{0, 0, 0};
  float focal_length = 1.0;
  auto viewport_w = Vec3{VIEWPORT_WIDTH, 0, 0};
  auto viewport_h = Vec3{0, -VIEWPORT_HEIGHT, 0};

  auto pixel_delta_w = viewport_w / IMAGE_WIDTH;
  auto pixel_delta_h = viewport_h / IMAGE_HEIGHT;

  auto viewport_upper_left = camera_position + Vec3{0, 0, -focal_length} -
                             viewport_w / 2 - viewport_h / 2;

  // TODO not quite sure why we times it my 0.5...
  auto pixel_00_loc =
      viewport_upper_left + 0.5 * (pixel_delta_w + pixel_delta_h);

  auto spheres = {Sphere{.center = Vec3{-1.0, 0, -1.0},
                         .radius = 0.5,
                         .color = {0.8, 0.8, 0.8},
                         .material = METAL,
                         .fuzzy_metal_reflection = 0.3},
                  Sphere{.center = Vec3{0, 0, -1.0},
                         .radius = 0.5,
                         .color = {0.7, 0.3, 0.3},
                         .material = DEFUSE},
                  Sphere{.center = Vec3{1, 0, -1},
                         .radius = 0.5,
                         .color = {0.8, 0.6, 0.2},
                         .material = METAL,
                         .fuzzy_metal_reflection = 1},
                  Sphere{.center = Vec3{0, -50.5, -1},
                         .radius = 50,
                         .color = {0.8, 0.8, 0.0},
                         .material = DEFUSE}};

  RenderState state = {
      .spheres = spheres,
      .camera_position = camera_position,
      .viewport_w = viewport_w,
      .viewport_h = viewport_h,
      .pixel_delta_w = pixel_delta_w,
      .pixel_delta_h = pixel_delta_h,

      .pixel_00_loc = pixel_00_loc,
  };

  while (1) {
    oldFrameStartTime = newFrameStartTime;
    newFrameStartTime = Time::now();

    oldDeviceInput.mouse_position = newDeviceInput.mouse_position;

    // Cannot simply assign it as i dont want to overwrite BufferedKeypressTime.
    // didnt really think this through...
    for (int i = 0; i < ___LAST___PLAYER_COMMANDS_COUNT; i++) {
      oldDeviceInput.pressed_commands[i].transition_count =
          newDeviceInput.pressed_commands[i].transition_count;
      oldDeviceInput.pressed_commands[i].key_ended_down =
          newDeviceInput.pressed_commands[i].key_ended_down;
      oldDeviceInput.pressed_commands[i].key_started_down =
          newDeviceInput.pressed_commands[i].key_started_down;
    }

    for (ButtonState &buttonState : newDeviceInput.pressed_commands) {
      buttonState.transition_count = 0;
      buttonState.key_started_down = buttonState.key_ended_down;
      // buttonState.KeyEndedDown = false;
      buttonState.buffered_keypress_time = buttonState.buffered_keypress_time;
      // code block to be executed
    }

    SDL_Surface *surface =
        SDL_CreateRGBSurface(0, IMAGE_WIDTH, IMAGE_HEIGHT, 32, 0, 0, 0, 0);
    handleInput(&newDeviceInput);

    int msSinceLastWorldUpdate =
        (int)std::chrono::duration_cast<ms>(oldFrameEndTime - oldFrameStartTime)
            .count();

    updateAndRender(state, &oldDeviceInput, surface, msSinceLastWorldUpdate);

    blitToScreen(surface);
    SDL_FreeSurface(surface);

    int timeForCurrentFrame =
        std::chrono::duration_cast<ms>(Time::now() - newFrameStartTime).count();

    if (correspondingMsPerFrame > timeForCurrentFrame) {
      SDL_Delay(correspondingMsPerFrame - timeForCurrentFrame);
    }

    //std::promise<void>().get_future().wait();
    oldFrameEndTime = Time::now();
  }

  return 0;
}
