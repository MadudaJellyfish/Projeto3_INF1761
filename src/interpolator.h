#pragma once
#include <memory>
#include <glm/glm.hpp>

#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

class Interpolator;
using InterpolatorPtr = std::shared_ptr<Interpolator>;

class Interpolator {
public:
	virtual ~Interpolator() {}
	virtual glm::vec3 Interpolate(float t) = 0;
};


#endif 


