#include "postfix.h"
#include "syntax.h"

void TPostfix::ToPostfix()
{
  TStack<std::string> opStack;

  for (int i = 0; i < infix.size(); i++) {
    std::string lexem = infix[i];

    if (lexem == "," || lexem == ";" || lexem == "then")
      continue;

    if (!operation.isOperation(lexem)) {
      postfix.push_back(lexem);
      continue;
    }
    else {
      if (lexem == "(") {
        opStack.push(lexem);
        continue;
      }

      if (lexem == ")") {
        //��������� �������� ����� ��������� ����� ()
        while (opStack.tos() != "(")
          postfix.push_back(opStack.pop());
        opStack.pop();
        continue;
      }
      //���� �� ������� ����� ��������� �������� � ������� �����������, ��� ������� ��������� � ��������
      while (!opStack.empty() && operation.getPriority(opStack.tos()) >= operation.getPriority(lexem))
        postfix.push_back(opStack.pop());

      if (opStack.empty()) {
        opStack.push(lexem);
        continue;
      }
      else {
        if (operation.getPriority(opStack.tos()) < operation.getPriority(lexem)) {
          opStack.push(lexem);
          continue;
        }
      }
    }
  }
  while (!opStack.empty())
    postfix.push_back(opStack.pop());
}

bool TPostfix::IsNumber(const std::string& lexem)
{
  if (lexem.find_first_not_of("1234567890.") != std::string::npos)
    return false;
  return true;
}


// ����� ����������� �������, � ����� �������������� ��������
void TPostfix::Execute(HierarchyList::const_iterator* it)
{

  TStack<Variable> algArguments;
  TStack<std::string> strArguments;

  infix = Parser(**it);
  postfix.clear();
  ToPostfix();

  for (size_t i = 0; i < postfix.size(); i++)
  {
    std::string tmp = postfix[i];
    if (tmp == "program")
    {
      it->next();
      break;
    }
    else if (tmp == "const")
    {
      UpdateTable(*it);
      it->next();
      break;
    }
    else if (tmp == "var")
    {
      UpdateTable(*it);
      it->next();
      break;
    }
    else if (tmp == "begin")
    {
      it->down();
      break;
    }
    else if (tmp == "end")
    {
      it->next();
      break;
    }
    else if (tmp == "if")
    {
      if (algArguments.pop().val.i)
      {
        logicBlock.push(true);
        lastCompare = true;
        it->down();
        if (!*it) // it != nullptr ����� � ���, ��� �� ����� �������
        {
          break;
        }
        else
        {
          it->up();
          it->next();
        }
      }
      else
      {
        logicBlock.push(false);
        lastCompare = false;
        it->down();
        if (!*it) // it != nullptr ����� � ���, ��� �� ����� �������, �� ������ ���������� �.�. false
        {
          it->up();
          it->next();
        }
        else //���� begin
        {
          it->up();
          //����� ���������� begin end
          it->next();
          it->next();
          it->next();
        }
      }
    }
    else if (tmp == "else")
    {
      if (lastCompare == false)
      {
        it->down();
        if (!*it) // it != nullptr ����� � ���, ��� �� ����� ������
        {
          break; //�������� � begin
        }
        else
        {
          it->up();
          it->next();
        }
      }
      else
      {
        it->down();
        if (!*it) // it != nullptr ����� � ���, ��� �� ����� �������, �� ������ ���������� �.�. false
        {
          it->up();
          it->next();
        }
        else //���� begin
        {
          it->up();
          //����� ���������� begin end
          it->next();
          it->next();
          it->next();
        }

      }
    }
    else if (tmp == "write")
    {
      //����� �� ���������� ����� �����, ����� ����������� �������� ������ ��� variable.
      if (strArguments.empty())
      {
        if (algArguments.empty())
          throw ExitCodes::NOT_ENOUGH_ARGUMENTS_TO_CALL_THE_FUNCTION;
        std::cout << algArguments.pop();
      }
      else
      {
        std::cout << strArguments.pop();
        if (algArguments.empty())
        {
          it->next();
          break;
        }
        else
        {
          std::cout << algArguments.pop();
        }
      }
      it->next();
    }
    else if (tmp == "writeln")
    {
      //����� �� ���������� ����� �����, ����� ����������� �������� ������ ��� variable.
      if (strArguments.empty())
      {
        if (algArguments.empty())
          throw ExitCodes::NOT_ENOUGH_ARGUMENTS_TO_CALL_THE_FUNCTION;
        std::cout << algArguments.pop() << std::endl;
      }
      else
      {
        std::cout << strArguments.pop();
        if (algArguments.empty())
        {
          it->next();
          std::cout << std::endl;
          break;
        }
        else
        {
          std::cout << algArguments.pop() << std::endl;
        }
      }
      it->next();
    }
    else if (tmp == "read")
    {
      Variable argument = algArguments.pop();
      Variable* tableArg = table->Find(argument.name);
      if (tableArg == nullptr)
        throw ExitCodes::NO_SUCH_VARIABLE;
      std::cin >> argument;
      *tableArg = argument;
      it->next();
    }
    else if (tmp == ":=")
    {
      Variable right = algArguments.pop();
      Variable left = algArguments.pop();
      if (!right.isInt)
      {
        if (left.isInt)
        {
          right = Variable((int)right.val.d, right.name);
        }
      }
      right.name = left.name;
      Variable* tableArg = table->Find(left.name);
      if (tableArg == nullptr)
        throw std::string("No such variable!");
      *tableArg = right;
      it->next();
    }
    //��������� ������� ������ write
    else if (tmp[0] == '\'')
    {
      int j = 1;
      std::string str;
      while (tmp[j] != '\'')
      {
        str += tmp[j];
        j++;
      }
      strArguments.push(str);
      continue;
    }
    else if (operation.IsMathOperation(tmp))
    {
      Variable right = algArguments.pop();
      //delete
      Variable left = algArguments.pop();
      //delete
      algArguments.push(operation.Calc(tmp, left, right));
    }
    //���� ������ �� �����, �� ��� ���������� ���� �����
    else
    {
      if (IsNumber(tmp))
      {
        Variable num(tmp);
        algArguments.push(num);
      }
      //������, ����������
      else
      {
        Variable* variable = table->Find(tmp);
        if (variable == nullptr)
          throw ExitCodes::NO_SUCH_VARIABLE;
        algArguments.push(*variable);
      }
    }

  }
  //������, � ���������� ���������� ����������� ��������� ������ � ����� �����
  if (*it == nullptr)
  {
    it->up();
    if (!logicBlock.empty())
      lastCompare = logicBlock.pop();
    it->next();
  }
}

