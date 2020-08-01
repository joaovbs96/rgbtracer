#pragma once

// textures
#include "../texture/checker.hpp"
#include "../texture/color.hpp"
#include "../texture/image.hpp"

// materials
#include "../brdf/diffuse_light.hpp"
#include "../brdf/glass.hpp"
#include "../brdf/lambertian.hpp"
#include "../brdf/metal.hpp"

// geometry
#include "../geometry/box.hpp"
#include "../geometry/bvh_node.hpp"
#include "../geometry/geometry_list.hpp"
#include "../geometry/rectangle.hpp"
#include "../geometry/sphere.hpp"

// transform nodes
#include "../geometry/flip_normals.hpp"
#include "../geometry/motion_blur.hpp"
#include "../geometry/rotation.hpp"
#include "../geometry/translate.hpp"

// lights
#include "../light/area_light.hpp"
#include "../light/light_list.hpp"

// others
#include "../sampling/rng.hpp"
#include "../utils/camera.hpp"
#include "environment.hpp"

class Scene {
 public:
  Scene() {}

  void add(Geometry* obj) { list.add(obj); }

  void add(Light* light) {
    Geometry* geo = light->getGeometry();
    lights.add(light);
    list.add(light->getGeometry());
  }

  void buildBVH(float t0 = 0.0f, float t1 = 1.0f) {
    geometry = new BVHNode(list, t0, t1);
  }

  float3 sample_direct(const float3& p,   // next ray origin
                       const float3& wo,  // previous ray direction
                       const float3& n,   // surface normal
                       Brdf* brdf,        // surface material
                       float2 uv, uint& seed) {
    float3 direct_light(0.0f);

    // returns black if we don't have lights in the scene
    int nLights = lights.size();
    if (nLights == 0) return float3(0.0f);

    // ramdomly pick one light and multiply the result by the number of lights
    // it's the same as dividing by the PDF if they have the same probability
    int light = ((int)(rnd(seed) * nLights)) % nLights;

    // return black if there's just one light and we just hit it
    bool isLight = brdf->isLight();
    if (isLight && nLights == 1) return float3(0.0f);

    // Sample Light
    // only sample if surface normal is in the light direction
    float3 wi = lights[light]->sample(p, wo, n, seed);
    if (dot(wi, n) < 0.0f) return float3(0.0f);

    float lightpdf;
    float3 emission = lights[light]->emission(p, wo, wi, n, uv, lightpdf);
    lightpdf = lights[light]->pdf(p, wo, wi, n);

    // check if light is occluded
    Hit_Record rec;
    Ray shadowRay(p, wi);
    if (geometry->hit(shadowRay, rec, 0.001f, FLT_MAX)) {
      // if light is occluded, return black
      if (rec.brdf->isLight()) {
        // nothing
      } else {
        return float3(0.0f);
      }
    }

    // Multiple Importance Sampling

    // sample light
    if (lightpdf != 0.0f && !isNull(emission)) {
      float matpdf;
      float3 matcolor = brdf->eval(p, wo, wi, n, uv, matpdf);

      if (matpdf != 0.0f && !isNull(matcolor)) {
        float weight = PowerHeuristic(1, lightpdf, 1, matpdf);
        direct_light += matcolor * emission * weight / lightpdf;
      }
    }

    // sample brdf
    wi = brdf->sample(p, wo, n, seed);
    float matpdf;
    float3 matcolor = brdf->eval(p, wo, wi, n, uv, matpdf);

    if (matpdf != 0.0f && !isNull(matcolor)) {
      lightpdf = lights[light]->pdf(p, wo, wi, n);

      // if we didn't hit anything, ignore the brdf sample
      if (!lightpdf || isNull(emission)) return direct_light;

      float weight = PowerHeuristic(1, matpdf, 1, lightpdf);
      direct_light += matcolor * emission * weight / matpdf;
    }

    return direct_light;
  }

  LightList lights;
  Geometry* geometry;
  Environment* environment;

 private:
  GeometryList list;
};

