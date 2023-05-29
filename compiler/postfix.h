#pragma once
#include <string>
#include <vector>
#include "syntax.h"
#include "stack.h"
#include "unsortListTable.h"
#include "hierarchyList.h"
#include "exitCodes.h"

class TPostfix {
private:
  Operations operation;
  std::shared_ptr<UnsortListTable<std::string, Variable>> table;
  std::vector<std::string> infix;
  std::vector<std::string> postfix;
  TStack<bool> logicBlock;
  bool lastCompare = false;

  void operator=(const TPostfix&) = delete; // Запрет на присваивание

  void ToPostfix(); // Преобразование infix в vector<string> postfix
  bool IsNumber(const std::string& lexem);

public:
  TPostfix() { table = nullptr; }
  TPostfix(std::shared_ptr<UnsortListTable<std::string, Variable>> _table) { table = _table; }

  void Init(std::shared_ptr<UnsortListTable<std::string, Variable>> _table) { table = _table; }

  std::vector<std::string> GetPostfix() const { return postfix; }

  std::string GetStringPostfix() const {
    std::string tmp;
    for (const std::string& elem : postfix)
      tmp += elem + ' ';
    return tmp;
  }

  void Execute(HierarchyList::const_iterator* it);
  void UpdateTable(HierarchyList::const_iterator it);
};
