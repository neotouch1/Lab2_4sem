#include "gtest/gtest.h"
#include "unsortListTable.h"

typedef UnsortListTable<std::string, int> Tab;

TEST(UnsortListTable, can_create_table)
{
  ASSERT_NO_THROW(Tab t);
}

TEST(UnsortListTable, can_add_elem_in_table)
{
  Tab t;
  ASSERT_NO_THROW(t.Insert("Hi", 5));
}

TEST(UnsortListTable, can_find_existing_elem_in_table)
{
  Tab t;
  t.Insert("Hi", 5);
  EXPECT_EQ(*t.Find("Hi"), 5);
}

TEST(UnsortListTable, cant_find_non_existing_elem_in_table)
{
  Tab t;
  EXPECT_EQ(t.Find("Hi"), nullptr);
}
