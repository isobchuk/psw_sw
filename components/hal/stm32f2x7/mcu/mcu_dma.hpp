#pragma once

#include "meta_types.hpp"
#include "reg_dma.hpp"
#include "reg_rcc.hpp"

namespace stm32f217::drivers::dma {

enum class EPort : cpp_register::RegisterAddress { Dma1 = 0x4002'6000UL, Dma2 = 0x4002'6400UL };
enum class EStream : unsigned { Stream0, Stream1, Stream2, Stream3, Stream4, Stream5, Stream6, Stream7 };
using TControlRegisterSize = uint32_t;
enum class EChannel : TControlRegisterSize {
  Channel0,
  Channel1,
  Channel2,
  Channel3,
  Channel4,
  Channel5,
  Channel6,
  Channel7,
};
enum class EMemoryBurst : TControlRegisterSize { SingleTransfer, Incr4, Incr8, Incr16 };
enum class EPeripheralBurst : TControlRegisterSize { SingleTransfer, Incr4, Incr8, Incr16 };
enum class ECurrentTarget : TControlRegisterSize { Memory0, Memory1 };
enum class EDoubleBuffer : TControlRegisterSize { Disabled, Enabled };
enum class EPriority : TControlRegisterSize { Low, Medium, High, VeryHigh };
enum class EPeripheralIncrementOffset : TControlRegisterSize { Disabled, Enabled };
enum class EMemoryDataSize : TControlRegisterSize { Byte, HalfWord, Word };
enum class EPeripheralDataSize : TControlRegisterSize { Byte, HalfWord, Word };
enum class EMemoryIncrementMode : TControlRegisterSize { Disabled, Enabled };
enum class EPeripheralIncrementMode : TControlRegisterSize { Disabled, Enabled };
enum class ECircularMode : TControlRegisterSize { Disabled, Enabled };
enum class EDataTransferDirection : TControlRegisterSize { Peripheral2Memory, Memory2Peripheral, Memory2Memory };
enum class EPeripheralFlowController : TControlRegisterSize { DMA, Peripheral };
enum class EInterruptTransferComplete : TControlRegisterSize { Disabled, Enabled };
enum class EInterruptHalfTransfer : TControlRegisterSize { Disabled, Enabled };
enum class EInterruptTransferError : TControlRegisterSize { Disabled, Enabled };
enum class EInterruptDirectModeError : TControlRegisterSize { Disabled, Enabled };

template <typename Function> struct SDmaChannelConfiguration final {
  const Function _Function;
  const EPort _Port;
  const EStream _Stream;
  const EChannel _Channel;
  const EMemoryBurst _MemoryBurst;
  const EPeripheralBurst _PeripheralBurst;
  const ECurrentTarget _CurrentTarget;
  const EDoubleBuffer _DoubleBuffer;
  const EPriority _Priority;
  const EPeripheralIncrementOffset _PeripheralIncrementOffset;
  const EMemoryDataSize _MemoryDataSize;
  const EPeripheralDataSize _PeripheralDataSize;
  const EMemoryIncrementMode _MemoryIncrementMode;
  const EPeripheralIncrementMode _PeripheralIncrementMode;
  const ECircularMode _CircularMode;
  const EDataTransferDirection _DataTransferDirection;
  const EPeripheralFlowController _PeripheralFlowController;
  const EInterruptTransferComplete _InterruptTransferComplete;
  const EInterruptHalfTransfer _InterruptHalfTransfer;
  const EInterruptTransferError _InterruptTransferError;
  const EInterruptDirectModeError _InterruptDirectModeError;

  template <typename... Args>
  requires iso::meta_type::var_pack::is_types_unique_v<Args...> &&
               iso::meta_type::var_pack::is_type_list<EMemoryBurst, EPeripheralBurst, ECurrentTarget, EDoubleBuffer, EPriority,
                                                      EPeripheralIncrementOffset, EMemoryDataSize, EPeripheralDataSize, EMemoryIncrementMode,
                                                      EPeripheralIncrementMode, ECircularMode, EDataTransferDirection, EPeripheralFlowController,
                                                      EInterruptTransferComplete, EInterruptHalfTransfer, EInterruptTransferError,
                                                      EInterruptDirectModeError>::contains_v<Args...>
  consteval SDmaChannelConfiguration(const Function func, const EPort port, const EStream stream, const EChannel ch, const Args... args)
      : _Function(func), _Port(port), _Stream(stream), _Channel(ch), _MemoryBurst(iso::meta_type::var_pack::type<EMemoryBurst>::get(args...)),
        _PeripheralBurst(iso::meta_type::var_pack::type<EPeripheralBurst>::get(args...)),
        _CurrentTarget(iso::meta_type::var_pack::type<ECurrentTarget>::get(args...)),
        _DoubleBuffer(iso::meta_type::var_pack::type<EDoubleBuffer>::get(args...)),
        _Priority(iso::meta_type::var_pack::type<EPriority>::get(args...)),
        _PeripheralIncrementOffset(iso::meta_type::var_pack::type<EPeripheralIncrementOffset>::get(args...)),
        _MemoryDataSize(iso::meta_type::var_pack::type<EMemoryDataSize>::get(args...)),
        _PeripheralDataSize(iso::meta_type::var_pack::type<EPeripheralDataSize>::get(args...)),
        _MemoryIncrementMode(iso::meta_type::var_pack::type<EMemoryIncrementMode>::get(args...)),
        _PeripheralIncrementMode(iso::meta_type::var_pack::type<EPeripheralIncrementMode>::get(args...)),
        _CircularMode(iso::meta_type::var_pack::type<ECircularMode>::get(args...)),
        _DataTransferDirection(iso::meta_type::var_pack::type<EDataTransferDirection>::get(args...)),
        _PeripheralFlowController(iso::meta_type::var_pack::type<EPeripheralFlowController>::get(args...)),
        _InterruptTransferComplete(iso::meta_type::var_pack::type<EInterruptTransferComplete>::get(args...)),
        _InterruptHalfTransfer(iso::meta_type::var_pack::type<EInterruptHalfTransfer>::get(args...)),
        _InterruptTransferError(iso::meta_type::var_pack::type<EInterruptTransferError>::get(args...)),
        _InterruptDirectModeError(iso::meta_type::var_pack::type<EInterruptDirectModeError>::get(args...)) {}
};

template <typename T>
concept dma_config = requires(const T &str) {
  { decltype(str._Port){} } -> std::same_as<EPort>;
  { decltype(str._Stream){} } -> std::same_as<EStream>;
  { decltype(str._Channel){} } -> std::same_as<EChannel>;
  { decltype(str._MemoryBurst){} } -> std::same_as<EMemoryBurst>;
  { decltype(str._PeripheralBurst){} } -> std::same_as<EPeripheralBurst>;
  { decltype(str._CurrentTarget){} } -> std::same_as<ECurrentTarget>;
  { decltype(str._DoubleBuffer){} } -> std::same_as<EDoubleBuffer>;
  { decltype(str._Priority){} } -> std::same_as<EPriority>;
  { decltype(str._PeripheralIncrementOffset){} } -> std::same_as<EPeripheralIncrementOffset>;
  { decltype(str._MemoryDataSize){} } -> std::same_as<EMemoryDataSize>;
  { decltype(str._PeripheralDataSize){} } -> std::same_as<EPeripheralDataSize>;
  { decltype(str._MemoryIncrementMode){} } -> std::same_as<EMemoryIncrementMode>;
  { decltype(str._PeripheralIncrementMode){} } -> std::same_as<EPeripheralIncrementMode>;
  { decltype(str._CircularMode){} } -> std::same_as<ECircularMode>;
  { decltype(str._DataTransferDirection){} } -> std::same_as<EDataTransferDirection>;
  { decltype(str._PeripheralFlowController){} } -> std::same_as<EPeripheralFlowController>;
  { decltype(str._InterruptTransferComplete){} } -> std::same_as<EInterruptTransferComplete>;
  { decltype(str._InterruptHalfTransfer){} } -> std::same_as<EInterruptHalfTransfer>;
  { decltype(str._InterruptTransferError){} } -> std::same_as<EInterruptTransferError>;
  { decltype(str._InterruptDirectModeError){} } -> std::same_as<EInterruptDirectModeError>;
};

enum class EAddressRegion { Memory0, Memory1, Peripheral };
enum class EInterruptReason { ErrorFifo, ErrorDirectMode, ErrorTransfer, HalfTransfer, TransferComplete };

template <typename T>
concept dma_stream = requires(const T &str) {
  { str.Init() } -> std::same_as<void>;
  { str.Address(iso::meta_type::const_v<EAddressRegion{}>, iso::meta_type::const_v<std::size_t{}>) } -> std::same_as<void>;
  { str.Address(iso::meta_type::const_v<EAddressRegion{}>, iso::meta_type::const_v<(void *){}>) } -> std::same_as<void>;
  { str.Address(iso::meta_type::const_v<EAddressRegion{}>, std::size_t{}) } -> std::same_as<void>;
  { str.Address(iso::meta_type::const_v<EAddressRegion{}>, (void *){}) } -> std::same_as<void>;
  { str.Number(iso::meta_type::const_v<uint32_t{}>) } -> std::same_as<void>;
  { str.Number(uint32_t{}) } -> std::same_as<void>;
  { str.Enable() } -> std::same_as<void>;
  { str.Disable() } -> std::same_as<void>;
  { str.InterruptStatus(iso::meta_type::const_v<EInterruptReason{}>) } -> std::same_as<bool>;
  { str.InterruptClear(iso::meta_type::const_v<EInterruptReason{}>) } -> std::same_as<void>;
};

template <dma_config auto &config> class CDmaStream final {
  static constexpr registers::dma::DMA_T<static_cast<cpp_register::RegisterAddress>(config._Port)> *DMA{};
  static constexpr auto STREAM = cpp_register::reg_v<static_cast<unsigned>(config._Stream)>;
  static constexpr auto _Config = config;

public:
  consteval CDmaStream()
  requires dma_stream<CDmaStream>
  {}

  inline void Init() const {
    using namespace cpp_register;
    using namespace stm32f217::registers::dma;

    DMA->SCR[STREAM] |= DMA_SCR::CHSEL(reg_v<_Config._Channel>) | DMA_SCR::MBURST(reg_v<_Config._MemoryBurst>) |
                        DMA_SCR::PBURST(reg_v<_Config._PeripheralBurst>) | DMA_SCR::CT(reg_v<_Config._CurrentTarget>) |
                        DMA_SCR::DBM(reg_v<_Config._DoubleBuffer>) | DMA_SCR::PL(reg_v<_Config._Priority>) |
                        DMA_SCR::PINCOS(reg_v<_Config._PeripheralIncrementOffset>) | DMA_SCR::MSIZE(reg_v<_Config._MemoryDataSize>) |
                        DMA_SCR::PSIZE(reg_v<_Config._PeripheralDataSize>) | DMA_SCR::MINC(reg_v<_Config._MemoryIncrementMode>) |
                        DMA_SCR::PINC(reg_v<_Config._PeripheralIncrementMode>) | DMA_SCR::CIRC(reg_v<_Config._CircularMode>) |
                        DMA_SCR::DIR(reg_v<_Config._DataTransferDirection>) | DMA_SCR::PFCTRL(reg_v<_Config._PeripheralFlowController>) |
                        DMA_SCR::TCIE(reg_v<_Config._InterruptTransferComplete>) | DMA_SCR::HTIE(reg_v<_Config._InterruptHalfTransfer>) |
                        DMA_SCR::TEIE(reg_v<_Config._InterruptTransferError>) | DMA_SCR::DMEIE(reg_v<_Config._InterruptDirectModeError>);
  }

  template <iso::meta_type::const_value_of_type<EAddressRegion> AddressRegion, iso::meta_type::const_value_of_type<std::size_t> AddressValue>
  inline void Address(const AddressRegion, const AddressValue) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::dma;
    if constexpr (EAddressRegion::Memory0 == AddressRegion::value) {
      DMA->SM0AR[STREAM] = DMA_SM0AR::M0A(reg_v<AddressValue::value>);
    } else if (EAddressRegion::Memory1 == AddressRegion::value) {
      DMA->SM1AR[STREAM] = DMA_SM1AR::M1A(reg_v<AddressValue::value>);
    } else if (EAddressRegion::Peripheral == AddressRegion::value) {
      DMA->SPAR[STREAM] = DMA_SPAR::PA(reg_v<AddressValue::value>);
    } else {
      static_assert(((EAddressRegion::Memory0 == AddressRegion::value) || (EAddressRegion::Memory1 == AddressRegion::value) ||
                     (EAddressRegion::Peripheral == AddressRegion::value)),
                    "Driver DMA: Not allowable address region!");
    }
  }

