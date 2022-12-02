#pragma once

class enemy {
public:
	int attack = 1;
	void isInTime() {
		touchPlayer = true;
	}
private:
	bool touchPlayer = false;
};

class player : public enemy {
public:
	int life = 1;
	int loseLife(int attack, int life) {
		life = life - attack;
		return life;
	}
	void isInTime(int life) {
		if (life == 0) {
			isDead = true;
		}
	}
private:
	bool isDead = false;
};
