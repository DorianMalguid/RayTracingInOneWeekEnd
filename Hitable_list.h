#pragma once

#include "Hitable.h"

class Hitable_list : public Hitable
{
private:
	Hitable** list;
	int list_size;
	
public:
	Hitable_list();
	Hitable_list(Hitable** _list, int _list_size);
	virtual bool hit(const Ray& ray, float t_min, float t_max, hit_record& record) const;
};