  template <iso::meta_type::const_value_of_type<EAddressRegion> AddressRegion, iso::meta_type::const_value_of_type<void *> AddressValue>
  inline void Address(const AddressRegion, const AddressValue) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::dma;
    if constexpr (EAddressRegion::Memory0 == AddressRegion::value) {
      DMA->SM0AR[STREAM] = DMA_SM0AR::M0A(reg_v<AddressValue::value>);
    } else if (EAddressRegion::Memory1 == AddressRegion::value) {
      DMA->SM1AR[STREAM] = DMA_SM1AR::M1A(reg_v<AddressValue::value>);
    } else if (EAddressRegion::Peripheral == AddressRegion::value) {
      DMA->SPAR[STREAM] = DMA_SPAR::PA(reg_v<AddressValue::value>);
    } else {
      static_assert(((EAddressRegion::Memory0 == AddressRegion::value) || (EAddressRegion::Memory1 == AddressRegion::value) ||
                     (EAddressRegion::Peripheral == AddressRegion::value)),
                    "Driver DMA: Not allowable address region!");
    }
  }

  template <iso::meta_type::const_value_of_type<EAddressRegion> AddressRegion>
  inline void Address(const AddressRegion, const std::size_t addressValue) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::dma;
    if constexpr (EAddressRegion::Memory0 == AddressRegion::value) {
      DMA->SM0AR[STREAM] = addressValue;
    } else if (EAddressRegion::Memory1 == AddressRegion::value) {
      DMA->SM1AR[STREAM] = addressValue;
    } else if (EAddressRegion::Peripheral == AddressRegion::value) {
      DMA->SPAR[STREAM] = addressValue;
    } else {
      static_assert(((EAddressRegion::Memory0 == AddressRegion::value) || (EAddressRegion::Memory1 == AddressRegion::value) ||
                     (EAddressRegion::Peripheral == AddressRegion::value)),
                    "Driver DMA: Not allowable address region!");
    }
  }

  template <iso::meta_type::const_value_of_type<EAddressRegion> AddressRegion>
  inline void Address(const AddressRegion, const void *addressValue) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::dma;
    if constexpr (EAddressRegion::Memory0 == AddressRegion::value) {
      DMA->SM0AR[STREAM] = addressValue;
    } else if (EAddressRegion::Memory1 == AddressRegion::value) {
      DMA->SM1AR[STREAM] = addressValue;
    } else if (EAddressRegion::Peripheral == AddressRegion::value) {
      DMA->SPAR[STREAM] = addressValue;
    } else {
      static_assert(((EAddressRegion::Memory0 == AddressRegion::value) || (EAddressRegion::Memory1 == AddressRegion::value) ||
                     (EAddressRegion::Peripheral == AddressRegion::value)),
                    "Driver DMA: Not allowable address region!");
    }
  }

  template <iso::meta_type::const_value_of_type<uint32_t> NumberValue> inline void Number(const NumberValue) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::dma;
    DMA->SNDTR[STREAM] = DMA_SNDTR::NDT(reg_v<NumberValue::value>);
  }

  inline void Number(const uint32_t numberValue) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::dma;
    DMA->SNDTR[STREAM] = numberValue;
  }

  inline void Enable() const {
    using namespace cpp_register;
    using namespace stm32f217::registers::dma;
    DMA->SCR[STREAM] |= DMA_SCR::EN;
  }

  inline void Disable() const {
    using namespace cpp_register;
    using namespace stm32f217::registers::dma;
    DMA->SCR[STREAM] &= DMA_SCR::EN;
  }

  template <iso::meta_type::const_value_of_type<EInterruptReason> Reason> [[nodiscard]] inline bool InterruptStatus(const Reason) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::dma;
    using enum EInterruptReason;

    if constexpr (EStream::Stream0 == _Config._Stream) {
      constexpr auto check =
          DMA_LISR::FEIF0(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) | DMA_LISR::DMEIF0(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
          DMA_LISR::TEIF0(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) | DMA_LISR::HTIF0(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
          DMA_LISR::TCIF0(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      return (DMA->LISR & check);
    } else if (EStream::Stream1 == _Config._Stream) {
      constexpr auto check =
          DMA_LISR::FEIF1(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) | DMA_LISR::DMEIF1(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
          DMA_LISR::TEIF1(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) | DMA_LISR::HTIF1(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
          DMA_LISR::TCIF1(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      return (DMA->LISR & check);
    } else if (EStream::Stream2 == _Config._Stream) {
      constexpr auto check =
          DMA_LISR::FEIF2(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) | DMA_LISR::DMEIF2(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
          DMA_LISR::TEIF2(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) | DMA_LISR::HTIF2(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
          DMA_LISR::TCIF2(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      return (DMA->LISR & check);
    } else if (EStream::Stream3 == _Config._Stream) {
      constexpr auto check =
          DMA_LISR::FEIF3(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) | DMA_LISR::DMEIF3(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
          DMA_LISR::TEIF3(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) | DMA_LISR::HTIF3(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
          DMA_LISR::TCIF3(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      return (DMA->LISR & check);
    } else if (EStream::Stream4 == _Config._Stream) {
      constexpr auto check =
          DMA_HISR::FEIF4(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) | DMA_HISR::DMEIF4(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
          DMA_HISR::TEIF4(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) | DMA_HISR::HTIF4(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
          DMA_HISR::TCIF4(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      return (DMA->HISR & check);
    } else if (EStream::Stream5 == _Config._Stream) {
      constexpr auto check =
          DMA_HISR::FEIF5(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) | DMA_HISR::DMEIF5(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
          DMA_HISR::TEIF5(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) | DMA_HISR::HTIF5(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
          DMA_HISR::TCIF5(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      return (DMA->HISR & check);
    } else if (EStream::Stream6 == _Config._Stream) {
      constexpr auto check =
          DMA_HISR::FEIF6(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) | DMA_HISR::DMEIF6(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
          DMA_HISR::TEIF6(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) | DMA_HISR::HTIF6(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
          DMA_HISR::TCIF6(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      return (DMA->HISR & check);
    } else if (EStream::Stream7 == _Config._Stream) {
      constexpr auto check =
          DMA_HISR::FEIF7(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) | DMA_HISR::DMEIF7(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
          DMA_HISR::TEIF7(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) | DMA_HISR::HTIF7(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
          DMA_HISR::TCIF7(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      return (DMA->HISR & check);
    }

    return false;
  }

  template <iso::meta_type::const_value_of_type<EInterruptReason> Reason> inline void InterruptClear(const Reason) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::dma;
    using enum EInterruptReason;

    if constexpr (EStream::Stream0 == _Config._Stream) {
      constexpr auto check = DMA_LIFCR::CFEIF0(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CDMEIF0(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CTEIF0(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CHTIF0(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CTCIF0(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      DMA->LIFCR |= check;
    } else if (EStream::Stream1 == _Config._Stream) {
      constexpr auto check = DMA_LIFCR::CFEIF1(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CDMEIF1(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CTEIF1(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CHTIF1(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CTCIF1(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      DMA->LIFCR |= check;
    } else if (EStream::Stream2 == _Config._Stream) {
      constexpr auto check = DMA_LIFCR::CFEIF2(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CDMEIF2(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CTEIF2(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CHTIF2(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CTCIF2(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      DMA->LIFCR |= check;
    } else if (EStream::Stream3 == _Config._Stream) {
      constexpr auto check = DMA_LIFCR::CFEIF3(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CDMEIF3(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CTEIF3(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CHTIF3(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_LIFCR::CTCIF3(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      DMA->LIFCR |= check;
    } else if (EStream::Stream4 == _Config._Stream) {
      constexpr auto check = DMA_HIFCR::CFEIF4(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CDMEIF4(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CTEIF4(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CHTIF4(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CTCIF4(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      DMA->HIFCR |= check;
    } else if (EStream::Stream5 == _Config._Stream) {
      constexpr auto check = DMA_HIFCR::CFEIF5(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CDMEIF5(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CTEIF5(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CHTIF5(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CTCIF5(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      DMA->HIFCR |= check;
    } else if (EStream::Stream6 == _Config._Stream) {
      constexpr auto check = DMA_HIFCR::CFEIF6(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CDMEIF6(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CTEIF6(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CHTIF6(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CTCIF6(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      DMA->HIFCR |= check;
    } else if (EStream::Stream7 == _Config._Stream) {
      constexpr auto check = DMA_HIFCR::CFEIF7(reg_v<(ErrorFifo == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CDMEIF7(reg_v<(ErrorDirectMode == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CTEIF7(reg_v<(ErrorTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CHTIF7(reg_v<(HalfTransfer == Reason::value ? 1UL : 0UL)>) |
                             DMA_HIFCR::CTCIF7(reg_v<(TransferComplete == Reason::value ? 1UL : 0UL)>);
      DMA->HIFCR |= check;
    }
  }
};

template <iso::meta_type::array auto &table>
requires std::same_as<decltype(table[0]), const SDmaChannelConfiguration<std::remove_const_t<decltype(table[0]._Function)>> &>
class CDma final {
  static constexpr auto &_DmaMap = table;
  static constexpr auto _DmaMapSize = sizeof(table) / sizeof(table[0]);
  using DmaFunction = std::remove_const_t<decltype(_DmaMap[0]._Function)>;

public:
  consteval CDma() {
    constexpr auto uniqueCheck = []() -> bool {
      unsigned i, j;
      for (i = 0; i < _DmaMapSize; i++) {
        const EPort portI = _DmaMap[i]._Port;
        const uint16_t valI = static_cast<uint16_t>(_DmaMap[i]._Stream) << 8 | static_cast<uint16_t>(_DmaMap[i]._Channel);
        for (j = i + 1; j < _DmaMapSize; j++) {
          const EPort portJ = _DmaMap[j]._Port;
          const uint16_t valJ = static_cast<uint16_t>(_DmaMap[j]._Stream) << 8 | static_cast<uint16_t>(_DmaMap[j]._Channel);
          if ((portI == portJ) && (valI == valJ)) {
            return false;
          }
        }
      }
      return true;
    }();

    static_assert(uniqueCheck, "Driver DMA: No unique Port/Stream/Channel configuration!");
  }

  inline void Init() const {
    using namespace cpp_register;
    using namespace stm32f217::registers::rcc;

    constexpr auto _IsPortInside = [](const EPort port) -> uint32_t {
      uint32_t isIn = 0UL;
      for (const auto &el : _DmaMap) {
        if (port == el._Port) {
          isIn = 1UL;
          break;
        }
      }
      return isIn;
    };

    RCC->AHB1ENR |= RCC_AHB1ENR::DMA2EN(reg_v<_IsPortInside(EPort::Dma2)>) | RCC_AHB1ENR::DMA1EN(reg_v<_IsPortInside(EPort::Dma1)>);
  }

  template <iso::meta_type::const_value_of_type<DmaFunction> Function> [[nodiscard]] inline consteval auto operator[](const Function) const {
    constexpr auto index = []() {
      unsigned i = 0;
      while ((Function::value != _DmaMap[i]._Function) && (_DmaMapSize > i)) {
        i++;
      }
      return i;
    }();
    static_assert((_DmaMapSize >= index), "Driver DMA: No such Dma function inside!");
    return CDmaStream<_DmaMap[index]>{};
  }
};

} // namespace stm32f217::drivers::dma