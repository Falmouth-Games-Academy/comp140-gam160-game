#pragma once

class Object {
public:
	enum Type {
		Hand = 0,
		NumTypes
	};

	virtual Type GetType() const = 0;
};