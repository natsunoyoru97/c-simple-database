// Copyright 2022 natsunoyoru97

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <algorithm>
#include <csignal>
#include <iostream>
#include <vector>

#include "absl/strings/str_join.h"
#include "pager/pager.h"
#include "storage/storage.h"

DEFINE_string(test, "simple database", "the flag for test");

namespace {
// Tokenizer that takes the space as the delimiter
static std::vector<std::string> Tokenize(const std::string& s) {
  std::vector<std::string> tokens;
  std::string curr;
  for (char c : s) {
    if (c == ' ') {
      if (!curr.empty()) {
        tokens.push_back(curr);
      }
      curr.clear();
      continue;
    }

    curr += c;
  }

  if (!curr.empty()) {
    tokens.push_back(curr);
  }

  return tokens;
}

void Select(const std::vector<std::string>& params) {
  LOG(INFO) << "This is select function." << std::endl;

  for (const std::string& s : params) {
    LOG(INFO) << s << " ";
  }
  LOG(INFO) << "\n";
}

void Insert(const std::vector<std::string>& params) {
  LOG(INFO) << "This is insert function." << params[0] << std::endl;
}

void Delete(const std::vector<std::string>& params) {
  LOG(INFO) << "This is delete function." << params[0] << std::endl;
}

static void ExecuteDb(const std::string& command,
                      const std::vector<std::string>& params) {
  if (command == "select") {
    Select(params);
  } else if (command == "insert") {
    Insert(params);
  } else if (command == "delete") {
    Delete(params);
  } else if (command == "exit") {
    std::cout << "Are you sure to exit? y/n"
              << "\n";
    while (true) {
      std::string c;
      std::cin >> c;
      std::transform(c.begin(), c.end(), c.begin(), ::tolower);

      if (c == "y") {
        std::cout << "Exit from the simple database cli."
                  << "\n";
        exit(0);
      }
      if (c == "n") {
        std::cout << "Cancel to exit."
                  << "\n";
        break;
      }
    }
  } else {
    std::cout << "Invalid command."
              << "\n";
  }
}

void InitGlog(const char* argv0) {
  google::InitGoogleLogging(argv0);
  FLAGS_logtostderr = true;
}

void TestStorage() {
  storage::Table* tbl = storage::Table::InitTable("./basic.db");
  std::cerr << sizeof(tbl) << "\n";
  tbl->GetRowSlot(1);
}

}  // namespace

int main(int argc, char** argv) {
  /* TODO(natsunoyoru97): It seems that the result is not as expected.
   It is a bug to fix.
   */
  std::signal(SIGPIPE, SIG_IGN);

  InitGlog(argv[0]);
  std::cout << argc << "\n";

  TestStorage();

  // while (true) {
  std::string command;
  std::cin >> command;

  std::vector<std::string> params;
  std::string param;

  getline(std::cin, param);
  LOG(INFO) << param << "\n";

  ExecuteDb(command, Tokenize(param));
  //}
}