void TPostfix::UpdateTable(HierarchyList::const_iterator it)
{
  it.down();
  //������ �������� �� ��� ���, ���� �� ����� �����
  while (!it)
  {
    postfix.clear();
    infix = Parser(*it);
    ToPostfix();
    TStack<std::string> lexems;
    TStack<Variable> algArguments;
    for (size_t i = 0; i < postfix.size(); i++)
    {
      if (IsNumber(postfix[i]))
      {
        algArguments.push(Variable(postfix[i]));
        continue;
      }
      else if (postfix[i] == ":")
      {
        std::string type = lexems.pop();
        if (type == "integer")
        {
          if (algArguments.empty())
          {
            while (!lexems.empty())
            {
              std::string key = lexems.tos();
              table->changeValue(key, Variable(INT_MAX, lexems.pop()));
            }

          }
          else
          {
            Variable value(algArguments.pop());
            while (!lexems.empty())
            {
              value.name = lexems.tos();
              table->changeValue(lexems.pop(), value);
            }
          }
        }
        else if (type == "double")
        {
          if (algArguments.empty())
          {
            while (!lexems.empty())
            {
              std::string key = lexems.tos();
              table->changeValue(key, Variable(DBL_MAX, lexems.pop()));
            }

          }
          else
          {
            Variable value(algArguments.pop());
            while (!lexems.empty())
            {
              value.name = lexems.tos();
              table->changeValue(lexems.pop(), value);
            }

          }
        }
        else
          throw ExitCodes::INVALID_VARIABLE_TYPE;
      }
      else if (postfix[i] == "=")
        continue;
      else
        lexems.push(postfix[i]);
    }
    it.next();
  }
}
