// Copyright 2022 natsunoyoru97
//
// Created by natsunoyoru on 23-1-2.
//

#include "pager.h"  //NOLINT

#include <fcntl.h>

#include <cstdlib>
#include <iostream>

namespace storage {

FileHandler::FileHandler(const char* filename, int fd) {
  fd_ = fd;
  // TODO(natsunoyoru97): consider about the case that the file is created and
  // it is 0
  off_t file_len = lseek(fd, 0, SEEK_END);
  file_len_ = file_len;
}

FileHandler::~FileHandler() {
  int ret = close(fd_);
  if (ret == -1) {
    std::cout << "Fail to close db file\n";
    exit(EXIT_FAILURE);
  }
}

absl::StatusOr<FileHandler*> FileHandler::InitFileHandler(
    const char* filename) {
  int fd = open(filename, O_RDWR | O_APPEND, S_IWUSR | S_IRUSR);
  return (fd == -1)
             ? absl::StatusOr<FileHandler*>(
                   absl::FailedPreconditionError("Fail to open the file"))
             : absl::StatusOr<FileHandler*>(new FileHandler(filename, fd));
}

uint32_t FileHandler::GetFileLen() { return file_len_; }

int FileHandler::GetFd() { return fd_; }

Pager::Pager(const char* filename, FileHandler* file_handler) {
  file_handler_ = file_handler;
  num_rows_ = (file_handler_->GetFileLen() - 1) / rowSize + 1;
  pages_.fill(nullptr);
}

absl::StatusOr<Pager*> Pager::InitPager(const char* filename) {
  absl::StatusOr<FileHandler*> result = FileHandler::InitFileHandler(filename);
  return result.ok() ? absl::StatusOr<Pager*>(new Pager(filename, *result))
                     : absl::StatusOr<Pager*>(result.status());
}

Pager::~Pager() {
  // uint32_t num_full_pages = (num_rows_ - 1) / rowsPerPage + 1;
  delete file_handler_;
  for (int i = 0; i < TABLE_MAX_PAGES; ++i) {
    if (pages_[i] != nullptr) {
      delete[] pages_[i];
    }
  }
}

FileHandler* Pager::GetFileHandler() { return file_handler_; }

const char* Pager::GetPage(uint32_t page_num) {
  if (page_num >= TABLE_MAX_PAGES) {
    std::cout << "Tried to fetch page number out of bounds.\n";
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
          pread(file_handler_->GetFd(), page, kPageSize, page_num * kPageSize);
      if (bytes_read == -1) {
        std::cout << "Fail to read the file\n";
        exit(EXIT_FAILURE);
      }
    }

    pages_[page_num] = page;
  }

  return pages_[page_num];
}

absl::Status Pager::Flush(uint32_t page_start) {
  if (pages_[page_start] == nullptr) {
    std::cout << "Tried to flush null page\n";
    exit(EXIT_FAILURE);
  }

  // TODO(natsunoyoru97): the bytes to allocate may be greater than the space!
  ssize_t bytes_written = pwrite(file_handler_->GetFd(), pages_[page_start],
                                 kPageSize, page_start * kPageSize);

  if (bytes_written == -1) {
    std::cout << "Fail to write to db\n";
    exit(EXIT_FAILURE);
    return absl::FailedPreconditionError("Fail to write to the database");
  }

  return absl::OkStatus();
}
}  // namespace storage
