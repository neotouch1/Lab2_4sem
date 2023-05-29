#include "gtest/gtest.h"
#include "list.h"

TEST(TList, can_create_list)
{
  ASSERT_NO_THROW(TList<int> l);
}

TEST(TList, can_get_size_of_list)
{
  TList<int> l;
  ASSERT_NO_THROW(l.GetSize());
}

TEST(TList, true_if_list_is_empty)
{
  TList<int> l;
  EXPECT_EQ(true, l.IsEmpty());
}

TEST(TList, can_insert_first_elem_in_list)
{
  TList<int> l;
  ASSERT_NO_THROW(l.InsertFirst(5));
}

TEST(TList, can_insert_last_elem_in_list)
{
  TList<int> l;
  ASSERT_NO_THROW(l.InsertLast(5));
}

TEST(TList, can_create_list_iterator)
{
  TList<int> l;
  ASSERT_NO_THROW(TList<int>::iterator it;);
}

TEST(TList, can_get_begin_in_list_iterator)
{
  TList<int> l;
  ASSERT_NO_THROW(l.begin());
}

TEST(TList, can_get_end_in_list_iterator)
{
  TList<int> l;
  ASSERT_NO_THROW(l.end());
}

TEST(TList, can_get_elem_in_list_iterator)
{
  TList<int> l;
  l.InsertLast(1);
  auto it = l.begin();
  EXPECT_EQ(1, *it);
}

TEST(TList, can_pre_increase_in_list_iterator)
{
  TList<int> l; l.InsertLast(1);
  auto it = l.begin();
  ASSERT_NO_THROW(++it);
}

TEST(TList, can_post_increase_in_list_iterator)
{
  TList<int> l; l.InsertLast(1);
  auto it = l.begin();
  ASSERT_NO_THROW(it++);
}

TEST(TList, can_delete_elem_in_list)
{
  TList<int> l; l.InsertLast(1);
  l.Delete(l.begin());
  EXPECT_EQ(0, l.GetSize());
}
