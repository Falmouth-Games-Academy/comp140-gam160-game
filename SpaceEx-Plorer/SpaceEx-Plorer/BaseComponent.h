#pragma once

// Scoring component: attached to any SObjects that require scoring functionality

class BaseComponent
{
public:
	BaseComponent() {};
	~BaseComponent() {};

public:
	int getScore() { return score; }

public:
	static int score;
};

class ProjectileComponent : public BaseComponent
{
public:
	ProjectileComponent() {};
	~ProjectileComponent() {};
public:
	int damage;
	int speed;
};

class EnemyComponent : public BaseComponent
{
public:
	EnemyComponent() {};
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
	PlayerComponent() {};
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