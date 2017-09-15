# pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

struct billboardRandomData
{
	glm::vec3 _position;
	glm::vec3 _scale;
	float _distance;

	billboardRandomData(glm::vec3 _position, glm::vec3 _scale, float _distance)
	{
		this->_position = _position; this->_scale = _scale; this->_distance = _distance;
	}
};

class Utilities
{

public:
	double lastTime;
	double deltaTime;
	double nowTime;
	std::vector<billboardRandomData> billbRandomDataList;


	Utilities();
	void initTimeCounting();
	void measureTime();
	void resetTime();
	float rand_FloatRange(float a, float b);
	glm::vec3 generateRandomVector();
	
	void initRandomDataBillboards(int number_entries);
};