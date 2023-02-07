// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 23-1-2.
//

#ifndef SRC_PAGER_PAGER_H_
#define SRC_PAGER_PAGER_H_

#include <array>
#include <cstdint>
#include <mutex>

#include "../config/config.h"
#include "../util/util.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"

namespace storage {

// An additional layer for the Pager object to handle the file descriptor
class FileHandler {
 private:
  int fd_;
  int32_t file_len_;
  explicit FileHandler(const char* filename, int fd);

 public:
  static absl::StatusOr<FileHandler*> InitFileHandler(const char* filename);
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
  std::atomic<uint32_t> num_rows_;
  // TODO(natsunoyoru97): how about using other data structures?
  // TODO(natsunoyoru97): also considering about using disk space
  std::array<char*, TABLE_MAX_PAGES> pages_;
  explicit Pager(const char* filename, FileHandler* file_handler);

 public:
  static absl::StatusOr<Pager*> InitPager(const char* filename);
  ~Pager();
  FileHandler* GetFileHandler();

  // Get a page from the data cache
  absl::StatusOr<const char*> GetPage(uint32_t page_num);
  // Write data to the data cache
  absl::Status Flush(uint32_t page_start);
  // Increase the num_rows_ by one.
  // Note it is assumed to provide guarantee for CONCURRENCY
  void IncRowCntByOne();
  // Get num_rows_
  uint32_t GetNumRows();
};

}  // namespace storage

#endif  // SRC_PAGER_PAGER_H_
