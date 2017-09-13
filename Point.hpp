#pragma once

#include <initializer_list>
#include <cmath>
#include "hash.hpp"
#include "reflection/Serializable.hpp"

namespace putils
{
    template<typename Precision, std::size_t Dimensions = 2>
    struct Point : putils::Reflectible<Point<Precision, Dimensions>>,
                   public Serializable<Point<Precision, Dimensions>>
    {
        Precision x;
        Precision y;

        Point(Precision x = 0, Precision y = 0)
                : x(x), y(y)
        {}

        Point(const Point &) noexcept = default;
        Point &operator=(const Point &) noexcept = default;

        Point(Point &&) noexcept = default;
        Point &operator=(Point &&) noexcept = default;

        template<typename P>
        Point(const Point<P, 3> &other) : x(other.x), y(other.y) {}

        template<typename P>
        bool operator==(const Point<P> &rhs) const noexcept { return x == rhs.x && y == rhs.y; }

        template<typename P>
        bool operator!=(const Point<P> &rhs) const noexcept { return !(*this == rhs); }

        template<typename P>
        Point operator+(const Point<P> &rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }

        template<typename P>
        Point &operator+=(const Point<P> &rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }

        template<typename P>
        Point operator-(const Point<P> &rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }

        template<typename P>
        Point &operator-=(const Point<P> &rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }

        template<typename P>
        Precision distanceTo(const Point<P> &rhs) const noexcept
        {
            return std::sqrt(
                    std::pow(x - rhs.x, 2) +
                    std::pow(y - rhs.y, 2)
            );
        }

        double angleTo(const Point<Precision, 3> &rhs) const noexcept
        {
            return -std::atan2(rhs.y - y, rhs.x - x) + M_PI / 2;
        }

        static const auto get_class_name() { return "Point2"; }

        static const auto &get_attributes()
        {
            static const auto table = pmeta::make_table(
                    pmeta_reflectible_attribute(&Point::x),
                    pmeta_reflectible_attribute(&Point::y)
            );
            return table;
        }

        static const auto &get_methods()
        {
            static const auto table = pmeta::make_table(
                    pmeta_nameof(distanceTo), &Point::distanceTo<Precision>,
                    pmeta_reflectible_attribute(&Point::angleTo)
            );
            return table;
        }

        static const auto &get_parents()
        {
            static const auto table = pmeta::make_table();
            return table;
        }
    };

    template<typename Precision, std::size_t Dimensions = 2>
    struct Rect : public Reflectible<Rect<Precision, Dimensions>>,
                  public Serializable<Rect<Precision, Dimensions>>
    {
        Point<Precision> topLeft;
        Point<Precision> size;

        Rect(Point<Precision, Dimensions> topLeft = {}, Point<Precision, Dimensions> size = {})
                : topLeft(topLeft), size(size)
        {}

        template<typename P>
        bool operator==(const Rect<P> &rhs) const { return topLeft == rhs.topLeft && size == rhs.size; }

        template<typename P>
        bool operator!=(const Rect<P> &rhs) const { return !(*this == rhs); }

        bool intersect(const Rect &other, bool inclusiveBorders = false) const
        {
            if (inclusiveBorders)
                return !(topLeft.x > other.topLeft.x + other.size.x ||
                         topLeft.x + size.x < other.topLeft.x ||
                         topLeft.y > other.topLeft.y + other.size.y ||
                         topLeft.y + size.y < other.topLeft.y
                );

            return !(topLeft.x >= other.topLeft.x + other.size.x ||
                     topLeft.x + size.x < other.topLeft.x ||
                     topLeft.y >= other.topLeft.y + other.size.y ||
                     topLeft.y + size.y < other.topLeft.y
            );
        }

        bool contains(const Point<Precision, 2> &point) const
        {
            return (topLeft.x <= point.x &&
                    topLeft.x + size.x > point.x &&
                    topLeft.y <= point.y &&
                    topLeft.y + size.y > point.y
            );
        }

        static const auto get_class_name() { return "Rect2"; }

        static const auto &get_attributes()
        {
            static const auto table = pmeta::make_table(
                    pmeta_reflectible_attribute(&Rect::topLeft),
                    pmeta_reflectible_attribute(&Rect::size)
            );
            return table;
        }

        static const auto &get_methods()
        {
            static const auto table = pmeta::make_table(
                    pmeta_reflectible_attribute(&Rect::intersect),
                    pmeta_reflectible_attribute(&Rect::contains)
            );
            return table;
        }

        static const auto &get_parents()
        {
            static const auto table = pmeta::make_table();
            return table;
        }
    };

