#pragma once

#include "Engine.h"
#include "KeyEnum.h"
#include "Window.h"

#include <map>
#include <vector>
#include <functional>

class InputManager
{

	Window* window;

	struct ActionMapping
	{
		// default constructor
		explicit ActionMapping(InputManager* manager = nullptr, const std::vector<Keyboard>& keys = std::vector<Keyboard>())
			: manager(manager),
			keysUsed(keys)
		{
			
		}

		~ActionMapping()
		{
			if (pressed)
				delete pressed;

			if (released)
				delete released;
		}

		void setPressedCallback(const std::function<void()>& fun)
		{
			pressed = new std::function<void()>(fun);
		}

		void setReleasedCallback(const std::function<void()>& fun)
		{
			released = new std::function<void()>(fun);
		}

		InputManager* manager;

		std::vector<Keyboard> keysUsed;

		// function pointers -- they are pointers to make sure they are all allocated and freed in the same module.
		std::function<void()>* pressed;
		std::function<void()>* released;

		bool isPressed;

		void operator()()
		{
			// if neither of the events exist, return.
			if (!pressed && !released)
			{
				return;
			}

			// loop through the keys until we get one that is pressed
			bool isPressedNew = false;
			for (auto& elem : keysUsed)
			{
				// if there is one that is pressed, the entire binding is pressed,
				// so break
				if (manager->window->getIsKeyPressed(elem))
				{
					isPressedNew = true;
					break;
				}
			}

			// if there is no change, return
			if (isPressedNew == isPressed) return;

			// if isPressedNew is true and isPressed is false, then we just pressed the key.
			// also checks the pressed function pointer
			if (isPressedNew && !isPressed && pressed) (*pressed)();

			if (!isPressedNew && isPressed && released) (*released)();
		}

	};

	struct AxisMapping
	{
		// vector of the values of the keys
		std::vector < std::pair<Keyboard, float> > values;


	};

	friend AxisMapping;
	friend ActionMapping;
	
};