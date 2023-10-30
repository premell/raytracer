#pragma once

#include "./math.h"
#include <SDL2/SDL_scancode.h>
#include <chrono>
#include <vector>

using TimePoint = std::chrono::high_resolution_clock::time_point;
using std::chrono::high_resolution_clock;
using std::chrono::system_clock;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;

typedef uint mouse_scancode;

// CONSTANTS

const float ASPECT_RATIO = 16.0 / 9.0;
const uint IMAGE_WIDTH = 900;
const uint IMAGE_HEIGHT = static_cast<uint>(IMAGE_WIDTH / ASPECT_RATIO);

const float VIEWPORT_HEIGHT = 2.0;
const float VIEWPORT_WIDTH =
    VIEWPORT_HEIGHT * (static_cast<double>(IMAGE_WIDTH) / IMAGE_HEIGHT);

const uint RAY_BOUNCES = 10;
const uint SAMPLES_PER_PIXEL = 25;

const float RAY_T_MIN = 0.0001;
const uint RAY_T_MAX = 100000;

typedef Vec3 Color;
typedef Vec3 Point;

// INPUT

enum player_commands {
  MOVE_FORWARD,
  MOVE_BACKWARD,
  MOVE_LEFT,
  MOVE_RIGHT,

  DEBUG_1,
  DEBUG_2,
  DEBUG_3,
  DEBUG_4,
  DEBUG_5,

  ___LAST___PLAYER_COMMANDS_COUNT // just a easy way to get number of entries in
                                  // PlayerCommands
};

enum DeviceInputType { KEYBOARD, MOUSE };

struct ButtonState {
  bool key_started_down;
  bool key_ended_down;
  // amount of times the key was pressed and released
  // ATM we dont do anything with this...
  int transition_count;

  bool held_down_since_last_event;

  // time of the keypress that is buffered
  TimePoint buffered_keypress_time;
};

// keyboard, mouse, controller etc
struct DeviceInput {
  ButtonState pressed_commands[___LAST___PLAYER_COMMANDS_COUNT];
  Vec2 mouse_position;
};

struct DeviceInputEvent {
  SDL_Scancode keyboard_code;
  mouse_scancode mouse_code;

  bool pressed_else_released;
  DeviceInputType device_type;
};

enum Material {
  METAL,
  GLASS,
  DEFUSE
};

struct Sphere {
  Vec3 center;
  float radius;

  //float light_damping;

  Color color;

  Material material;
  // radius of the fuzzy sphere, the highter the more fuzz
  float fuzzy_metal_reflection;

  Color emissive_color;
  float emissive_strength;
};

struct RenderState {
  // eeeeeh todo
  std::vector<Sphere> spheres;

  Vec3 camera_position;
  float focal_length;
  Vec3 viewport_w;
  Vec3 viewport_h;

  // width
  Vec3 pixel_delta_w;
  // height
  Vec3 pixel_delta_h;

  Vec3 pixel_00_loc;
};

// xyz => red green blue

// inline Vec3

// struct color {
//   float Red;
//   float Green;
//   float Blue;
// };

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
