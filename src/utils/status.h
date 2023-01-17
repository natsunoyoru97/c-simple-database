// Copyright 2023 natsunoyoru97
//
// Created by natsunoyoru on 23-1-17.
//
// Status code used to represent error, see
// https://grpc.github.io/grpc/core/md_doc_statuscodes.html

#ifndef SRC_UTILS_STATUS_H_
#define SRC_UTILS_STATUS_H_

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "status_code.h"

namespace utils {

class Status;
namespace internal {
void SetPayload(Status&, std::string key, std::string payload);
absl::optional<std::string> GetPayload(Status const&, std::string const& key);
}  // namespace internal

/**
 * Describes the cause of the error with structured details.
 *
 * @see https://cloud.google.com/apis/design/errors#error_info
 */
class ErrorInfo {
 public:
  ErrorInfo() = default;
  explicit ErrorInfo(std::string reason, std::string domain,
                     std::unordered_map<std::string, std::string> metadata)
      : reason_(std::move(reason)),
        domain_(std::move(domain)),
        metadata_(std::move(metadata)) {}

  std::string const& reason() const { return reason_; }
  std::string const& domain() const { return domain_; }
  std::unordered_map<std::string, std::string> const& metadata() const {
    return metadata_;
  }

  friend bool operator==(ErrorInfo const&, ErrorInfo const&);
  friend bool operator!=(ErrorInfo const&, ErrorInfo const&);

 private:
  std::string reason_;
  std::string domain_;
  std::unordered_map<std::string, std::string> metadata_;
};

/**
 * Represents success or an error with info about the error.

 * This class is typically used to indicate whether or not a function or other
 * operation completed successfully. Success is indicated by an "OK" status. OK
 * statuses will have `.code() == StatusCode::kOk` and `.ok() == true`, with
 * all other properties having empty values. All OK statuses are equal. Any
 * non-OK `Status` is considered an error. Users can inspect the error using
 * the member functions, or they can simply stream the `Status` object, and it
 * will print itself in some human readable way (the streamed format may change
 * over time and you should *not* depend on the specific format of a streamed
 * `Status` object remaining unchanged).
 *
 * This is a regular value type that can be copied, moved, compared for
 * equality, and streamed.
 */
class Status {
 public:
  Status();
  ~Status();
  Status(Status const&);
  Status& operator=(Status const&);
  Status(Status&&) noexcept;
  Status& operator=(Status&&) noexcept;

  /**
   * Constructs a Status with the given @p code and @p message.
   *
   * Ignores @p message if @p code is `StatusCode::kOk`.
   */
  explicit Status(StatusCode code, std::string message, ErrorInfo info = {});

  bool ok() const { return !impl_; }
  StatusCode code() const;
  std::string const& message() const;
  ErrorInfo const& error_info() const;

  friend inline bool operator==(Status const& a, Status const& b) {
    return (a.ok() && b.ok()) || Equals(a, b);
  }
  friend inline bool operator!=(Status const& a, Status const& b) {
    return !(a == b);
  }
  friend std::ostream& operator<<(std::ostream& os, Status const& s);

 private:
  static bool Equals(Status const& a, Status const& b);
  friend void internal::SetPayload(Status&, std::string, std::string);
  friend absl::optional<std::string> internal::GetPayload(Status const&,
                                                          std::string const&);

  class Impl;
  // A null `impl_` is an OK status. Only non-OK Statuses allocate an Impl.
  std::unique_ptr<Impl> impl_;
};

/**
 * A runtime error that wraps a `google::cloud::Status`.
 */
class RuntimeStatusError : public std::runtime_error {
 public:
  explicit RuntimeStatusError(Status status);

  Status const& status() const { return status_; }

 private:
  Status status_;
};

}  // namespace utils

#endif  // SRC_UTILS_STATUS_H_
