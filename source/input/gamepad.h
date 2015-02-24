#pragma once

#ifdef WIN32

// OSG
#include <osg/ref_ptr>
// troen
#include "pollingdevice.h"
#include "../forwarddeclarations.h"
//other
#define _AMD64_
#include <xinput.h>
#include <vector>


namespace troen
{
namespace input
{
	/*! The Gamepad class is responsible for receiving input from a X-Box 360 controller. Additionally, it can control the vibration of the controller.*/
	class Gamepad : public PollingDevice
	{
	public:
		Gamepad(osg::ref_ptr<BikeInputState> bikeInputState) : PollingDevice(bikeInputState) {
			m_deadzoneX = 0.2f;
			m_deadzoneY = 0.2f;
			m_isConnected = false;
			m_controllerId = -1;
		};
		~Gamepad();
		XINPUT_GAMEPAD* getState();
		bool checkConnection();
		void run() override;
		bool isPressed(const unsigned short button);

		void vibrate();
		int getPort();
		static std::vector <int>* getFreePorts();
		static std::vector <int> freePorts;
		static void clearPorts();
		virtual InputDevice getType() { return InputDevice::GAMEPAD; }
	private:
		int m_controllerId, m_isConnected;
		XINPUT_STATE m_state;
		float m_deadzoneX, m_deadzoneY;
		float m_leftStickX, m_leftStickY, m_rightStickX, m_rightStickY;
		float m_leftTrigger, m_rightTrigger;
	};
}
}

#endif