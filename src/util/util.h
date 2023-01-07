// Copyright 2023 natsunoyoru97
//
// Created by natsunoyoru on 23-1-6.
//

#ifndef SRC_UTIL_UTIL_H_
#define SRC_UTIL_UTIL_H_

// The size of bytes a page has
constexpr uint32_t kPageSize = 256;
// The fixed-size of rows that contains in a page
constexpr uint32_t rowsPerPage = 100;
// The size of bytes a row has
constexpr uint32_t rowSize = 100;

#endif  // SRC_UTIL_UTIL_H_
