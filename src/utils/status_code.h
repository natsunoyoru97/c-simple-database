// Copyright 2023 natsunoyoru97
//
// Created by natsunoyoru on 23-1-17.
//
// Status code used to represent error, see
// https://grpc.github.io/grpc/core/md_doc_statuscodes.html

#ifndef SRC_UTILS_STATUS_CODE_H_
#define SRC_UTILS_STATUS_CODE_H_

#include <iostream>
#include <string>

namespace utils {

enum class StatusCode {
  // Not an error; returned on success.
  kOk = 0,

  kCancelled = 1,
  kUnknown = 2,
  kInvalidArgument = 3,
  kDeadlineExceeded = 4,
  kNotFound = 5,
  kAlreadyExists = 6,
  kPermissionDenied = 7,
  kUnauthenticated = 16,
  kResourceExhausted = 8,
  kFailedPrecondition = 9,
  kAborted = 10,
  kOutOfRange = 11,
  kUnimplemented = 12,
  kInternal = 13,
  kUnavailable = 14,
  kDataLoss = 15,
};

std::string StatusCodeToString(StatusCode code);
std::ostream& operator<<(std::ostream& os, StatusCode code);

}  // namespace utils

#endif  // SRC_UTILS_STATUS_CODE_H_
