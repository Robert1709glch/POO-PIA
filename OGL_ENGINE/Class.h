#ifndef CLASS_H
#define CLASS_H

#pragma once
#include <time.h>
#include <Windows.h>

#define TIMER 100

class ENEMIGO {
public:
	bool timeOut = false;
private:
	
};

class ENEMIGO1 : public ENEMIGO {
public:
	float ataque(float time) {
		
		if (timer != 0) {
			if (timer < 0) {
				timer = timer + 1;
				Sleep(9);
			}
			else {
				timer = timer - 1;
				Sleep(9);
			}
		}
		else {
			if (timer == 0) {
				timeOut = true;
				return timer;
			}
			else {
				return timer;
			}
		}
	}
private:
	
};

class ENEMIGO2 : public ENEMIGO {
public:
	float ataque(float time) {

		if (timer != 0) {
			if (timer < 0) {
				timer = timer + 1;
				Sleep(4);
			}
			else {
				timer = timer - 1;
				Sleep(4);
			}
		}
		else {
			if (timer == 0) {
				timeOut = true;
				return timer;
			}
			else {
				return timer;
			}
		}
	}
private:
	
};

//class enemy {
//public:
//	int attack = 1;
//	void isInTime() {
//		touchPlayer = true;
//	}
//private:
//	bool touchPlayer = false;
//};
//
//class player : public enemy {
//public:
//	int life = 1;
//	int loseLife(int attack, int life) {
//		life = life - attack;
//		return life;
//	}
//	void isInTime(int life) {
//		if (life == 0) {
//			isDead = true;
//		}
//	}
//private:
//	bool isDead = false;
//};
//
//class enemigo : public Camera{
//public:
//	glm::vec3(posicionEnemigo) = enemy.getPosition();
//	glm::vec3(posicionCamara) = camera.Position;
//	glm::vec3 acercar(vec3 enemigo, vec3 jugador) {
//		glm::vec3(nuevaPosicion) = enemigo - (jugador / glm::vec3(2, 0, 2));
//		return nuevaPosicion;
//	}
//};
#endif