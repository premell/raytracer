#include "render.h"
#include "common.h"
#include "math.h"

void updateScene() {}

double hitSphere(const Point &center, double radius, const Ray &ray) {
  // TODO what the hell are these????
  Vec3 oc = ray.origin - center;
  auto a = vec_length_squared(ray.direction);
  auto half_b = dot(oc, ray.direction);
  auto c = vec_length_squared(oc) - radius * radius;
  auto discriminant = half_b * half_b - a * c;

  if (discriminant < 0) {
    return -1.0;
  } else {
    return (-half_b - sqrt(discriminant)) / a;
  }
}

Color rayColor(const Ray &ray) {
  auto t = hitSphere(Point{0, 0, -1}, 0.5, ray);
  if (t > 0.0) {
    Vec3 normalized_ray = normalize(rayAt(ray, t) - Vec3{0, 0, -1});
    return 0.5 * Color{normalized_ray.x + 1, normalized_ray.y + 1,
                       normalized_ray.z + 1};
  }

  Vec3 unit_direction = normalize(ray.direction);
  auto a = 0.5 * (unit_direction.y + 1.0);
  return (1.0 - a) * Color{1.0, 1.0, 1.0} + a * Color{0.5, 0.7, 1.0};
}

void renderScene(RenderState state, SDL_Surface *surfaceToDrawOn) {
  uint32 *Row = (uint32 *)surfaceToDrawOn->pixels;
  for (uint rowIndex = 0; rowIndex < IMAGE_HEIGHT; rowIndex++) {
    uint32 *Pixel = Row;
    for (uint columnIndex = 0; columnIndex < IMAGE_WIDTH; columnIndex++) {
      auto pixel_center = state.pixel_00_loc +
                          (columnIndex * state.pixel_delta_w) +
                          (rowIndex * state.pixel_delta_h);
      auto ray_direction = pixel_center - state.camera_position;

      Ray ray = Ray{state.camera_position, ray_direction};

      Color pixel_color = rayColor(ray);
      *Pixel++ = ((int)(pixel_color.x * 255) << 16) |
                 ((int)(pixel_color.y * 255) << 8) | (int)(pixel_color.z * 255);
    }

    Row += surfaceToDrawOn->w;
  }
}

void updateAndRender(RenderState state, DeviceInput *deviceInput,
                     SDL_Surface *surfaceToDrawOn,
                     uint msSinceLastWorldUpdate) {

  updateScene();
  renderScene(state, surfaceToDrawOn);
}
