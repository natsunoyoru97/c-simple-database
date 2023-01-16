// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 23-1-2.
//

#ifndef SRC_PAGER_PAGER_H_
#define SRC_PAGER_PAGER_H_

#include <array>
#include <cstdint>

#include "../config/config.h"
#include "../util/util.h"

namespace storage {

// An additional layer for the Pager object to handle the file descriptor
class FileHandler {
  private:
    int fd_;
    uint32_t file_len_;
    FileHandler(const char* filename);
  public:
    static FileHandler* InitFileHandler(const char* filename);
    ~FileHandler();
    uint32_t GetFileLen();
    int GetFd();
};

// The indirection layer to handle file descriptor and give a block of memory
class Pager {
 private:
  FileHandler* file_handler_;
  uint32_t file_len_;
  // Rows this block of memory has
  uint32_t num_rows_;
  // TODO(natsunoyoru97): how about using other data structures?
  // TODO(natsunoyoru97): also considering about using disk space
  std::array<char*, TABLE_MAX_PAGES> pages_;
  explicit Pager(const char* filename);

 public:
  static Pager* InitPager(const char* filename);
  ~Pager();

  // Get a page from the data cache
  const char* GetPage(uint32_t page_num);
  // Write data to the data cache
  // TODO(natsunoyoru97): it will return a Status object
  void Flush(uint32_t page_start);

  void FreePage();
};

}  // namespace storage

#endif  // SRC_PAGER_PAGER_H_
