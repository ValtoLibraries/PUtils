#include "gtest/gtest.h"
#include "Observable.hpp"

TEST(ObservableTest, ChangedNoParameters)
{
    bool check = false;

    putils::Observable<> o;
    o.addObserver([&check] { check = true; });
    o.changed();

    EXPECT_TRUE(check);
}

TEST(ObservableTest, ChangedOneParameter)
{
    bool check = false;

    putils::Observable<bool> o;
    o.addObserver([&check](bool p) { check = p; });

    o.changed(false);
    EXPECT_FALSE(check);

    o.changed(true);
    EXPECT_TRUE(check);
}

TEST(ObservableTest, ChangedMultipleObservers)
{
    bool check = false;
    int i = 0;

    putils::Observable<bool, int> o;
    o.addObserver([&check](bool p, int) { check = p; });
    o.addObserver([&i](bool, int p) { i = p; });

    o.changed(true, 0);
    EXPECT_TRUE(check);
    EXPECT_EQ(i, 0);

    o.changed(false, 42);
    EXPECT_FALSE(check);
    EXPECT_EQ(i, 42);

    o.changed(true, 42);
    EXPECT_TRUE(check);
    EXPECT_EQ(i, 42);
}

TEST(ObservableTest, RemoveObserver)
{
    bool check = false;

    putils::Observable<> o;
    const auto id = o.addObserver([&check] { check = !check; });
    o.changed();
    EXPECT_TRUE(check);

    o.removeObserver(id);
    o.changed();
    EXPECT_TRUE(check);

    o.addObserver([&check] { check = !check; });
    o.changed();
    EXPECT_FALSE(check);
}
