#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <chrono>

#include "Vec3.h"
#include "Ray.h"
#include "Hitable_list.h"
#include "Sphere.h"
#include "Camera.h"
#include "RandomGen.h"
#include "Material.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"

using namespace std;
using namespace chrono;



const int MAX_REFLECTION_DEPTH = 50;


RandomGen* rdgen = RandomGen::getInstance();



Vec3 rayColour(const Ray& ray, Hitable* world, int depth) {
    hit_record record;
    if (world->hit(ray, 0.001f, FLT_MAX, record)) {
        Ray scattered_ray;
        Vec3 attenuation;
        if (depth < MAX_REFLECTION_DEPTH && record.material_ptr->scatter(ray, record, attenuation, scattered_ray)) {
            Vec3 res = attenuation * rayColour(scattered_ray, world, depth + 1);
            if (isnan(res.x()) || isnan(res.y()) || isnan(res.z())) {
                float c = 0;
            }
            return res;
        }
        else {
            return Vec3(0, 0, 0);
        }
    }
    else {
        Vec3 directionUnitaire = ray.getDirection().unit_vector();
        float t = 0.5f * (1.f + directionUnitaire.y());
        return (1 - t) * Vec3(1, 1, 1) + t * Vec3(0.5f, 0.7f, 1);
    }
}



Hitable* random_scene() {
    int n = 300;
    Hitable** list = new Hitable *[n + 1];
    list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5f, 0.5f, 0.5f)));

    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_mat = rdgen->random();
            Vec3 center(a + 0.9f * rdgen->random(), 0.2f, b + 0.9f * rdgen->random());
            if ((center - Vec3(4, 0.2f, 0)).norm() > 0.9f && i < n-2) {
                if (choose_mat < 0.8f) {
                    list[i++] = new Sphere(center, 0.2f, new Lambertian(Vec3(rdgen->random() * rdgen->random(), rdgen->random() * rdgen->random(), rdgen->random() * rdgen->random())));
                }
                else if (choose_mat < 0.95f) {
                    list[i++] = new Sphere(center, 0.2f, new Metal(Vec3(0.5f * (1 + rdgen->random()), 0.5f * (1 + rdgen->random()), 0.5f * (1 + rdgen->random())), 0.5f * rdgen->random()));
                }
                else {
                    list[i++] = new Sphere(center, 0.2f, new Dielectric(1.5f));
                }
            }
        }
    }

    list[i++] = new Sphere(Vec3(0, 1, 0), 1, new Dielectric(1.5f));
    list[i++] = new Sphere(Vec3(-4, 1, 0), 1, new Lambertian(Vec3(0.4f, 0.2f, 0.1f)));
    list[i++] = new Sphere(Vec3(4, 1, 0), 1, new Metal(Vec3(0.7f, 0.6f, 0.5f), 0));
    
    return new Hitable_list(list, i);
}




int main()
{

    int nx = 1200;
    int ny = 600;
    int rays_per_pixel = 200;

    ofstream out("image.ppm");
    streambuf* console_buf = cout.rdbuf();
    cout.rdbuf(out.rdbuf());

    milliseconds startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    cout << "P3" << endl <<  nx << " " << ny << endl << "255" << endl;


    //Hitable* list[4];
    //list[0] = new Sphere(Vec3(0, 0, -1), 0.5f, new Lambertian(Vec3(0.8f, 0.3f, 0.3f)));
    //list[1] = new Sphere(Vec3(0, -100.5f, -1), 100, new Lambertian(Vec3(0.8f, 0.8f, 0.0f)));
    //list[2] = new Sphere(Vec3(1, 0, -1), 0.5f, new Metal(Vec3(0.8f, 0.6f, 0.2f), 0));
    //list[3] = new Sphere(Vec3(-1, 0, -1), 0.5f, new Dielectric(1.5f));
    ////list[3] = new Sphere(Vec3(-1, 0, -1), -0.45f, new Dielectric(1.5f));
    //Hitable_list* world = new Hitable_list(list, 4);

    Hitable* world = random_scene();

    Vec3 lookfrom(13, 2, 3);
    Vec3 lookat(0, 0, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;

    Camera camera(lookfrom, lookat, Vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);
    
    /*float R = cos(3.14159265358979f / 4);
    Hitable* list[2];
    list[0] = new Sphere(Vec3(-R, 0, -1), R, new Lambertian(Vec3(0, 0, 1)));
    list[1] = new Sphere(Vec3(R, 0, -1), R, new Lambertian(Vec3(1, 0, 0)));
    Hitable* world = new Hitable_list(list, 2);
    Camera camera(90, float(nx) / float(ny));*/

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {

            Vec3 couleur(0, 0, 0);
            for (int k = 0; k < rays_per_pixel; k++)
            {
                float u = float(i + rdgen->random()) / float(nx);
                float v = float(j + rdgen->random()) / float(ny);
                Ray r = camera.getRay(u, v);
                Vec3 p = r.point_at_parameter(2.0f);
                couleur += rayColour(r, world, 0);
            }
            couleur /= rays_per_pixel;

            couleur = Vec3(sqrt(couleur[0]), sqrt(couleur[1]), sqrt(couleur[2]));

            int ir = int(255.99 * couleur.r());
            int ig = int(255.99 * couleur.g());
            int ib = int(255.99 * couleur.b());

            if (ir < 0 || ig < 0 || ib < 0) {
                rdgen->random();
            }
            cout << ir << " " << ig << " " << ib << endl;
        }
    }

    milliseconds stopTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    out.close();

    cout.rdbuf(console_buf);
    cout << "Temps d'exécution : " << float(stopTime.count() - startTime.count()) / 1000 << "s" << endl;

    return 0;
}