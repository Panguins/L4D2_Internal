#pragma once
#include "prediction.h"
#include "movement.h"
#include "aimbot.h"
#include "accuracy.h"
#include "esp.h"
#include "speedhack.h"
#include "fakelag.h"

class c_cheat
{
public:
	//extra
	c_prediction prediction;
	c_movement movement;
	c_speedhack speedhack;
	c_fakelag fakelag;

	//aim
	c_aimbot aimbot;
	c_accuracy accuracy;

	//visual
	c_esp esp;

}; extern c_cheat g_cheat;