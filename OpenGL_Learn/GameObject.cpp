#include "GameObject.h"

#include <iostream>

#include "Time.h"
#include "Input.h"

using namespace std;

void GameObject::Update()
{
	if (Name == "Object")
	{
		//GetTransform().Rotate(V3_RIGHT + V3_UP, Time::DeltaTime() * 180);
		//GetTransform().Translate(V3_UP* Time::DeltaTime() * 0.2f);
	}
	else if (Name == "DiMian")
	{
		//GetTransform().Rotate(V3_UP, Time::DeltaTime() * 90, true);
	}

}

void GameObject::LateUpdate()
{
}

GameObject::GameObject(const string& name, unique_ptr<MeshRenderer> & r) :Name(name), Enabled(true), renderer(move(r))
{
	this->transform = make_unique<Transform>();
}