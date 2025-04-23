#pragma once

#include "register.hpp"

namespace stm32f217::registers::cryp {

template <const cpp_register::RegisterAddress address> struct CRYP_T {
	static constexpr cpp_register::Register<address + 0x0, cpp_register::AccessMode::RW, uint32_t, struct CR> CR{};
	static constexpr cpp_register::Register<address + 0x4, cpp_register::AccessMode::R, uint32_t, struct SR> SR{};
	static constexpr cpp_register::Register<address + 0x8, cpp_register::AccessMode::RW, uint32_t, struct DIN> DIN{};
	static constexpr cpp_register::Register<address + 0xC, cpp_register::AccessMode::R, uint32_t, struct DOUT> DOUT{};
	static constexpr cpp_register::Register<address + 0x10, cpp_register::AccessMode::RW, uint32_t, struct DMACR> DMACR{};
	static constexpr cpp_register::Register<address + 0x14, cpp_register::AccessMode::RW, uint32_t, struct IMSCR> IMSCR{};
	static constexpr cpp_register::Register<address + 0x18, cpp_register::AccessMode::R, uint32_t, struct RISR> RISR{};
	static constexpr cpp_register::Register<address + 0x1C, cpp_register::AccessMode::R, uint32_t, struct MISR> MISR{};
	static constexpr cpp_register::Register<address + 0x20, cpp_register::AccessMode::W, uint32_t, struct KR, 8U> KR{};
	static constexpr cpp_register::Register<address + 0x40, cpp_register::AccessMode::RW, uint32_t, struct IVR, 4U> IVR{};

};

inline constexpr CRYP_T<0x50060000> const *CRYP{};


 struct CRYP_CR {
	static constexpr cpp_register::Field<decltype(CRYP->CR), (1UL << 15), cpp_register::AccessMode::RW, 1> CRYPEN{};
	static constexpr cpp_register::Field<decltype(CRYP->CR), (1UL << 14), cpp_register::AccessMode::RW, 1> FFLUSH{};
	static constexpr cpp_register::Field<decltype(CRYP->CR), (1UL << 8), cpp_register::AccessMode::RW, 2> KEYSIZE{};
	static constexpr cpp_register::Field<decltype(CRYP->CR), (1UL << 6), cpp_register::AccessMode::RW, 2> DATATYPE{};
	static constexpr cpp_register::Field<decltype(CRYP->CR), (1UL << 3), cpp_register::AccessMode::RW, 3> ALGOMODE{};
	static constexpr cpp_register::Field<decltype(CRYP->CR), (1UL << 2), cpp_register::AccessMode::RW, 1> ALGODIR{};
};

struct CRYP_SR {
	static constexpr cpp_register::Field<decltype(CRYP->SR), (1UL << 4), cpp_register::AccessMode::R, 1> BUSY{};
	static constexpr cpp_register::Field<decltype(CRYP->SR), (1UL << 3), cpp_register::AccessMode::R, 1> OFFU{};
	static constexpr cpp_register::Field<decltype(CRYP->SR), (1UL << 2), cpp_register::AccessMode::R, 1> OFNE{};
	static constexpr cpp_register::Field<decltype(CRYP->SR), (1UL << 1), cpp_register::AccessMode::R, 1> IFNF{};
	static constexpr cpp_register::Field<decltype(CRYP->SR), (1UL << 0), cpp_register::AccessMode::R, 1> IFEM{};
};

struct CRYP_DIN {
	static constexpr cpp_register::Field<decltype(CRYP->DIN), (1UL << 0), cpp_register::AccessMode::RW, 32> DATAIN{};
};

struct CRYP_DOUT {
	static constexpr cpp_register::Field<decltype(CRYP->DOUT), (1UL << 0), cpp_register::AccessMode::R, 32> DATAOUT{};
};

struct CRYP_DMACR {
	static constexpr cpp_register::Field<decltype(CRYP->DMACR), (1UL << 1), cpp_register::AccessMode::RW, 1> DOEN{};
	static constexpr cpp_register::Field<decltype(CRYP->DMACR), (1UL << 0), cpp_register::AccessMode::RW, 1> DIEN{};
};

struct CRYP_IMSCR {
	static constexpr cpp_register::Field<decltype(CRYP->IMSCR), (1UL << 1), cpp_register::AccessMode::RW, 1> OUTIM{};
	static constexpr cpp_register::Field<decltype(CRYP->IMSCR), (1UL << 0), cpp_register::AccessMode::RW, 1> INIM{};
};

struct CRYP_RISR {
	static constexpr cpp_register::Field<decltype(CRYP->RISR), (1UL << 1), cpp_register::AccessMode::R, 1> OUTRIS{};
	static constexpr cpp_register::Field<decltype(CRYP->RISR), (1UL << 0), cpp_register::AccessMode::R, 1> INRIS{};
};

struct CRYP_MISR {
	static constexpr cpp_register::Field<decltype(CRYP->MISR), (1UL << 1), cpp_register::AccessMode::R, 1> OUTMIS{};
	static constexpr cpp_register::Field<decltype(CRYP->MISR), (1UL << 0), cpp_register::AccessMode::R, 1> INMIS{};
};

struct CRYP_K0LR {
	static constexpr cpp_register::Field<decltype(CRYP->KR), (1UL << 0), cpp_register::AccessMode::W, 32> b{};
};


struct CRYP_IV0LR {
	static constexpr cpp_register::Field<decltype(CRYP->IVR), (1UL << 0), cpp_register::AccessMode::RW, 32> IV{};
};

}
