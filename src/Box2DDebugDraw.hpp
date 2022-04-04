// Game Programming 2020
// Exam Project: John the Fisherman
// Inspired by Box2DDebugDraw by Morten Nobel-J�rgensen - 2017

#pragma once
#include <Box2D/Common/b2Draw.h>
#include <vector>
#include <glm/vec3.hpp>

class Box2DDebugDraw : public b2Draw
{
public:
	Box2DDebugDraw(float scale);

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;

    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

    void DrawTransform(const b2Transform& xf) override;

    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override;

    void clear();

    const std::vector<glm::vec3>& getLines();
private:
	std::vector<glm::vec3> lines;
	float scale;
};