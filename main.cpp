#include <klib/klib.hpp>
#include <klib/delay.hpp>
#include <klib/stream.hpp>
#include <klib/usb/device/mouse.hpp>

#include <io/can.hpp>
#include <io/usb.hpp>
#include <io/system.hpp>
#include <io/power.hpp>
#include <io/pins.hpp>

namespace target = klib::target;

int main() {
    // enable tracing on the hardware
    target::io::system::trace::enable<true>();
    
    klib::cout << "Booting" << klib::endl;

    // configure the usb pll
    target::io::system::clock::set_usb<12'000'000>();

    // get the usb hardware
    using usb = target::io::usb<target::io::periph::lqfp_80::usb0, klib::usb::device::mouse_hid<1>>;

    // init the usb hardware
    usb::init();

    // wait until we are configured. (this happens after the host has connected)
    while (!usb::device::is_configured<usb>()) {
        klib::delay(klib::time::ms(10));
        klib::cout << ".";
    }
    klib::cout << klib::endl;
    klib::cout << "Connected to host" << klib::endl;

    // Keep moving the mouse every 10 seconds
    while (true) {
        klib::cout << "Sending move command" << klib::endl;
        usb::device::write<usb>(0x00, -1, 0);
        usb::device::write<usb>(0x00, 1, 0);

        klib::delay(klib::time::s{10});
    }
}
