// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 23-1-2.
//

#include "pager.h"  // NOLINT

#include <fcntl.h>


#include <cstdlib>
#include <iostream>

#include "../util/util.h"

namespace storage {
  /*
  Pager::Pager(const char* filename) {
    int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

    off_t file_len = lseek(fd, 0, SEEK_END);
    std::cout << file_len << "\n";

    fd_ = fd;
    file_len_ = file_len;

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
      pages_[i] = nullptr;
    }

    uint32_t num_rows = (file_len_ % rowSize == 0) ? file_len_ / rowSize
                                                    : file_len_ / rowSize + 1;
    num_rows_ = num_rows;
  }
*/
Pager* Pager::InitPager(const char* filename) {
  Pager* pager;
  int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

  // TODO(natsunoyoru97): propogate error status
  if (fd == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to open the file\n";
    // TODO(natsunoyoru97): Use Status/StatusOr instead
    exit(EXIT_FAILURE);
  }

  // TODO(natsunoyoru97): consider about the case that the file is created and it is 0
  off_t file_len = lseek(fd, 0, SEEK_END);

  pager->fd_ = fd;
  pager->file_len_ = file_len;

  for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
    pager->pages_[i] = nullptr;
  }

  uint32_t num_rows = (pager->file_len_ % rowSize == 0) ? pager->file_len_ / rowSize
                                                  : pager->file_len_ / rowSize + 1;
  pager->num_rows_ = num_rows;

  return pager;
}

Pager::~Pager() {
  uint32_t num_full_pages = (num_rows_ % rowsPerPage == 0)
                                ? num_rows_ / rowsPerPage
                                : num_rows_ / rowsPerPage + 1;

  for (uint32_t i = 0; i < num_full_pages; ++i) {
    if (pages_[i] == nullptr) {
      continue;
    }
    Flush(i);
    // TODO(natsunoyoru97): Make page an ADT
    // and consider use unique_ptr
    delete pages_[i];
  }

  int ret = close(fd_);
  if (ret == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to close db file\n";
    // TODO(natsunoyoru97): Use Status/StatusOr instead
    exit(EXIT_FAILURE);
  }

  // TODO(natsunoyoru97): double free
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
    // TODO(natsunoyoru97): Use Status/StatusOr instead
    exit(EXIT_FAILURE);
  }

  if (pages_[page_num] == nullptr) {
    // TODO(natsunoyoru97): Make page an ADT
    // and consider use unique_ptr
    char* page = new char[kPageSize];
    uint32_t num_pages = file_len_ / kPageSize;

    if (file_len_ % kPageSize != 0) {
      num_pages++;
    }

    if (page_num < num_pages) {
      ssize_t bytes_read =
          pread(fd_, page, kPageSize, page_num * kPageSize);
      if (bytes_read == -1) {
        // TODO(natsunoyoru97): Use glog to replace the cout
        std::cout << "Fail to read the file\n";
        // TODO(natsunoyoru97): Use Status/StatusOr instead
        exit(EXIT_FAILURE);
      }
    }

    pages_[page_num] = page;
  }

  return pages_[page_num];
}

void Pager::Flush(uint32_t page_start) {
  if (pages_[page_start] == nullptr) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Tried to flush null page\n";
    // TODO(natsunoyoru97): Use Status/StatusOr instead
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_written =
      pwrite(fd_, pages_[page_start], kPageSize,
             page_start * kPageSize);

  if (bytes_written == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to write to db\n";
    // TODO(natsunoyoru97): Use Status/StatusOr instead
    exit(EXIT_FAILURE);
  }
}
}  // namespace storage
