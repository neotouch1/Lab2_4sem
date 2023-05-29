#include "gtest/gtest.h"
#include "hierarchyList.h"

TEST(HierarchyList, can_create_list)
{
  ASSERT_NO_THROW(HierarchyList l);
}

TEST(HierarchyList, can_build_correct_list)
{
  HierarchyList l;
  ASSERT_NO_THROW(l.Build("correct.txt"));
}

TEST(HierarchyList, cant_build_wrong_list)
{
  HierarchyList l;
  ASSERT_ANY_THROW(l.Build("wrong.txt"));
}

TEST(HierarchyList_iterator, can_get_list_iterator)
{
  HierarchyList l;
  ASSERT_NO_THROW(l.begin());
  ASSERT_NO_THROW(l.cbegin());
}

TEST(HierarchyList, can_build_list_is_correct)
{
  HierarchyList l;
  l.Build("correct.txt");
  EXPECT_EQ("begin", *l.cbegin());
}

TEST(HierarchyList_iterator, can_go_next_list_elem)
{
  HierarchyList l;
  l.Build("correct.txt");
  auto it = l.cbegin();
  it.next();
  EXPECT_EQ(*it, "end.");
}

TEST(HierarchyList_iterator, can_go_down_list_elem)
{
  HierarchyList l;
  l.Build("correct.txt");
  auto it = l.cbegin();
  it.down();
  EXPECT_EQ(*it, "down");
}

TEST(HierarchyList_iterator, can_go_up_list_elem)
{
  HierarchyList l;
  l.Build("correct.txt");
  auto it = l.cbegin();
  it.down();
  it.up();
  EXPECT_EQ(*it, "begin");
}
