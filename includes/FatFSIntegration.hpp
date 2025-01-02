#pragma once

#include "ff.h"
#include "kdbx.hpp"

namespace integration::fatfs {

enum class EError { NoError, ErrorMount, OpenDirectory, OpenFile, ReadFile };

class CFatFs final {
public:
  consteval CFatFs() = default;

  inline EError Init() const {
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

    // TODO: file search
    FIL file;
    FILINFO finfo;
    f_findfirst(&dir, &finfo, "", "*.pss");
    res = f_open(&file, finfo.fname, FA_OPEN_EXISTING | FA_READ);
    if (FR_OK != res) {
      return OpenFile;
    }

    char readBuff[128];
    unsigned int bytesRead;
    res = f_read(&file, readBuff, sizeof(readBuff) - 1, &bytesRead);
    if (FR_OK != res) {
      return ReadFile;
    }

    return NoError;
  }
};
} // namespace integration::fatfs