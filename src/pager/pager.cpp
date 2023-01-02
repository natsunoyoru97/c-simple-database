// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 23-1-2.
//

#include "src/pager/pager.h"

#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

// TODO(natsunoyoru): duplicate declaration, this will be in an Util class
constexpr uint32_t PAGE_SIZE = 256;

namespace pager {
Pager::Pager(const char* filename) {
  int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

  if (fd == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to open the file\n";
    exit(EXIT_FAILURE);
  }

  off_t file_len = lseek(fd, 0, SEEK_END);

  file_descriptor_ = fd;
  file_len_ = file_len;

  for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
    pages_[i] = nullptr;
  }
}

Pager::~Pager() {
  int ret = close(file_descriptor_);
  if (ret == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to close db file\n";
    exit(EXIT_FAILURE);
  }
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    void* page = pages_[i];
    if (page != nullptr) {
      // NOTE(natsunoyoru97): void* cannot apply for new[]/delete[]
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
    void* page = malloc(PAGE_SIZE);
    uint32_t num_pages = file_len_ / PAGE_SIZE;

    if (file_len_ % PAGE_SIZE != 0) {
      num_pages++;
    }

    if (page_num < num_pages) {
      lseek(file_descriptor_, page_num * PAGE_SIZE, SEEK_SET);
      ssize_t bytes_read = read(file_descriptor_, page, PAGE_SIZE);
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

  off_t offset = lseek(file_descriptor_, page_num * PAGE_SIZE, SEEK_SET);

  if (offset == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to seek the page\n";
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_written = write(file_descriptor_, pages_[page_num], size);

  if (bytes_written == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to write to db\n";
    exit(EXIT_FAILURE);
  }
}
}  // namespace pager
