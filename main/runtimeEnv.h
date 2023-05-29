#pragma once
#include <iostream>
#include <conio.h>
#include <memory>
#include "exitCodes.h"
#include "syntChecker.h"
#include "execObj.h"
#include "unsortListTable.h"
#include "postfix.h"

class RuntimeEnv {
  UnsortListTable<std::string, ExecObj> progs;

  void istreamCleaner() {
    while (getchar() != '\n');
    std::cin.clear();
  }

  int Clamp(int border1, int border2) {
    int choice = INT_MIN;
    while (choice < border1 || choice > border2) {
      std::cin >> choice;
      if (choice < border1 || choice > border2) {
        std::cout << "Invalid number entered. Try again.\n";
        istreamCleaner();
      }
    }
    return choice;
  }
public:
  RuntimeEnv() {}

  void Run() {
    int numStr = 0;
    int choice;
    ExitCodes::CODES code;
    std::string tmp;

    // UnsortListTable<std::string, Variable>* t = nullptr;
    std::shared_ptr<UnsortListTable<std::string, Variable>> t; ;
    std::shared_ptr<HierarchyList> l;
    ExecObj* prog = nullptr;

    while (true) {
      system("cls");
      std::cout <<
        " _--*** Pascal-- Emulator ***--_" << std::endl <<
        "Menu: " << std::endl <<
        "1 Load Program" << std::endl <<
        "2 Run Program" << std::endl <<
        "3 Show program list" << std::endl <<
        "0 Exit" << std::endl;
      choice = Clamp(0, 3);
      switch (choice) {
      case 0: // EXIT
        std::cout << "Goodbye" << std::endl;
        return;
      case 1: // LOAD PROGRAM
        std::cout << "Enter path: ";
        istreamCleaner();
        getline(std::cin, tmp);
        l = std::make_shared<HierarchyList>();
        try { l->Build(tmp); }
        catch (int line) {
          if (line == -1)
            std::cout << "No such file" << std::endl;
          else
            std::cout << ExitCodes::CODES::WRONG_TABULATION << " on line " << line << std::endl;
          break;
        }
        std::cout << *l << std::endl << std::endl;
        t = std::make_shared<UnsortListTable<std::string, Variable>>();
        if ((code = SyntChecker::Check(l, t, numStr)) != ExitCodes::ALL_IS_GOOD) {
          std::cout << code << " on line " << numStr << std::endl;
          system("pause");
          break;
        }
        std::cout << "Enter name: ";
        getline(std::cin, tmp);
        progs.Insert(tmp, ExecObj(l, t));
        system("pause");
        break;
      case 2: // RUN PROGRAM
        std::cout << "Enter name: ";
        istreamCleaner();
        getline(std::cin, tmp);
        if ((prog = progs.Find(tmp)) == nullptr) {
          std::cout << "No such program" << std::endl;
          break;
        }
        system("cls");
        prog->Execute();
        istreamCleaner();
        system("pause");
        break;
      case 3: // PRINT PROGRAMS
        std::cout << "Program list:" << std::endl;
        istreamCleaner();
        progs.PrintKeys();
        istreamCleaner();
        break;
      default:
        std::cout << "No such command" << std::endl;
      }
      system("cls");
    }
  }
  ~RuntimeEnv() {}
};
