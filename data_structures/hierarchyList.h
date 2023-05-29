#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Stack.h"

class HierarchyList
{
private:
  struct Node {
    std::string data;
    Node* next[2];
    Node(std::string x) :data(x) { next[0] = next[1] = nullptr; }
  };
  Node* root = nullptr;

  void DeleteSubTree(Node* p) {
    if (p == nullptr)
      return;
    DeleteSubTree(p->next[0]);
    DeleteSubTree(p->next[1]);
    delete p;
    return;
  }
public:
  class iterator; class const_iterator;

  HierarchyList() {}

  void Build(std::string path) {
    std::ifstream file;
    file.open(path);
    if (!file.is_open())
      throw - 1;
    file >> *this;
    file.close();
  }

  friend std::istream& operator>>(std::istream& is, HierarchyList& l) {
    l.DeleteSubTree(l.root);
    std::size_t cur_layer, layer;
    std::string s;
    int line = 1;

    if (is.peek() != EOF) {
      std::getline(is, s);
      if (s.find_first_not_of(" ") != 0)
        throw line;
      l.root = new Node(s);
    }
    cur_layer = 0; ++line;
    auto it = l.begin();

    while (is.peek() != EOF) {
      std::getline(is, s);
      if ((layer = s.find_first_not_of(" \n")) != std::string::npos)
        if (layer == cur_layer) {
          it.Next(0) = new Node(s.substr(layer));
          it.next();
        }
        else if (layer == cur_layer + 2) {
          it.Next(1) = new Node(s.substr(layer));
          it.down();
          cur_layer = layer;
        }
        else if (layer < cur_layer && (cur_layer - layer) % 2 == 0) {
          for (int i = 0; i < (cur_layer - layer) / 2; ++i)
            it.up();
          it.Next(0) = new Node(s.substr(layer));
          it.next();
          cur_layer = layer;
        }
        else
          throw line;
      ++line;
    }
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const HierarchyList& l) {
    auto it = l.cbegin();
    while (true) {
      if (!it) {
        for (int i = 0; i < it.layer(); ++i)
          std::cout << "| ";
        std::cout << *it << std::endl;
        it.down();
      }
      else if (it.empty()) break;
      else {
        it.up();
        it.next();
      }
    }
    return os;
  }

  ~HierarchyList() { DeleteSubTree(root); }

  iterator begin() { return iterator(root); }
  const_iterator cbegin() const { return const_iterator(root); }

  class iterator {
    TStack<Node*>* st;
    Node* cur;

  public:
    Node*& Next(int i) { return cur->next[i]; }
    friend class HierarchyList;

    iterator() : st(nullptr), cur(nullptr) {}
    iterator(Node* x) : cur(x) { st = new TStack<Node*>; }
    iterator(const HierarchyList::iterator& it) :cur(it.cur) { st = new TStack<Node*>; }
    iterator& operator=(const iterator& i) {
      cur = i.cur;
      delete st;
      st = new TStack<Node*>;
      return *this;
    }
    void next() { cur = cur->next[0]; }
    void down() { st->push(cur); cur = cur->next[1]; }
    void up() { cur = st->tos(); st->pop(); }
    bool operator!() const { return cur != nullptr; }
    bool end() const { return cur == nullptr && st->empty(); }
    std::string& operator*() { return cur->data; }
    std::string* operator->() { return &(cur->data); }

    ~iterator() { delete st; }
  };

  class const_iterator {
    TStack<Node*>* st;
    Node* cur = nullptr;

  public:
    const_iterator() : st(nullptr), cur(nullptr) {}
    const_iterator(Node* x) : cur(x) { st = new TStack<Node*>; }
    const_iterator(const HierarchyList::const_iterator& it) :cur(it.cur) { st = new TStack<Node*>; }
    const_iterator& operator=(const const_iterator& i) {
      cur = i.cur;
      delete st;
      st = new TStack<Node*>;
      return *this;
    }
    void next() { cur = cur->next[0]; }
    void down() { st->push(cur); cur = cur->next[1]; }
    void up() { cur = st->tos(); st->pop(); }
    bool operator!() const { return cur != nullptr; }
    bool end() const { return cur == nullptr && st->empty(); }
    const std::string& operator*() const { return cur->data; }
    std::string* operator->() const { return &(cur->data); }
    int layer() const { return st->size(); }
    bool empty() const { return st->empty(); }
    friend bool operator==(const const_iterator& it, void* p) { return it.cur == p; }

    ~const_iterator() { delete st; }
  };
};
