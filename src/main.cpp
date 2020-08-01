
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include "integrator/pathtracer.hpp"
#include "utils/framebuffer.hpp"

int main() {
  const int samples = 200;
  stbi_flip_vertically_on_write(1);
  unsigned int seed = tea<64>(2, 5);
  const int width = 500, height = 500;

  Framebuffer fb(width, height);
  Camera cam;
  Scene scene = Cornell(cam, width, height, seed);

  Integrator* integrator = &PathTracer(cam, samples, width, height);

  auto t0 = std::chrono::system_clock::now();

  // render loop
  for (int r = 0; r < fb.height(); r++) {
    for (int c = 0; c < fb.width(); c++) {
      float3 color = integrator->render(scene, c, r, seed);
      fb.set_pixel(r, c, sqrt(color));  // gamma correction
    }
    printf("Progress: %.2f \r", 100 * (float(r) / height));
  }
  printf("\n");

  // render time
  auto t1 = std::chrono::system_clock::now();
  auto time = std::chrono::duration<float>(t1 - t0).count();
  printf("Rendering time: %2.fs\n", float(time));

  // save image to file
  std::string filename = "output" + std::to_string(samples) + ".png";
  fb.save(filename.c_str());

  system("pause");
  return 0;
}