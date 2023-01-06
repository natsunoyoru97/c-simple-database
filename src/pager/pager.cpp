// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 23-1-2.
//

#include "pager.h"  // NOLINT

#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

// TODO(natsunoyoru97): duplicate declaration, this will be in an Util class
// The size of bytes a page has
constexpr uint32_t k_page_size = 256;
// The fixed-size of rows that contains in a page
constexpr uint32_t rows_per_page = 100;
// The size of bytes a row has
constexpr uint32_t row_size = 100;

namespace storage {
Pager* Pager::InitPager(const char* filename) {
  Pager* pager;
  int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

  // TODO(natsunoyoru97): propogate error status
  if (fd == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to open the file\n";
    exit(EXIT_FAILURE);
  }

  off_t file_len = lseek(fd, 0, SEEK_END);

  pager->fd_ = fd;
  pager->file_len_ = file_len;

  for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
    pages_[i] = nullptr;
  }

  uint32_t num_rows = (file_len_ % row_size == 0) ? file_len_ / row_size
                                                  : file_len_ / row_size + 1;
  pager->num_rows_ = num_rows;

  return pager;
}

Pager::~Pager() {
  uint32_t num_full_pages = (num_rows_ % rows_per_page == 0)
                                ? num_rows_ / rows_per_page
                                : num_rows_ / rows_per_page + 1;

  for (uint32_t i = 0; i < num_full_pages; ++i) {
    if (pages_[i] == nullptr) {
      continue;
    }
    Flush(i, 1);
    // TODO(natsunoyoru97): Make page an ADT
    // and consider use unique_ptr
    delete pages_[i];
  }

  int ret = close(fd_);
  if (ret == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to close db file\n";
    exit(EXIT_FAILURE);
  }
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    char* page = pages_[i];
    if (page != nullptr) {
      // TODO(natsunoyoru97): Make page an ADT
    // and consider use unique_ptr
      delete page;
    }
  }
}

const char* Pager::GetPage(uint32_t page_num) {
  if (page_num >= TABLE_MAX_PAGES) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Tried to fetch page number out of bounds.\n";
    exit(EXIT_FAILURE);
  }

  if (pages_[page_num] == nullptr) {
    // TODO(natsunoyoru97): Make page an ADT
    // and consider use unique_ptr
    char* page = new char[k_page_size];
    uint32_t num_pages = file_len_ / k_page_size;

    if (file_len_ % k_page_size != 0) {
      num_pages++;
    }

    if (page_num < num_pages) {
      ssize_t bytes_read =
          pread(fd_, page, k_page_size, page_num * k_page_size);
      if (bytes_read == -1) {
        // TODO(natsunoyoru97): Use glog to replace the cout
        std::cout << "Fail to read the file\n";
        exit(EXIT_FAILURE);
      }
    }

    pages_[page_num] = page;
  }

  return pages_[page_num];
}

void Pager::Flush(uint32_t page_start, uint32_t page_num_to_flush) {
  if (pages_[page_start] == nullptr) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Tried to flush null page\n";
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_written =
      pwrite(fd_, pages_[page_start], page_num_to_flush * k_page_size, page_start * k_page_size);

  if (bytes_written == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to write to db\n";
    exit(EXIT_FAILURE);
  }
}
}  // namespace storage
