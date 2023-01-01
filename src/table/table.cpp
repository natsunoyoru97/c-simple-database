//
// Created by natsunoyoru on 22-12-31.
//

#include "table.h"

#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

const uint32_t ROWS_PER_PAGE = 100;
const uint32_t ROW_SIZE = 100;
const uint32_t PAGE_SIZE = 256;

namespace table {
Pager::Pager(const char* filename) {
  int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

  if (fd == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to open the file\n";
    exit(EXIT_FAILURE);
  }

  off_t file_len = lseek(fd, 0, SEEK_END);

  this->file_descriptor = fd;
  this->file_len = file_len;

  for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
    this->pages[i] = nullptr;
  }
}

Pager::~Pager() {}

void* Pager::GetPage(uint32_t page_num) {
  if (page_num >= TABLE_MAX_PAGES) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Tried to fetch page number out of bounds.\n";
    exit(EXIT_FAILURE);
  }

  if (this->pages[page_num] == nullptr) {
    // NOTE(natsunoyoru97): void* cannot apply for new[]/delete[]
    void* page = malloc(PAGE_SIZE);
    uint32_t num_pages = this->file_len / PAGE_SIZE;

    if (this->file_len % PAGE_SIZE != 0) {
      num_pages++;
    }

    if (page_num < num_pages) {
      lseek(this->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
      ssize_t bytes_read = read(this->file_descriptor, page, PAGE_SIZE);
      if (bytes_read == -1) {
        // TODO(natsunoyoru97): Use glog to replace the cout
        std::cout << "Fail to read the file\n";
        exit(EXIT_FAILURE);
      }
    }

    this->pages[page_num] = page;
  }

  return this->pages[page_num];
}

void Pager::Flush(uint32_t page_num, uint32_t size) {
  if (this->pages[page_num] == NULL) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Tried to flush null page\n";
    exit(EXIT_FAILURE);
  }

  off_t offset = lseek(this->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);

  if (offset == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to seek the page\n";
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_written =
      write(this->file_descriptor, this->pages[page_num], size);

  if (bytes_written == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to write to db\n";
    exit(EXIT_FAILURE);
  }
}

Table::Table(const char* filename) {
  Pager* pager = new Pager(filename);
  uint32_t num_rows = pager->file_len / ROW_SIZE;

  this->pager = pager;
  this->num_rows = num_rows;
}

Table::~Table() {
  Pager* pager = this->pager;
  uint32_t num_full_pages = this->num_rows / ROWS_PER_PAGE;

  for (uint32_t i = 0; i < num_full_pages; ++i) {
    if (pager->pages[i] == nullptr) {
      continue;
    }
    pager->Flush(i, PAGE_SIZE);
    // NOTE(natsunoyoru97): void* cannot apply for new[]/delete[]
    free(pager->pages[i]);
    pager->pages[i] = nullptr;
  }

  // There may be a partial page to write to the end of the file
  uint32_t num_additional_rows = (this->num_rows) % ROWS_PER_PAGE;
  if (num_additional_rows > 0) {
    uint32_t page_num = num_full_pages;
    if (pager->pages[page_num] != NULL) {
      pager->Flush(page_num, num_additional_rows * ROW_SIZE);
      // NOTE(natsunoyoru97): void* cannot apply for new[]/delete[]
      free(pager->pages[page_num]);
      pager->pages[page_num] = NULL;
    }
  }

  int ret = close(pager->file_descriptor);
  if (ret == -1) {
    // TODO(natsunoyoru97): Use glog to replace the cout
    std::cout << "Fail to close db file\n";
    exit(EXIT_FAILURE);
  }
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    void* page = pager->pages[i];
    if (page != nullptr) {
      // NOTE(natsunoyoru97): void* cannot apply for new[]/delete[]
      free(page);
      pager->pages[i] = nullptr;
    }
  }
}

void* Table::RowSlot(uint32_t row_num) {
  uint32_t page_num = row_num / ROWS_PER_PAGE;
  void* page = this->pager->GetPage(page_num);
  uint32_t row_offset = row_num % ROWS_PER_PAGE;
  uint32_t byte_offset = row_offset * ROW_SIZE;

  return page + byte_offset;
}

}  // namespace table
