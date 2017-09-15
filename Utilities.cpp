#include "Utilities.h"

Utilities::Utilities() {
	lastTime = 0;
	deltaTime = 0;
	nowTime = 0;
}

void Utilities::initTimeCounting() {
	lastTime = glfwGetTime();
}

void Utilities::measureTime() {
	nowTime = glfwGetTime();
	deltaTime += (nowTime - lastTime);
	lastTime = nowTime;
}

void Utilities::resetTime() {
	//lastTime = 0;
	deltaTime = 0;
	nowTime = 0;
}

float Utilities::rand_FloatRange(float a, float b)
{
	return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

glm::vec3 Utilities::generateRandomVector()
{
	return glm::vec3(rand_FloatRange(-5.0, 5.0), rand_FloatRange(2.5, 4.5), rand_FloatRange(-3.0, 3.0));
}

void Utilities::initRandomDataBillboards(int number_entries)
{
	float _aux;
	for (int i = 0; i < number_entries; i++)
	{
		_aux = rand_FloatRange(0.4, 5.0);
		billboardRandomData _bbrd = billboardRandomData(generateRandomVector(), glm::vec3(_aux), rand_FloatRange(1.0, 3.0));
		billbRandomDataList.push_back(_bbrd);
	}
}
