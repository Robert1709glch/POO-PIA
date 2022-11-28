#pragma once

class enemy {
public://ENCAPSULAMIENTO
	int attack = 1;//ABSTRACCION
	void isInTime(/*receive time remain*/) {//POLIMORFISMO[1]
		touchPlayer = true;
	}
private:
	bool touchPlayer = false;
};

class player : public enemy {//HERENCIA
public:
	int life = 1;
	int loseLife(int attack, int life) {
		life = life - attack;
		return life;
	}
	void isInTime(/*receivet the time remain*/int life) {//[2]
		if (life == 0) {
			isDead = true;
		}
	}
private:
	bool isDead = false;
};
