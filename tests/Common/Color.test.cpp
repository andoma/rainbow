#include "Common/Color.h"

TEST(ColorBytesTest, Assignment)
{
	Colorb cb;
	cb = 0x12345678;
	ASSERT_EQ(0x12, cb.r);
	ASSERT_EQ(0x34, cb.g);
	ASSERT_EQ(0x56, cb.b);
	ASSERT_EQ(0x78, cb.a);
}

TEST(ColorFloatsTest, Assignment)
{
	Colorf cf;
	cf = 0x12345678;
	ASSERT_FLOAT_EQ(0x12 / 255.0f, cf.r);
	ASSERT_FLOAT_EQ(0x34 / 255.0f, cf.g);
	ASSERT_FLOAT_EQ(0x56 / 255.0f, cf.b);
	ASSERT_FLOAT_EQ(0x78 / 255.0f, cf.a);
}