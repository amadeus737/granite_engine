#pragma once

#define GraniteGame public Granite::Application
#define GRANITE_GAME_CREATE Granite::Application* Granite::CreateApplication() { return new Game(); }

#include <stdio.h>

#include "Granite/Application.h"
#include "Granite/EntryPoint.h"
#include "Granite/GameWindow.h"
