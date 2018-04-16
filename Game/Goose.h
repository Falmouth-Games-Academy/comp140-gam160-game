#pragma once
#include "Object.h"

class Goose : public Object {
public:
	void OnSpawn() override;

	void Update(float deltaTime) override;

	Object::Type GetType() const override {return GooseType;}

private:
	enum State {
		Idle,
		PrepareCharge,
		Charge,
	};

	State state;
};