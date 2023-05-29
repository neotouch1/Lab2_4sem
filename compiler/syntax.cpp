#include "syntax.h"

std::vector<std::string> Parser(std::string str)
{
  Syntax syntax;
  std::vector<std::string> infix;

  for (int i = 0; i < str.size();)
  {
    std::string elem;
    std::string lexem;
    elem = str[i];

    if (syntax.isSyntax(elem))
    {
      lexem = (char)tolower(elem[0]);
      i++;
    }
    else
    {
      if (elem == "'" && i < str.size())
      {
        do
        {
          lexem += elem[0];
          elem = str[++i];
        } while (elem != "'" && i < str.size());
        lexem += elem[0];
        elem = str[++i];
      }
      while (!syntax.isSyntax(lexem) && !syntax.isSyntax(elem) && i < str.size())
      {
        if (elem != " ")
          lexem += (char)tolower(elem[0]);
        elem = str[++i];
      }
    }

    if (lexem == ":" && i < str.size())
      if (str[i] == '=')
        lexem += str[i++];

    if (lexem != "" && str.find("writeln") != std::string::npos && str.find("write") == str.find(lexem))
    {
      lexem = "writeln";
      i += 2;
      infix.push_back(lexem);
      continue;
    }

    if (lexem != "" && str.find("end.") != std::string::npos && str.find("end") == str.find(lexem))
    {
      lexem = "end.";
      i += 1;
      infix.push_back(lexem);
      continue;
    }

    size_t pos = std::min(lexem.find("mod"), lexem.find("div"));
    if (pos != std::string::npos)
    {
      infix.push_back(lexem.substr(0, pos));
      infix.push_back(lexem.substr(pos, 3));
      infix.push_back(lexem.substr(pos + 3));
      continue;
    }

    if ((lexem == "-") && (infix.size() == 0 || (infix.size() > 0 && infix[infix.size() - 1] == "("))) // ѕревращение унарного минуса в бинарный
      infix.push_back("0");

    if (lexem.size() != 0)
      infix.push_back(lexem);
  }

  return infix;
}
