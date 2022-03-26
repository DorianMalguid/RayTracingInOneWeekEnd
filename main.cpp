#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <chrono>
#include <thread>

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
const int IMAGE_WIDTH = 1200;
const int IMAGE_HEIGHT = 600;
const int RAYS_PER_PIXEL = 20;
const int NB_THREADS = 10;


RandomGen* rdgen = RandomGen::getInstance();


// Lance un rayon dans le monde, il peut rebondir jusqu’à depth fois
Vec3 rayColour(const Ray& ray, Hitable* world, int depth) {
    hit_record record;
    if (world->hit(ray, 0.001f, FLT_MAX, record)) {
        Ray scattered_ray;
        Vec3 attenuation;
        if (depth < MAX_REFLECTION_DEPTH && record.material_ptr->scatter(ray, record, attenuation, scattered_ray)) {
            return attenuation * rayColour(scattered_ray, world, depth + 1);
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


// Génère une scène aléatoire avec des boules composées des trois matériaux différents
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


// Lance les rayons dans le monde et agrège les résultats dans le tableau colour_buf
void ray_cast(Hitable* world, float* colour_buf, Camera camera) {
    int position = 0;
    int J;
    for (int j = IMAGE_HEIGHT - 1; j >= 0; j--) {
        for (int i = 0; i < IMAGE_WIDTH; i++) {

            Vec3 couleur(0, 0, 0);
            for (int k = 0; k < RAYS_PER_PIXEL; k++)
            {
                float u = float(i + rdgen->random()) / float(IMAGE_WIDTH);
                float v = float(j + rdgen->random()) / float(IMAGE_HEIGHT);
                Ray r = camera.getRay(u, v);

                couleur += rayColour(r, world, 0);
            }

            J = IMAGE_HEIGHT - j - 1;
            colour_buf[position++] += couleur[0];
            colour_buf[position++] += couleur[1];
            colour_buf[position++] += couleur[2];
        }
    }
}


// Écriture du fichier image
void write_image_file(float* colour_buf, int nb_col) {
    ofstream out("image.ppm");
    streambuf* console_buf = cout.rdbuf();
    cout.rdbuf(out.rdbuf());


    cout << "P3" << endl << IMAGE_WIDTH << " " << IMAGE_HEIGHT << endl << "255" << endl;

    int i = 0;
    int pr = 0;
    int pg = 0;
    int pb = 0;
    int px_corrompus = 0;
    while (i < nb_col) {
        int ir = int(255.99 * sqrt(colour_buf[i++] / (NB_THREADS * RAYS_PER_PIXEL)));
        int ig = int(255.99 * sqrt(colour_buf[i++] / (NB_THREADS * RAYS_PER_PIXEL)));
        int ib = int(255.99 * sqrt(colour_buf[i++] / (NB_THREADS * RAYS_PER_PIXEL)));

        // Parfois des pixels sont corrompus (~1/100 000) donc dans ce cas on reprend la valeur précédente
        // pour pouvoir ouvrir l’image quand même
        if (ir < 0 || ir > 255 || ig < 0 || ig > 255 || ib < 0 || ib > 255) {
            ir = pr;
            ig = pg;
            ib = pb;
            px_corrompus++;
        }

        cout << ir << " " << ig << " " << ib << endl;
    }


    out.close();
    cout.rdbuf(console_buf);
    cout << px_corrompus << " pixels corrompus réécrits" << endl;
}



// Main
int main()
{
    milliseconds startTime, stopTime;


    // Génération de la scène
    cout << "Génération de la scène" << endl;
    startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    Hitable* world = random_scene();

    stopTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "Fini en " << float(stopTime.count() - startTime.count()) / 1000 << "s" << endl << endl;


    // Caméra
    Vec3 lookfrom(13, 2, 3);
    Vec3 lookat(0, 0, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;

    Camera camera(lookfrom, lookat, Vec3(0, 1, 0), 20, float(IMAGE_WIDTH) / float(IMAGE_HEIGHT), aperture, dist_to_focus);


    // On initialise un tableau de flottants dans lequel les threads pourront actualiser les couleurs calculées
    cout << "Initialisation buffer image" << endl;
    startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    float* colours = new float[3 * IMAGE_WIDTH * IMAGE_HEIGHT];
    for (int i = 0; i < 3 * IMAGE_WIDTH * IMAGE_HEIGHT; i++)
    {
        colours[i] = 0;
    }

    stopTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "Fini en " << float(stopTime.count() - startTime.count()) / 1000 << "s" << endl << endl;



    // Lancer de rayons
    cout << "Rendu de la scène" << endl;
    startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    vector<thread> threads;
    for (int i = 0; i < NB_THREADS; i++)
    {
        threads.emplace_back(ray_cast, world, colours, camera);
    }
    for (auto &th : threads)
    {
        th.join();
    }
    //ray_cast(world, colours, camera);

    stopTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "Fini en " << float(stopTime.count() - startTime.count()) / 1000 << "s" << endl << endl;



    // Écriture du fichier ppm
    cout << "Écriture de l’image" << endl;
    startTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    write_image_file(colours, 3 * IMAGE_WIDTH * IMAGE_HEIGHT);

    stopTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "Fini en " << float(stopTime.count() - startTime.count()) / 1000 << "s" << endl << endl;



    return 0;
}