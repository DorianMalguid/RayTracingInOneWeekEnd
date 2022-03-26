#include "Hitable_list.h"

Hitable_list::Hitable_list() {}

Hitable_list::Hitable_list(Hitable** _list, int _list_size) {
	list = _list;
	list_size = _list_size;
}

bool Hitable_list::hit(const Ray& ray, float t_min, float t_max, hit_record& record) const {
	hit_record temp;
	bool hit_anything = false;
	double closest = t_max;
	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(ray, t_min, closest, temp)) {
			hit_anything = true;
			record = temp;
			closest = temp.t;
		}
	}
	return hit_anything;
}