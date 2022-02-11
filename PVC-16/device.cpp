#include "device.h"
#include "bus.h"
#include <iostream>

void DeviceController::start(void)
{
	std::thread thr([*this](void) mutable -> void
	{
		while (true)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (Device* device : devices)
				device->process();
			std::this_thread::sleep_until(start + std::chrono::milliseconds(1));
		}
	});

	thr.detach();
}

void DeviceController::addDevice(Device* device)
{
	device->dc = this;
	devices.push_back(device);
}

void DebugOutputDevice::process(void)
{
	if (auto ch = busRead(BUS_DOUT); ch)
	{
		putchar(busRead(BUS_DOUT));
		busWrite(BUS_DOUT, 0);
	}
}