    template<typename Precision>
    struct Point<Precision, 3> : public Reflectible<Point<Precision, 3>>,
                                 public Serializable<Point<Precision, 3>>
    {
        Precision x;
        Precision y;
        Precision z;

        Point(Precision x = 0, Precision y = 0, Precision z = 0)
                : x(x), y(y), z(z)
        {}

        Point(const Point &) noexcept = default;
        Point &operator=(const Point &) noexcept = default;

        Point(Point &&) noexcept = default;
        Point &operator=(Point &&) noexcept = default;

        template<typename P>
        Point(const Point<P, 2> &other) : x(other.x), y(other.y), z(0) {}

        template<typename P>
        bool operator==(const Point<P, 3> &rhs) const noexcept
        { return x == rhs.x && y == rhs.y && z == rhs.z; }

        template<typename P>
        bool operator!=(const Point<P, 3> &rhs) const noexcept { return !(*this == rhs); }

        Point operator-() const noexcept { return { -x, -y, -z }; }

        template<typename P>
        Point operator+(const Point<P, 3> &rhs) const noexcept { return { x + rhs.x, y + rhs.y, z + rhs.z }; }

        template<typename P>
        Point &operator+=(const Point<P, 3> &rhs) noexcept { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }

        template<typename P>
        Point operator-(const Point<P, 3> &rhs) const noexcept { return { x - rhs.x, y - rhs.y, z - rhs.z }; }

        template<typename P>
        Point &operator-=(const Point<P, 3> &rhs) noexcept { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }

        template<typename P>
        Precision distanceTo(const Point<P, 3> &rhs) const noexcept
        {
            return std::sqrt(
                    std::pow(x - rhs.x, 2) +
                    std::pow(y - rhs.y, 2) +
                    std::pow(z - rhs.z, 2)
            );
        }

        double angleToXY(const Point<Precision, 3> &rhs) const noexcept
        {
            return std::atan2(rhs.y - y, rhs.x - x);
        }

        double angleToXZ(const Point<Precision, 3> &rhs) const noexcept
        {
            return -std::atan2(rhs.z - z, rhs.x - x) + M_PI / 2;
        }

        static const auto get_class_name() { return "Point3"; }

        static const auto &get_attributes()
        {
            static const auto table = pmeta::make_table(
                    pmeta_reflectible_attribute(&Point::x),
                    pmeta_reflectible_attribute(&Point::y),
                    pmeta_reflectible_attribute(&Point::z)
            );
            return table;
        }

        static const auto &get_methods()
        {
            static const auto table = pmeta::make_table(
                    pmeta_nameof(distanceTo), &Point::distanceTo<Precision>,
                    pmeta_reflectible_attribute(&Point::angleToXY),
                    pmeta_reflectible_attribute(&Point::angleToXZ)
            );
            return table;
        }

        static const auto &get_parents()
        {
            static const auto table = pmeta::make_table();
            return table;
        }
    };

    template<typename Precision>
    struct Rect<Precision, 3> : public Reflectible<Rect<Precision, 3>>,
                                public Serializable<Rect<Precision, 3>>
    {
        Point<Precision, 3> topLeft;
        Point<Precision, 3> size;

        Rect(Point<Precision, 3> topLeft = {}, Point<Precision, 3> size = {})
                : topLeft(topLeft), size(size)
        {}

        template<typename P>
        bool operator==(const Rect<P> &rhs) { return topLeft == rhs.topLeft && size == rhs.size; }

        template<typename P>
        bool operator!=(const Rect<P> &rhs) { return !(*this == rhs); }

        bool intersect(const Rect &other, bool inclusiveBorders = false) const
        {
            if (inclusiveBorders)
                return !(topLeft.x > other.topLeft.x + other.size.x ||
                         topLeft.x + size.x < other.topLeft.x ||
                         topLeft.y > other.topLeft.y + other.size.y ||
                         topLeft.y + size.y < other.topLeft.y ||
                         topLeft.z > other.topLeft.z + other.size.z ||
                         topLeft.z + size.z < other.topLeft.z
                );

            return !(topLeft.x >= other.topLeft.x + other.size.x ||
                     topLeft.x + size.x <= other.topLeft.x ||
                     topLeft.y >= other.topLeft.y + other.size.y ||
                     topLeft.y + size.y <= other.topLeft.y ||
                     topLeft.z >= other.topLeft.z + other.size.z ||
                     topLeft.z + size.z <= other.topLeft.z
            );
        }

        bool contains(const Point<Precision, 3> &point) const
        {
            return (topLeft.x <= point.x &&
                    topLeft.x + size.x > point.x &&
                    topLeft.y <= point.y &&
                    topLeft.y + size.y > point.y &&
                    topLeft.z <= point.z &&
                    topLeft.z + size.z > point.z
            );
        }

        static const auto get_class_name() { return "Rect3"; }

        static const auto &get_attributes()
        {
            static const auto table = pmeta::make_table(
                    pmeta_reflectible_attribute(&Rect::topLeft),
                    pmeta_reflectible_attribute(&Rect::size)
            );
            return table;
        }

        static const auto &get_methods()
        {
            static const auto table = pmeta::make_table(
                    pmeta_reflectible_attribute(&Rect::intersect),
                    pmeta_reflectible_attribute(&Rect::contains)
            );
            return table;
        }

        static const auto &get_parents()
        {
            static const auto table = pmeta::make_table();
            return table;
        }
    };

    using Point3d = Point<double, 3>;
    using Rect3d = Rect<double, 3>;
    using Point2d = Point<double, 2>;
    using Rect2d = Rect<double, 2>;

    using Point3i = Point<int, 3>;
    using Rect3i = Rect<int, 3>;
    using Point2i = Point<int, 2>;
    using Rect2i = Rect<int, 2>;

    using Point3f = Point<float, 3>;
    using Rect3f = Rect<float, 3>;
    using Point2f = Point<float, 2>;
    using Rect2f = Rect<float, 2>;
}

namespace std
{
    template<typename Precision>
    struct hash<putils::Point<Precision>>
    {
        size_t operator()(const putils::Point<Precision> &coord) const noexcept
        {
            return putils::PairHash().operator()(std::make_pair(coord.x, coord.y));
        }
    };
}

