#pragma once

// Scoring component: attached to any SObjects that require scoring functionality

enum ComponentType
{
	Player,
	Enemy,
	Projectile,
	Score
};

class BaseComponent
{
public:
	BaseComponent() {};
	~BaseComponent() {};

public:
	int getScore() { return score; }

public:
	static int score;

	ComponentType type;
};

class ProjectileComponent : public BaseComponent
{
public:
	ProjectileComponent() { type = ComponentType::Projectile; };
	~ProjectileComponent() {};
public:
	int damage;
	int speed;
};

class EnemyComponent : public BaseComponent
{
public:
	EnemyComponent() { type = ComponentType::Enemy; };
	~EnemyComponent() {};

	void hit(ProjectileComponent * projectile)
	{
		health -= projectile->damage;

		if (health <= 0)
		{
			score += points;
		}
	}
public:
	// Hit points
	int health;
	// Points rewarded for destroying enemy
	int points;
};

class PlayerComponent : public BaseComponent
{
public:
	PlayerComponent() { type = ComponentType::Player; };
	~PlayerComponent() {};

	void hit(ProjectileComponent * projectile)
	{
		health -= projectile->damage;

		if (health <= 0)
		{
			end();
		}
	}

	void end()
	{

	}

public:
	int health;
};