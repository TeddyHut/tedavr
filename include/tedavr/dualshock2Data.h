#pragma once

#include <avr/pgmspace.h>
#include <inttypes.h>
#include <string.h>

#include "gamepad.h"

namespace Dualshock2Data {
	namespace Common {
		constexpr uint8_t max_len = 21;
		namespace Button {
			enum e {
				Select = 0x00,
				L3 = 0x01,
				R3 = 0x02,
				Start = 0x03,
				Up = 0x04,
				Right = 0x05,
				Down = 0x06,
				Left = 0x07,
				L2 = 0x08,
				R2 = 0x09,
				L1 = 0x0a,
				R1 = 0x0b,
				X = 0x0c,
				A = 0x0d,
				B = 0x0e,
				Y = 0x0f,
			};
		}
	}
	namespace Master {
		namespace Header {
			//No size_t? Hm.
			constexpr uint8_t len = 3;
			uint8_t const seed[len] PROGMEM = { 0x01, 0x00, 0x00 };
			namespace Command {
				constexpr uint8_t offset = 0x01;
				enum e {
					ButtonInPoll = 0x41,
					Poll = 0x42,
					Config = 0x43,
					MotorMap = 0x4d,
					Mode = 0x44,
					PressureSetup = 0x4f,
				};
			}
		}
		namespace Packet {
			namespace ButtonInPoll {
				constexpr uint8_t len = 6;
				uint8_t const seed[len] PROGMEM = { 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a };
				namespace Button {
					constexpr uint8_t offset = 0x00;
				}
			}
			namespace Poll {
				constexpr uint8_t len = 18;
				uint8_t const seed[len] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
				namespace Rumble {
					namespace Left {
						constexpr uint8_t offset = 0x00;
					}
					namespace Right {
						constexpr uint8_t offset = 0x01;
					}
				}
			}
			namespace Config {
				constexpr uint8_t len = 18;
				uint8_t const seed[len] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
				namespace Mode {
					constexpr uint8_t offset = 0x00;
					enum e {
						Normal = 0x00,
						Config = 0x01,
					};
				}
			}
			namespace MotorMap {
				constexpr uint8_t len = 6;
				uint8_t const seed[len] PROGMEM = { 0x00, 0x00, 0xff, 0xff, 0xff, 0xff };
				enum e {
					Right = 0x00,
					Left = 0x01,
				};
				namespace X00 {
					constexpr uint8_t offset = 0x00;
				}
				namespace X01 {
					constexpr uint8_t offset = 0x01;
				}
			}
			namespace Mode {
				constexpr uint8_t len = 6;
				uint8_t const seed[len] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
				namespace Mode {
					constexpr uint8_t offset = 0x00;
					enum e {
						Digital = 0x00,
						Analog = 0x01,
					};
				}
				namespace Lock {
					constexpr uint8_t offset = 0x01;
					enum e {
						Unlocked = 0x00,
						Locked = 0x03,
					};
				}
			}
			namespace PressureSetup {
				//Cos I'm lazy just setting it up to always get all pressure values
				constexpr uint8_t len = 6;
				uint8_t const seed[len] PROGMEM = { 0xff, 0xff, 0x03, 0x00, 0x00, 0x00 };
			}
		}
	}
	namespace Slave {
		namespace Header {
			constexpr uint8_t len = 3;
			uint8_t const seed[len] PROGMEM = { 0xff, 0x00, 0x5a };
			namespace PacketWords {
				constexpr uint8_t offset = 0x01;
				constexpr uint8_t mask = 0x0f;
				constexpr uint8_t shift = 0;

			}
			namespace Mode {
				constexpr uint8_t offset = 0x01;
				constexpr uint8_t mask = 0xf0;
				constexpr uint8_t shift = 4;
				enum e {
					Digital = 0x4,
					Analog = 0x7,
					Config = 0xf,
				};
			}
			namespace Response {
				constexpr uint8_t offset = 0x01;
				enum e {
					bad_value = 0xff,
				};
			}
		}
		namespace Packet {
			constexpr uint8_t len = 6;
			uint8_t const seed[6] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x5a };
			namespace Button {
				constexpr uint8_t offset = 0x00;
			}
			namespace Poll {
				namespace Digital {
					constexpr uint8_t offset = 0x00;
					constexpr uint8_t len = 0x02;
				}
				namespace Stick {
					namespace Left {
						namespace X {
							constexpr uint8_t offset = 0x04;
						}
						namespace Y {
							constexpr uint8_t offset = 0x05;
						}
					}
					namespace Right {
						namespace X {
							constexpr uint8_t offset = 0x02;
						}
						namespace Y {
							constexpr uint8_t offset = 0x03;
						}
					}
				}
				namespace Pressure {
					namespace Right {
						constexpr uint8_t offset = 0x06;
					}
					namespace Left {
						constexpr uint8_t offset = 0x07;
					}
					namespace Up {
						constexpr uint8_t offset = 0x08;
					}
					namespace Down {
						constexpr uint8_t offset = 0x09;
					}
					namespace X {
						constexpr uint8_t offset = 0x0a;
					}
					namespace A {
						constexpr uint8_t offset = 0x0b;
					}
					namespace B {
						constexpr uint8_t offset = 0x0c;
					}
					namespace Y {
						constexpr uint8_t offset = 0x0d;
					}
					namespace L1 {
						constexpr uint8_t offset = 0x0e;
					}
					namespace R1 {
						constexpr uint8_t offset = 0x0f;
					}
					namespace L2 {
						constexpr uint8_t offset = 0x10;
					}
					namespace R2 {
						constexpr uint8_t offset = 0x11;
					}
				}
			}
		}
	}
	uint8_t const buttonMap[16] PROGMEM = {
		(Gamepad::A << 4) | Dualshock2Data::Common::Button::A,
		(Gamepad::B << 4) | Dualshock2Data::Common::Button::B,
		(Gamepad::X << 4) | Dualshock2Data::Common::Button::X,
		(Gamepad::Y << 4) | Dualshock2Data::Common::Button::Y,
		(Gamepad::L1 << 4) | Dualshock2Data::Common::Button::L1,
		(Gamepad::R1 << 4) | Dualshock2Data::Common::Button::R1,
		(Gamepad::L2 << 4) | Dualshock2Data::Common::Button::L2,
		(Gamepad::R2 << 4) | Dualshock2Data::Common::Button::R2,
		(Gamepad::L3 << 4) | Dualshock2Data::Common::Button::L3,
		(Gamepad::R3 << 4) | Dualshock2Data::Common::Button::R3,
		(Gamepad::Up << 4) | Dualshock2Data::Common::Button::Up,
		(Gamepad::Down << 4) | Dualshock2Data::Common::Button::Down,
		(Gamepad::Left << 4) | Dualshock2Data::Common::Button::Left,
		(Gamepad::Right << 4) | Dualshock2Data::Common::Button::Right,
		(Gamepad::Start << 4) | Dualshock2Data::Common::Button::Start,
		(Gamepad::Select << 4) | Dualshock2Data::Common::Button::Select };
}
