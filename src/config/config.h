// Copyright 2023 natsunoyoru97
//
// Created by natsunoyoru on 23-1-9.
//

#ifndef SRC_CONFIG_CONFIG_H_
#define SRC_CONFIG_CONFIG_H_

// The size of bytes a page has
constexpr uint32_t kPageSize = 256;
// The fixed-size of rows that contains in a page
constexpr uint32_t rowsPerPage = 100;
// The size of bytes a row has
constexpr uint32_t rowSize = 100;
// Maximum pages a table stores
constexpr uint32_t TABLE_MAX_PAGES = 1024;

#endif  // SRC_CONFIG_CONFIG_H_
