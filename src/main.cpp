// Copyright 2022 natsunoyoru97

#include <algorithm>
#include <iostream>
#include <vector>

// Tokenizer that takes the space as the delimiter
std::vector<std::string> Tokenize(const std::string& s) {
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
  std::cout << "This is select function." << std::endl;

  for (const std::string& s : params) {
    std::cout << s << " ";
  }
  std::cout << "\n";
}

void Insert(const std::vector<std::string>& params) {
  std::cout << "This is insert function." << std::endl;
}

void Delete(const std::vector<std::string>& params) {
  std::cout << "This is delete function." << std::endl;
}

void ExecuteDb(const std::string& command,
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
        std::cout << "Exit from the simple database cli." << std::endl;
        exit(0);
      }
      if (c == "n") {
        std::cout << "Cancel to exit." << std::endl;
        break;
      }
    }
  } else {
    std::cout << "Invalid command." << std::endl;
  }
}

int main(int argc, char** argv) {
  std::cout << "Simple database" << std::endl;

  while (true) {
    std::string command;
    std::cin >> command;

    std::vector<std::string> params;
    std::string param;

    getline(std::cin, param);
    std::cout << param << "\n";

    ExecuteDb(command, Tokenize(param));
  }
}
