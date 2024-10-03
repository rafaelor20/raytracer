#include "include/Material.h"
#include <string>
#define OLC_PGE_APPLICATION
#include "include/olcPixelGameEngine.h"

#include "include/Hittable.h"
#include "include/HittableList.h"
#include "include/Sphere.h"
#include "include/color.h"
#include "include/rtweekend.h"

class Example : public olc::PixelGameEngine {
public:
  Example() { sAppName = "Path Tracer"; }

public:
  bool OnUserCreate() override {
    image_width = ScreenWidth();
    aspect_ratio = 16.0 / 9.0;
    samples_per_pixel = 100;
    samples_per_pixel = 10;
    max_depth = 50;
    vfov = 20;
    lookfrom = point3(13, 2, 3); // Point camera is looking from
    lookat = point3(0, 0, 0);    // Point camera is looking at
    vup = Vec3(0, 1, 0);         // Camera-relative "up" direction
    defocus_angle = 0.6;
    focus_dist = 10.0;

    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    pixel_samples_scale = 1.0 / samples_per_pixel;

    // std::clog << "width: " << ScreenWidth() << std::endl;
    // std::clog << "height: " << ScreenHeight() << std::endl;
    // std::clog << "width: " << image_width << std::endl;
    // std::clog << "height: " << image_height << std::endl;

    center = lookfrom;

    // Determine viewport dimensions.
    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta / 2);
    auto viewport_height = 2 * h * focus_dist;
    auto viewport_width =
        viewport_height * (double(image_width) / image_height);

    // Calculate the vectors across the horizontal and down the vertical
    // viewport edges.
    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    w = Vec3::unit_vector(lookfrom - lookat);
    u = Vec3::unit_vector(Vec3::cross(vup, w));
    v = Vec3::cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical
    // viewport edges.
    Vec3 viewport_u =
        viewport_width * u; // Vector across viewport horizontal edge
    Vec3 viewport_v =
        viewport_height * -v; // Vector down viewport vertical edge

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left =
        center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    auto defocus_radius =
        focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
      for (int b = -11; b < 11; b++) {
        auto choose_mat = random_double();
        point3 center(a + 0.9 * random_double(), 0.2,
                      b + 0.9 * random_double());

        if ((center - point3(4, 0.2, 0)).length() > 0.9) {
          shared_ptr<Material> sphere_material;

          if (choose_mat < 0.8) {
            // diffuse
            auto albedo = color::random() * color::random();
            sphere_material = make_shared<lambertian>(albedo);
            world.add(make_shared<Sphere>(center, 0.2, sphere_material));
          } else if (choose_mat < 0.95) {
            // metal
            auto albedo = color::random(0.5, 1);
            auto fuzz = random_double(0, 0.5);
            sphere_material = make_shared<metal>(albedo, fuzz);
            world.add(make_shared<Sphere>(center, 0.2, sphere_material));
          } else {
            // glass
            sphere_material = make_shared<dielectric>(1.5);
            world.add(make_shared<Sphere>(center, 0.2, sphere_material));
          }
        }
      }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    frame_count = 0;  // Inicializa contador de frames
    max_frames = 3; // Defina quantos frames deseja renderizar
    start_time = std::chrono::high_resolution_clock::now(); // Marca o início
    return true;
  }

  bool OnUserUpdate(float fElapsedTime) override {
    if (frame_count >= max_frames) {
      auto end_time = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
      std::cout << "Renderização completa em " << duration << " ms" << std::endl;
      return false;
    }

    for (int y = 0; y < image_height; y++)
      for (int x = 0; x < image_width; x++) {
        auto pixel_center =
            pixel00_loc + (x * pixel_delta_u) + (y * pixel_delta_v);
        auto ray_direction = pixel_center - center;
        Ray r(center, ray_direction);

        color pixel_color(0, 0, 0);
        for (int sample = 0; sample < samples_per_pixel; sample++) {
          Ray r = get_ray(x, y);
          pixel_color += ray_color(r, max_depth, world);
        }
        pixel_color *= pixel_samples_scale;
        static const interval intensity(0.000, 0.999);
        int rbyte =
            int(256 * intensity.clamp(linear_to_gamma(pixel_color.x())));
        int gbyte =
            int(256 * intensity.clamp(linear_to_gamma(pixel_color.y())));
        int bbyte =
            int(256 * intensity.clamp(linear_to_gamma(pixel_color.z())));
        Draw(x, y, olc::Pixel(rbyte, gbyte, bbyte));
      }

    DrawString(0, 0, "FPS: " + std::to_string(GetFPS()), olc::BLACK);

    frame_count++;  // Incrementa o contador de frames
    return true;
  }

private:
  int image_width;
  double aspect_ratio;
  int image_height;   // Rendered image height
  point3 center;      // Camera center
  point3 pixel00_loc; // Location of pixel 0, 0
  Vec3 pixel_delta_u; // Offset to pixel to the right
  Vec3 pixel_delta_v; // Offset to pixel below
  HittableList world;
  int samples_per_pixel;
  double pixel_samples_scale; // Color scale factor for a sum of pixel samples
  int max_depth;
  double vfov;
  point3 lookfrom = point3(0, 0, 0); // Point camera is looking from
  point3 lookat = point3(0, 0, -1);  // Point camera is looking at
  Vec3 vup = Vec3(0, 1, 0);          // Camera-relative "up" direction
  Vec3 u, v, w;
  double defocus_angle = 0;
  double focus_dist = 10;
  Vec3 defocus_disk_u;
  Vec3 defocus_disk_v;

  int frame_count;  // Contador de frames
  int max_frames;   // Número máximo de frames
  std::chrono::high_resolution_clock::time_point start_time; // Tempo de início

  Ray get_ray(int i, int j) const {
    // Construct a camera ray originating from the origin and directed at
    // randomly sampled point around the pixel location i, j.

    auto offset = Vec3::sample_square();
    auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) +
                        ((j + offset.y()) * pixel_delta_v);

    auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_direction);
  }

  color ray_color(const Ray &r, int depth, const Hittable &world) const {
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
      return color(0, 0, 0);

    HitRecord rec;

    if (world.hit(r, interval(0.001, infinity), rec)) {
      Ray scattered;
      color attenuation;
      if (rec.mat->scatter(r, rec, attenuation, scattered))
        return attenuation * ray_color(scattered, depth - 1, world);
      return color(0, 0, 0);
    }

    Vec3 unit_direction = Vec3::unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
  }

  point3 defocus_disk_sample() const {
    // Returns a random point in the camera defocus disk.
    auto p = Vec3::random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
  }
};

int main() {
  Example demo;
  if (demo.Construct(400, 225, 1, 1))
    demo.Start();

  return 0;
}
