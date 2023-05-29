#pragma once
#include "syntax.h"
#include "exitCodes.h"
#include "hierarchyList.h"
#include "unsortListTable.h"

class SyntChecker
{
private:
  SyntChecker() = delete;
  ~SyntChecker() = delete;

  static bool IsNumber(const std::string& lexem) { return (lexem.find_first_not_of("1234567890.") == std::string::npos); }

  static bool IsVariable(const std::string& lexem)
  {
    Syntax synt;
    return (!synt.isSyntax(lexem) && lexem.find_first_of("0123456789") != 0 && lexem.find_first_not_of("0123456789.") != std::string::npos);
  }

  static bool IsSyntax(const std::string& lexem)
  {
    Syntax synt;
    return synt.isSyntax(lexem);
  }

  static bool BracketsCorrect(const std::string& str)
  {
    TStack<bool> stack; // Стек для проверки наличия '('
    for (const char& elem : str) {
      if (elem == '(') {
        stack.push(true);
        continue;
      }
      if (elem == ')') {
        if (stack.empty()) // Если стек пуст, то нет пары для ')' -> ошибка
          return false;
        stack.pop();
        continue;
      }
    }
    if (!stack.empty()) // Если стек не пуст, то слишком мало ')' -> ошибка
      return false;
    return true;
  }

public:
  static ExitCodes::CODES Check(std::shared_ptr<HierarchyList> ls, std::shared_ptr < UnsortListTable<std::string, Variable>> table, int& numStr)
  {
    Syntax synt;
    std::vector<bool> blocks = { false, false, false }; // program, const, var
    bool isReady = false;;
    UnsortListTable<std::string, Variable> constTable, varTable;
    auto it = ls->begin();
    numStr = 0;
    if (table != nullptr)
      table->clear();

    while (isReady == false)
    {
      ++numStr;
      std::vector<std::string> strParams = Parser(*it);
      if (strParams.size() == 0)
      {
        it.next();
        continue;
      }
      std::string tmp = strParams[0];
      if (tmp == "program")
      {
        if (blocks[0] == true)
        {
          return ExitCodes::UNABLE_TO_CREATE_SECOND_PROGRAM;
        }
        if (strParams.size() != 3 || !IsVariable(strParams[1]))
        {
          // Всегда три параметра: program, name, ;
          // Имя - не слово из синтаксиса
          // Имя подчиняется правилам названия переменных
          return ExitCodes::INCORRECT_PROGRAM_NAME;
        }
        it.next();
        blocks[0] = true;
        continue;
      }
      else if (blocks[0] != true)
      {
        // Блок program самый первый
        return ExitCodes::INCORRECT_ORDER_WITH_PROGRAM;
      }
      else if (tmp == "const")
      {
        if (blocks[1] == true)
        {
          return ExitCodes::UNABLE_TO_CREATE_CONST;
        }
        it.down();
        while (!it)
        {
          ++numStr;
          strParams = Parser(*it);
          // pi: double = 3.1415926;
          // Всегда упорядочено: название, :, тип, знак равенства, число, ;
          if (strParams.size() != 6)
            return ExitCodes::UNEXPECTED_EXPRESSION;
          if (!IsVariable(strParams[0]))
            return ExitCodes::INCORRECT_VARIABLE_NAME;
          if (strParams[1] != ":" || strParams[3] != "=")
            return ExitCodes::SEPARATOR_WAS_EXPECTED;
          if (strParams[2] != "double" && strParams[2] != "integer")
            return ExitCodes::INVALID_VARIABLE_TYPE;
          if (!IsNumber(strParams[4]))
            return ExitCodes::INVALID_VARIABLE_VALUE;
          if (strParams[5] != ";")
            return ExitCodes::SEMICOLON_IS_REQUIRED;
          if (constTable.Find(strParams[0]) != nullptr)
            return ExitCodes::DOUBLE_DECLARATION;
          if (strParams[2] == "double")
            constTable.Insert(strParams[0], -DBL_MAX);
          if (strParams[2] == "integer")
            constTable.Insert(strParams[0], -INT_MAX);
          it.next();
        }
        it.up();
        it.next();
        blocks[1] = true;
        continue;
      }
      else if (tmp == "var")
      {
        if (blocks[2] == true)
        {
          return ExitCodes::UNABLE_TO_CREATE_VAR;
        }
        it.down();
        while (!it)
        {
          ++numStr;
          strParams = Parser(*it);
          if (strParams.size() < 4) // название, разделитель, тип, ;
            return ExitCodes::UNEXPECTED_EXPRESSION;
          if (strParams[strParams.size() - 1] != ";")
            return ExitCodes::SEMICOLON_IS_REQUIRED;
          if (strParams[strParams.size() - 2] != "double" && strParams[strParams.size() - 2] != "integer")
            return ExitCodes::INVALID_VARIABLE_TYPE;
          if (strParams[strParams.size() - 3] != ":")
            return ExitCodes::UNEXPECTED_EXPRESSION;
          //   numa1 , numa2 : integer ;
          for (int j = 0; j < strParams.size() - 3; j++)
          {
            if (j % 2 == 0) // Название переменной
            {
              if (!IsVariable(strParams[j]))
                return ExitCodes::INCORRECT_VARIABLE_NAME;
              if (constTable.Find(strParams[j]) != nullptr || varTable.Find(strParams[j]) != nullptr)
                return ExitCodes::DOUBLE_DECLARATION;
              if (strParams[strParams.size() - 2] == "double")
                varTable.Insert(strParams[j], -DBL_MAX);
              if (strParams[strParams.size() - 2] == "integer")
                varTable.Insert(strParams[j], -INT_MAX);
            }
            else
            {
              if (strParams[j] != ",") // Разделитель
                return ExitCodes::UNEXPECTED_EXPRESSION;
            }
          }
          it.next();
        }
        it.up();
        it.next();
        blocks[1] = true; // Так как строгая последовательность
        blocks[2] = true;
        continue;
      }
      else if (tmp == "begin")
      {
        isReady = true;
        *table = constTable + varTable;
      }
    }

    TStack<bool> stack;

    while (*it != "end.")
    {
      ++numStr;
      if (!BracketsCorrect(*it))
        return ExitCodes::INCORRECT_BRACKETS;
      std::vector<std::string> strParams = Parser(*it);
      if (strParams.empty())
      {
        it.next();
        continue;
      }
      std::string firstWord = strParams[0];
      if (IsSyntax(firstWord) == true)
      {

        if (firstWord == "begin" || firstWord == "end" || firstWord == "end.")
        {
          if (strParams.size() > 1)
            return ExitCodes::UNEXPECTED_EXPRESSION;
          if (firstWord == "begin")
          {
            it.down();
            stack.push(true);
            continue;
          }
          else
          {
            if (stack.empty())
              return ExitCodes::TOO_MUCH_END;
            it.next();
            stack.pop();
            continue;
          }
          if (!stack.empty())
            return ExitCodes::TOO_MUCH_BEGIN;
        }
        else if (firstWord == ";" && strParams.size() > 1)
        {
          return ExitCodes::UNEXPECTED_EXPRESSION;
        }
        else if (firstWord == "if")
        {
          it.down();
          if (!(!it))
          {
            it.up();
            it.next();
          }
        }
        else if (firstWord == "else")
        {
          it.down();
          if (!it)
          {
            continue;
          }
          else //есть begin
          {
            it.up();
            //Чтобы пропустить begin end
            it.next();
            it.next();
            it.next();
          }
        }
        else if (firstWord == "write" || firstWord == "writeln")
        {

          if (strParams.size() < 5)
            return ExitCodes::NOT_ENOUGH_ARGUMENTS_TO_CALL_THE_FUNCTION;
          int numCommas = 0;
          for (int i = 0; i < strParams.size(); i++)
            if (strParams[i] == ",")
              numCommas++;
          if (numCommas > 1)
            return ExitCodes::UNEXPECTED_EXPRESSION;
          it.next();
        }
        else if (firstWord == "read")
        {
          if (strParams.size() != 5) // read ( var ) ;
            return ExitCodes::UNEXPECTED_EXPRESSION;
          if (strParams[1] != "(" || strParams[3] != ")" || strParams[strParams.size() - 1] != ";")
            return ExitCodes::UNEXPECTED_EXPRESSION;
          if (table->Find(strParams[2]) == nullptr)
            return ExitCodes::NO_SUCH_VARIABLE;
          it.next();
        }
      }
      else
      {
        if (strParams[1] != ":=" || strParams[strParams.size() - 1] != ";") // только num := 3 ;, других случаев у нас нет
        {
          return ExitCodes::UNEXPECTED_EXPRESSION;
        }
        else
        {
          if (constTable.Find(strParams[0]) != nullptr)
          {
            return ExitCodes::ATTEMPT_TO_CHANGE_A_CONSTANT_VARIABLE;
          }
        }
        it.next();
      }
      while (!(!it))
      {
        it.up();
        it.next();
      }
    }

    return ExitCodes::ALL_IS_GOOD;
  }
};