Scene In_One_Weekend(Camera& cam, int width, int height, uint& seed) {
  Scene scene;

  Texture* checker = new Checker(new Color(0.2f, 0.3f, 0.1f), new Color(0.9f));
  Brdf* ground = new Lambertian(new Image("earthmap.jpg"));
  scene.add(new Sphere(float3(0.f, -1000.f, -1.f), 1000.f, ground));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_mat = rnd(seed);
      float3 center = float3(a + rnd(seed), 0.2f, b + rnd(seed));
      if (choose_mat < 0.8f) {
        Brdf* mt = new Lambertian(new Color(rnd(seed), rnd(seed), rnd(seed)));
        scene.add(new Sphere(center, 0.2f, mt));
      } else if (choose_mat < 0.95f) {
        Brdf* mt = new Metal(new Color(0.5f * (1.f + rnd(seed)),   // r
                                       0.5f * (1.f + rnd(seed)),   // g
                                       0.5f * (1.f + rnd(seed))),  // b
                             0.5f * rnd(seed));                    // fuzz
        scene.add(new Sphere(center, 0.2f, mt));
      } else {
        Brdf* mt = new Glass(new Color(1.0f), 1.5);
        scene.add(new Sphere(center, 0.2f, mt));
      }
    }
  }

  Brdf* mt0 = new Glass(new Color(1.0f), 1.5);
  scene.add(new Sphere(float3(0.0f, 1.0f, 0.0f), 1.f, mt0));

  Brdf* mt2 = new Lambertian(new Color(0.4f, 0.2f, 0.1f));
  scene.add(new Sphere(float3(-4.f, 1.f, 0.f), 1.f, mt2));

  Brdf* mt3 = new Metal(new Image("earthmap.jpg"), 0.f);
  scene.add(new Sphere(float3(4.f, 1.f, 0.f), 1.f, mt3));

  scene.buildBVH();

  const float3 lookfrom(13.0f, 2.0f, 3.0f);
  const float3 lookat(0.0f);
  const float fovy(40.0f);
  const float aspect(float(width) / height);
  const float aperture(0.1f);
  const float dist(10.0f);
  cam = Camera(lookfrom, lookat, fovy, aspect, aperture, dist);

  scene.environment = new GradientBackground(float3(1.0f),               // c1
                                             float3(0.5f, 0.7f, 1.0f));  // c2

  return scene;
}

Scene Light_Test(Camera& cam, int width, int height, uint& seed) {
  Scene scene;

  Brdf* light = new DiffuseLight(new Color(15.0f));

  Geometry* geom = new Sphere(float3(4.f, 1.f, 0.f), 1.f, light);
  scene.add(new AreaLight(geom));

  Geometry* geom2 = new ZRect(3.0f, 5.0f, 1.0f, 3.0f, -2.0f, light);
  scene.add(new AreaLight(geom2));

  Texture* checker = new Checker(new Color(0.2f, 0.3f, 0.1f), new Color(0.9f));
  Brdf* ground = new Lambertian(new Image("earthmap.jpg"));
  scene.add(new Sphere(float3(0.f, -1000.f, -1.f), 1000.f, ground));

  Brdf* mt2 = new Lambertian(new Image("earthmap.jpg"));
  scene.add(new Sphere(float3(4.f, 1.f, 0.f), 1.f, mt2));

  scene.buildBVH();

  const float3 lookfrom(13.0f, 2.0f, 3.0f);
  const float3 lookat(0.0f);
  const float fovy(40.0f);
  const float aspect(float(width) / height);
  const float aperture(0.1f);
  const float dist(10.0f);
  cam = Camera(lookfrom, lookat, fovy, aspect, aperture, dist);

  scene.environment = new ColorBackground(float3(0.0f));

  return scene;
}

Scene Cornell(Camera& cam, int width, int height, uint& seed) {
  Scene scene;

  // brdfs
  Brdf* red = new Lambertian(new Color(1.0f, 0.0f, 0.0f));
  Brdf* green = new Lambertian(new Color(0.0f, 1.0f, 0.0f));
  Brdf* white = new Lambertian(new Color(1.0f, 1.0f, 1.0f));
  Brdf* light = new DiffuseLight(new Color(15.0f));

  // light
  Geometry* l_geom = new YRect(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, light);
  scene.add(new AreaLight(l_geom));

  // Geometry
  // Walls
  scene.add(new Flip(new XRect(0, 555, 0, 555, 555, green)));  // x
  scene.add(new XRect(0, 555, 0, 555, 0, red));                // x
  scene.add(new Flip(new YRect(0, 555, 0, 555, 555, white)));  // y
  scene.add(new YRect(0, 555, 0, 555, 0, white));              // y
  scene.add(new Flip(new ZRect(0, 555, 0, 555, 555, white)));  // z
  // Boxes
  Geometry* box1 = new Box(float3(0), float3(165, 330, 165), white);
  scene.add(new Translate(new YRotation(box1, 15.f), float3(265, 0, 295)));
  Geometry* box2 = new Box(float3(0), float3(165, 165, 165), white);
  scene.add(new Translate(new YRotation(box2, -18.f), float3(130, 0, 65)));

  scene.buildBVH();

  // Camera
  const float3 lookfrom(278.0f, 278.0f, -800.0f);
  const float3 lookat(278.0f, 278.0f, 0.0f);
  const float fovy(40.0f);
  const float aspect(float(width) / height);
  const float aperture(0.0f);
  const float dist(10.0f);
  cam = Camera(lookfrom, lookat, fovy, aspect, aperture, dist);

  scene.environment = new ColorBackground(float3(0.0f));

  return scene;
}