#pragma once

#include "integrator.hpp"

class PathTracer : public Integrator {
 public:
  PathTracer(const Camera& cam, const int samples, const int width,
             const int height)
      : cam(cam), width(width), height(height), samples(samples) {}

  float3 render(Scene& scene, const int& c, const int& r, uint& seed) {
    float3 color;
    for (int s = 0; s < samples; s++) {
      float u = float(c + rnd(seed)) / float(width);
      float v = float(r + rnd(seed)) / float(height);

      Ray ray = cam.make_ray(u, v, seed);
      color += trace(ray, scene, seed);
    }

    return color / float(samples);
  }

 private:
  float3 trace(Ray& r, Scene& scene, uint& seed) {
    float3 color = float3(0.0f);
    float3 throughput = float3(1.0f);
    bool wasSpecular = false;

    // iterative version of recursion
    for (int depth = 0;; depth++) {
      Hit_Record rec;

      // ray hit the scene
      if (scene.geometry->hit(r, rec, 0.001f, FLT_MAX)) {
        // gets hit values
        float3 p = rec.p;            // next origin
        float3 wo = -r.direction();  // current direction
        float3 n = rec.n;            // shading normal

        // get next direction
        float3 wi;
        if (rnd(seed) < 0.5f)
          wi = normalize(rec.brdf->sample(p, wo, n, seed));
        else
          wi = scene.lights.sample(p, wo, n, seed);

        // pdfs are obtained on brdf evaluate and light sample functions
        float pdfL = scene.lights.pdf(p, wo, wi, n), pdfB = 1.0f;

        // if we hit a light source, return color emitted
        if (rec.brdf->isLight()) {
          color += throughput * rec.brdf->eval(p, wo, wi, n, rec.uv, pdfB);
          break;
        }

        // accumulate brdf attenuation
        throughput *= rec.brdf->eval(p, wo, wi, n, rec.uv, pdfB) / PI;

        // use one pdf or the other
        float pdf = (pdfB + pdfL) / 2;
        throughput *= (1.0f / pdf);

        // Russian Roulette Path Termination
        float prob = max_component(throughput);
        if (depth > 10) {  // min ray depth
          if (rnd(seed) >= prob) {
            break;
          } else {
            // add the energy we 'lose' by terminating paths
            throughput *= 1.0f / prob;
          }
        }

        // shoots new ray
        wasSpecular = rec.brdf->isSpecular();
        r = Ray(p, wi, r.time());
      }

      // ray missed, sample background and return
      else {
        color += throughput * scene.environment->sample(r);
        return color;
      }
    }

    return color;
  }

  Camera cam;
  const int samples;
  const int width;
  const int height;
};
