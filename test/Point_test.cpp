#include "gtest/gtest.h"
#include "Point.hpp"

TEST(PointTest, Equality)
{
    putils::Point2i p{ 42, 42 };
    putils::Point2i p2{ 42, 42 };
    EXPECT_EQ(p, p2);
}

TEST(PointTest, Assignment)
{
    putils::Point2i p{ 42, 42 };
    putils::Point2i p2{ 20, 20 };
    p = p2;
    EXPECT_EQ(p, p2);
}

TEST(PointTest, Difference)
{
    putils::Point2i p{ 42, 42 };
    putils::Point2i p2{ 20, 20 };
    EXPECT_NE(p, p2);
}

TEST(PointTest, Addition)
{
    putils::Point2i p{ 42, 42 };
    p += putils::Point2i{ 5, 5 };
    putils::Point2i res{ 47, 47 };
    EXPECT_EQ(p, res);

    p = putils::Point2i{ 42, 42 } + putils::Point2i{ 5, 5 };
    EXPECT_EQ(p, res);
}

TEST(PointTest, Substraction)
{
    putils::Point2i p{ 42, 42 };
    p -= putils::Point2i{ 5, 5 };
    putils::Point2i res{ 37, 37 };
    EXPECT_EQ(p, res);

    p = putils::Point2i{ 42, 42 } - putils::Point2i{ 5, 5 };
    EXPECT_EQ(p, res);
}

TEST(PointTest, DistanceTo)
{
    putils::Point2i p{ 0, 0 };
    putils::Point2i right{ 5, 0 };
    putils::Point2i left{ -5, 0 };
    putils::Point2i top{ 0, 5 };
    putils::Point2i bottom{ 0, -5 };

    EXPECT_EQ(p.distanceTo(right), 5);
    EXPECT_EQ(p.distanceTo(left), 5);
    EXPECT_EQ(p.distanceTo(top), 5);
    EXPECT_EQ(p.distanceTo(bottom), 5);
}

TEST(PointTest, AngleTo)
{
    putils::Point2i p{ 0, 0 };
    putils::Point2i right{ 5, 0 };
    putils::Point2i left{ -5, 0 };
    putils::Point2i bottom{ 0, 5 };
    putils::Point2i top{ 0, -5 };

    EXPECT_EQ(p.angleTo(right), 0);
    EXPECT_EQ(p.angleTo(left), M_PI);
    EXPECT_EQ(p.angleTo(top), M_PI_2);
    EXPECT_EQ(p.angleTo(bottom), -M_PI_2);
}

TEST(PointTest, AngleTo3dXY)
{
    putils::Point3i p{ 0, 0 };
    putils::Point3i right{ 5, 0 };
    putils::Point3i left{ -5, 0 };
    putils::Point3i bottom{ 0, 5 };
    putils::Point3i top{ 0, -5 };

    EXPECT_EQ(p.angleToXY(right), 0);
    EXPECT_EQ(p.angleToXY(left), M_PI);
    EXPECT_EQ(p.angleToXY(top), M_PI_2);
    EXPECT_EQ(p.angleToXY(bottom), -M_PI_2);
}

TEST(PointTest, AngleTo3dXZ)
{
    putils::Point3i p{ 0, 0 };
    putils::Point3i right{ 5, 0 };
    putils::Point3i left{ -5, 0 };
    putils::Point3i bottom{ 0, 0, 5 };
    putils::Point3i top{ 0, 0, -5 };

    EXPECT_EQ(p.angleToXZ(right), 0);
    EXPECT_TRUE(p.angleToXZ(left) == M_PI || p.angleToXZ(left) == -M_PI);
    EXPECT_EQ(p.angleToXZ(top), M_PI_2);
    EXPECT_EQ(p.angleToXZ(bottom), -M_PI_2);
}

TEST(RectTest, Equality)
{
    putils::Rect2i r{ { 0, 0 }, { 1, 1} };
    putils::Rect2i r2{ { 0, 0 }, { 1, 1 } };
    EXPECT_EQ(r, r2);
}

TEST(RectTest, Difference)
{
    putils::Rect2i r{ { 0, 0 }, { 1, 1} };
    putils::Rect2i r2{ { 0, 0 }, { 1, 2 } };
    EXPECT_NE(r, r2);
}

TEST(RectTest, IntersectInside)
{
    putils::Rect2i r{ { 0, 0 }, { 5, 5 } };
    putils::Rect2i r2{ { 0, 0 }, { 1, 1 } };
    EXPECT_TRUE(r.intersect(r2));
}

TEST(RectTest, IntersectOutside)
{
    putils::Rect2i r{ { 0, 0 }, { 5, 5 } };
    putils::Rect2i r2{ { -1, -1 }, { 1, 1 } };
    EXPECT_FALSE(r.intersect(r2));

    r2 = putils::Rect2i{ { 5, 5 }, { 1, 1 } };
    EXPECT_FALSE(r.intersect(r2));
}

TEST(RectTest, IntersectInclusiveBorders)
{
    putils::Rect2i r{ { 0, 0 }, { 5, 5 } };
    putils::Rect2i r2{ { -1, -1 }, { 1, 1 } };
    EXPECT_TRUE(r.intersect(r2, true));

    r2 = putils::Rect2i{ { 5, 5 }, { 1, 1 } };
    EXPECT_TRUE(r.intersect(r2, true));
}

TEST(RectTest, Contains)
{
    putils::Rect2i r{ { 0, 0 }, { 5, 5 } };
    putils::Point2i p{ 1, 1 };
    EXPECT_TRUE(r.contains(p));
}

TEST(RectTest, ContainsBorders)
{
    putils::Rect2i r{ { 0, 0 }, { 5, 5 } };
    putils::Point2i p{ 0, 0 };
    EXPECT_TRUE(r.contains(p));
    p = { 5, 5 };
    EXPECT_FALSE(r.contains(p));
}

TEST(RectTest, ContainsBad)
{
    putils::Rect2i r{ { 0, 0 }, { 5, 5 } };
    putils::Point2i p{ -1, -1 };
    EXPECT_FALSE(r.contains(p));
}
