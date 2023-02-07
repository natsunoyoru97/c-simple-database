// Copyright 2023 natsunoyoru97
//
// Created by natsunoyoru on 23-1-9.
//

#ifndef SRC_CONFIG_CONFIG_H_
#define SRC_CONFIG_CONFIG_H_

// The fixed-size of rows that contains in a page
constexpr uint32_t rowsPerPage = 10;
// The size of bytes a row has
constexpr uint32_t rowSize = 64;
// The size of bytes a page has
constexpr uint32_t kPageSize = rowsPerPage * rowSize;
// Maximum pages a table stores
constexpr uint32_t TABLE_MAX_PAGES = 1024;

#endif  // SRC_CONFIG_CONFIG_H_
