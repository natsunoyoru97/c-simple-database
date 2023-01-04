// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 23-1-2.
//

#include "pager.h" // NOLINT

#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

// TODO(natsunoyoru): duplicate declaration, this will be in an Util class
// The size of bytes a page has
constexpr uint32_t PAGE_SIZE = 256;
// The fixed-size of rows that contains in a page
constexpr uint32_t ROWS_PER_PAGE = 100;
// The size of bytes a row has
constexpr uint32_t ROW_SIZE = 100;

namespace pager {
Pager::Pager(const char* filename) {
  int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

  if (fd == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to open the file\n";
    exit(EXIT_FAILURE);
  }

  off_t file_len = lseek(fd, 0, SEEK_END);

  fd_ = fd;
  file_len_ = file_len;

  for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
    pages_[i] = nullptr;
  }

  uint32_t num_rows = file_len_ / ROW_SIZE;
  num_rows_ = num_rows;
}

Pager::~Pager() {
  uint32_t num_full_pages = num_rows_ / ROWS_PER_PAGE;

  for (uint32_t i = 0; i < num_full_pages; ++i) {
    if (pages_[i] == nullptr) {
      continue;
    }
    Flush(i, PAGE_SIZE);
    // NOTE(natsunoyoru97): void* cannot apply for new[]/delete[]
    // TODO(natsunoyoru97): Make page an abstract object
    free(pages_[i]);
    pages_[i] = nullptr;
  }

  // There may be a partial page to write to the end of the file
  uint32_t num_additional_rows = num_rows_ % ROWS_PER_PAGE;
  if (num_additional_rows > 0) {
    uint32_t page_num = num_full_pages;
    if (pages_[page_num] != NULL) {
      Flush(page_num, num_additional_rows * ROW_SIZE);
      // NOTE(natsunoyoru97): void* cannot apply for new[]/delete[]
      // TODO(natsunoyoru97): Make page an abstract object
      free(pages_[page_num]);
      pages_[page_num] = NULL;
    }
  }

  int ret = close(fd_);
  if (ret == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to close db file\n";
    exit(EXIT_FAILURE);
  }
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    void* page = pages_[i];
    if (page != nullptr) {
      // NOTE(natsunoyoru97): void* cannot apply for new[]/delete[]
      // TODO(natsunoyoru97): Make page an ADT
      free(page);
      pages_[i] = nullptr;
    }
  }
}

void* Pager::GetPage(uint32_t page_num) {
  if (page_num >= TABLE_MAX_PAGES) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Tried to fetch page number out of bounds.\n";
    exit(EXIT_FAILURE);
  }

  if (pages_[page_num] == nullptr) {
    // NOTE(natsunoyoru97): void* cannot apply for new[]/delete[]
    // TODO(natsunoyoru97): Make page an ADT
    void* page = malloc(PAGE_SIZE);
    uint32_t num_pages = file_len_ / PAGE_SIZE;

    if (file_len_ % PAGE_SIZE != 0) {
      num_pages++;
    }

    if (page_num < num_pages) {
      lseek(fd_, page_num * PAGE_SIZE, SEEK_SET);
      ssize_t bytes_read = read(fd_, page, PAGE_SIZE);
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

void Pager::Flush(uint32_t page_num, uint32_t size) {
  if (pages_[page_num] == NULL) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Tried to flush null page\n";
    exit(EXIT_FAILURE);
  }

  off_t offset = lseek(fd_, page_num * PAGE_SIZE, SEEK_SET);

  if (offset == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to seek the page\n";
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_written = pwrite(fd_, pages_[page_num], size, 0);

  if (bytes_written == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to write to db\n";
    exit(EXIT_FAILURE);
  }
}
}  // namespace pager
