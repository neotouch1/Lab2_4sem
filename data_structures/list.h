#pragma once
template <class T>
class TList {
private:
  struct Node {
    T data{};
    Node* next = nullptr;
  };

  Node* pFirst = nullptr;
  Node** pLast = &pFirst;
  int Size = 0;

public:
  class iterator; class const_iterator;

  TList() {};

  TList(const TList<T>& l) {
    for (auto it = l.cbegin(); it != l.cend(); ++it)
      InsertLast(*it);
  }

  TList<T>& operator=(const TList<T>& l) {
    if (this == &l)
      return *this;
    Node* i = pFirst, * p;
    while (i != nullptr) {
      p = i;
      i = i->next;
      delete p;
    }
    Size = 0; pFirst = nullptr; pLast = &pFirst;
    for (auto it = l.cbegin(); it != l.cend(); ++it)
      InsertLast(*it);
    return *this;
  }

  int GetSize() const { return Size; }

  bool IsEmpty() const { return Size == 0; }

  void InsertFirst(T val) {
    Node* p = new Node{ val, pFirst };
    if (pFirst == nullptr)
      pLast = &(p->next);
    pFirst = p;
    ++Size;
  }

  void InsertLast(T val) {
    Node* p = new Node{ val, nullptr };
    *pLast = p;
    pLast = &(p->next);
    ++Size;
  }

  void Delete(const iterator it) {
    if (it == end())
      return;
    Node** p = &pFirst, * i;
    while (iterator(*p) != it && *p != nullptr)
      p = &((*p)->next);
    if (*p == nullptr)
      return;
    i = *p;
    *p = (*p)->next;
    if (*p == nullptr)
      pLast = p;
    delete i;
    --Size;
  }

  virtual ~TList() {
    Node* i = pFirst, * p;
    while (i != nullptr) {
      p = i;
      i = i->next;
      delete p;
    }
  }

  iterator begin() { return iterator(pFirst); }
  iterator end() { return iterator(*pLast); }
  const_iterator cbegin() const { return const_iterator(pFirst); }
  const_iterator cend() const { return const_iterator(*pLast); }

  class iterator {
  private:
    Node* cur;

  public:

    iterator() :cur(nullptr) {};

    iterator(Node* i) : cur(i) {};

    iterator(const iterator& i) { cur = i.cur; }

    iterator& operator=(const iterator& i) {
      cur = i.cur;
      return *this;
    }

    T& operator*() const { return cur->data; }

    T* operator->() const { return &(cur->data); }

    bool operator!=(const iterator& i) const { return cur != i.cur; }

    bool operator==(const iterator& i) const { return cur == i.cur; }

    iterator& operator++() {
      cur = cur->next;
      return *this;
    }

    iterator operator++(int) {
      iterator tmp(*this);
      cur = cur->next;
      return tmp;
    }
  };

  class const_iterator {
  private:
    Node* cur;

  public:

    const_iterator() :cur(nullptr) {};

    const_iterator(Node* i) : cur(i) {};

    const_iterator(const const_iterator& i) { cur = i.cur; }

    const_iterator& operator=(const const_iterator& i) {
      cur = i.cur;
      return *this;
    }

    const T& operator*() const { return cur->data; }

    T* operator->() const { return &(cur->data); }

    bool operator!=(const const_iterator& i) const { return cur != i.cur; }

    bool operator==(const const_iterator& i) const { return cur == i.cur; }

    const_iterator& operator++() {
      cur = cur->next;
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp(*this);
      cur = cur->next;
      return tmp;
    }
  };

};
