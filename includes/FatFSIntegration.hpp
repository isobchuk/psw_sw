#pragma once

#include "ff.h"
#include "kdbx.hpp"
#include "meta_types.hpp"

namespace integration::fatfs {

enum class EError { NoError, ErrorMount, OpenDirectory, OpenFile, ReadFile };

class CFatFs final {
  FIL file;
public:
  inline CFatFs() = default;

  inline EError Init() {
    using enum EError;
    FATFS fs;
    FRESULT res;

    res = f_mount(&fs, "", 0);
    if (FR_OK != res) {
      return ErrorMount;
    }

    DIR dir;
    res = f_opendir(&dir, "/");
    if (FR_OK != res) {
      return OpenDirectory;
    }
  
    FILINFO finfo;
    f_findfirst(&dir, &finfo, "", "*.pss");
    res = f_open(&file, finfo.fname, FA_OPEN_EXISTING | FA_READ);
    if (FR_OK != res) {
      return OpenFile;
    }

    return NoError;
  }

  template<const std::size_t N, iso::meta_type::const_value_of_type<std::size_t> Length = iso::meta_type::const_t<N>>
  requires (N >= Length::value)
  inline std::size_t Read(char (&buff)[N], const Length = iso::meta_type::const_v<N>) {
    std::size_t bytesRead = 0;
    FRESULT res = FR_OK;

    res = f_read(&file, buff, Length::value - 1, &bytesRead);

    return (FR_OK == res) ? bytesRead : std::size_t(-1);
  }

};
} // namespace integration::fatfs