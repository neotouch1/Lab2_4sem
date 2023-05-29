#pragma once
#ifndef __STACK_H__
#define __STACK_H__

const int MaxStackSize = 100;

template <class TType>
class TStack
{
private:
  TType* pMem;
  int Size;
  int curSize;
  int top;
  TStack(const TStack&) = delete;
  void operator=(const TStack&) = delete; // ������ �� ������������ �����

public:
  TStack(int _size = MaxStackSize) // ����������� �� ���������
  {
    Size = _size;
    curSize = 0;
    top = -1;
    if ((Size < 1) || (Size > MaxStackSize))
      throw - 1;
    pMem = new TType[Size];
  }

  ~TStack() // ����������
  {
    delete[] pMem;
  }

  int size() { return curSize; }

  bool empty() { return top == -1; } // �������� ������ �� ����

  bool full() { return top == Size - 1; } // �������� �������� �� ����

  void push(const TType obj) // ������� �������� � ������� �����
  {
    if (full())
      throw - 1;
    pMem[++top] = obj;
    ++curSize;
  }

  TType pop() // ��������� �������� �������� �������� � ���������
  {
    if (empty())
      throw - 1;
    --curSize;
    return pMem[top--];
  }

  TType tos() // ��������� �������� �������� �������� ��� �������� // Top Of Stack
  {
    if (empty())
      throw - 1;
    return pMem[top];
  }

};

#endif