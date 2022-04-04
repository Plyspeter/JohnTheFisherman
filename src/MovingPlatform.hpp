// Game Programming 2020
// Exam Project: John the Fisherman

#include "Component.hpp"

class MovingPlatform : public Component
{
public:
	explicit MovingPlatform(GameObject* gameObject);

	void setEnd(glm::vec2 position);
	void setSpeed(float speed);
	void update(float deltaTime) override;
	void read(rapidjson::Value& component) override;
	void write(rapidjson::Value& c, rapidjson::Document::AllocatorType& a) override;

private:
	bool reverse = false;
	float total = 0;
	float speed = 1.0f;

	glm::vec2 start = glm::vec2{ 0, 0 };
	glm::vec2 end = glm::vec2{ 0, 0 };
	glm::vec2 relativeEnd;
};