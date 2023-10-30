#include "render.h"
#include "common.h"
#include "math.h"

void updateScene() {}

double hitSphere(Sphere sphere, Ray ray) {
  Vec3 oc = ray.origin - sphere.center;
  auto a = vec_length_squared(ray.direction);
  auto half_b = dot(oc, ray.direction);
  auto c = vec_length_squared(oc) - sphere.radius * sphere.radius;
  auto discriminant = half_b * half_b - a * c;

  if (discriminant < 0) {
    return -1.0;
  } else {
    auto root = (-half_b - sqrt(discriminant)) / a;

    if (root <= RAY_T_MIN || RAY_T_MAX <= root) {
      root = (-half_b + sqrt(discriminant)) / a;
      if (root <= RAY_T_MIN || RAY_T_MAX <= root)
        return -1;
    }

    return root;
  }
}

Color rayColor(RenderState state, Ray ray, uint currentHitCount = 0) {
  if (currentHitCount >= RAY_BOUNCES) {
    return Color{0, 0, 0};
  }

  Sphere currentClosestSphere;
  float currentClosestT = -1;

  for (auto sphere : state.spheres) {
    auto t = hitSphere(sphere, ray);

    if ((t < currentClosestT && t > 0) || currentClosestT < 0) {
      currentClosestSphere = sphere;
      currentClosestT = t;
    }
  }

  // closest hit
  if (currentClosestT > 0.0) {

    Vec3 scatter_direction;
    Vec3 sphere_normal =
        normalize(rayAt(ray, currentClosestT) - currentClosestSphere.center);
    switch (currentClosestSphere.material) {

    case METAL: {
      scatter_direction =
          rayAt(ray, currentClosestT) -
          2 * dot(sphere_normal, rayAt(ray, currentClosestT)) * sphere_normal +
          currentClosestSphere.fuzzy_metal_reflection *
              random_vec_in_unit_sphere();
      break;
    }
    case DEFUSE: {
      scatter_direction = sphere_normal + random_vec_on_unit_sphere();
      // if the scatter direction is close to opposite of the normal we can
      // get near to infinite/NaN
      if (vec_length(scatter_direction) < 0.001) {
        scatter_direction = sphere_normal;
      }
      break;
    }

    default: {
      abort();
    }
    }

    return currentClosestSphere.color *
           rayColor(state,
                    Ray{.origin = rayAt(ray, currentClosestT),
                        .direction = normalize(scatter_direction)},
                    currentHitCount++);
  }

  // return 0.5 * Color{normalized_ray.x + 1, normalized_ray.y + 1,
  //                    normalized_ray.z + 1};

  Vec3 unit_direction = normalize(ray.direction);
  auto a = 0.5 * (unit_direction.y + 1.0);
  return (1.0 - a) * Color{1.0, 1.0, 1.0} + a * Color{0.5, 0.7, 1.0};
}

void renderScene(RenderState state, SDL_Surface *surfaceToDrawOn) {
  uint32 *Row = (uint32 *)surfaceToDrawOn->pixels;
  for (uint rowIndex = 0; rowIndex < IMAGE_HEIGHT; rowIndex++) {
    uint32 *Pixel = Row;
    for (uint columnIndex = 0; columnIndex < IMAGE_WIDTH; columnIndex++) {

      Color pixel_color{0, 0, 0};

      for (uint sample = 0; sample < SAMPLES_PER_PIXEL; ++sample) {
        auto pixel_center = state.pixel_00_loc +
                            (columnIndex * state.pixel_delta_w) +
                            (rowIndex * state.pixel_delta_h);
        auto ray_direction = pixel_center - state.camera_position;
        Ray ray = Ray{state.camera_position, ray_direction};

        pixel_color += rayColor(state, ray) / SAMPLES_PER_PIXEL;
      }

      // Color pixel_color = rayColor(state, ray);
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
