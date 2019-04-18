#pragma once

#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <glad/glad.h>

#include "Transform.h"
#include "Renderer.h"

//using namespace std;
//class Transform;
//class MeshRenderer;

class GameObject
{
public:
	bool Enabled;
	std::string Name;
	inline Transform& GetTransform() { return *(this->transform); }
	inline MeshRenderer& GetRenderer() { return *renderer; }

	virtual void Update();
	virtual void LateUpdate();

	GameObject(const std::string& name, std::unique_ptr<MeshRenderer>& r);

private:
	std::unique_ptr<Transform> transform;
	std::unique_ptr<MeshRenderer> renderer;
};

