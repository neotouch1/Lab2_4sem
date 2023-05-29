#pragma once
#include "exitCodes.h"
#include "postfix.h"
#include "hierarchyList.h"
#include "unsortListTable.h"

class ExecObj
{
private:
  std::shared_ptr<HierarchyList> ls;
  std::shared_ptr<UnsortListTable<std::string, Variable>> table; // TODO
  TPostfix postfix;
public:
  ExecObj(std::shared_ptr<HierarchyList> _ls, std::shared_ptr<UnsortListTable<std::string, Variable>> _table)
  {
    ls = _ls;
    table = _table;
    postfix.Init(_table);
  }

  ExecObj(const ExecObj& exObj)
  {
    ls = exObj.ls;
    table = exObj.table;
    postfix.Init(table);
  }

  void Execute()
  {
    auto it = ls->cbegin();
    while ((*it) != "end.")
    {
      try
      {
        postfix.Execute(&it);
      }
      catch (ExitCodes::CODES exitCode)
      {
        std::cout << exitCode;
        break;
      }
    }

  }

  ~ExecObj() {}
};
