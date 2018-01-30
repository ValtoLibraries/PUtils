#pragma once

#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstddef>
#include "Point.hpp"
#include "fwd.hpp"

namespace putils {
    template<typename Contained, typename Precision = int, std::size_t MaxChildren = 32>
    class QuadTree {
    public:
        struct Obj {
            Rect <Precision> boundingBox;
            Contained obj;
        };

    public:
        QuadTree(const Rect <Precision> & boundingBox)
                : _items(), _children(), _boundingBox(boundingBox) {}

    public:
        template<typename O>
        bool add(O && obj, const Rect <Precision> & boundingBox) {
            while (!_boundingBox.intersect(boundingBox))
                enlarge(boundingBox);

            if (_items.size() < MaxChildren || _boundingBox.size.x < 2 || _boundingBox.size.y < 2) {
                _items.push_back(Obj{ boundingBox, FWD(obj) });
                return true;
            }

            if (_children.empty())
                divideIntoChildren();

            bool good = false;
            for (auto & c : _children)
                good |= c.add(FWD(obj), boundingBox);

            if (!good)
                throw std::runtime_error("Couldn't add object. This should never happen.");

            return good;
        }

    public:
        void remove(const Contained & obj) noexcept {
            auto it = findObject(_items, obj);
            while (it != _items.end()) {
                _items.erase(it);
                it = findObject(_items, obj);
            }

            for (auto & c : _children)
                c.remove(obj);
        }

    public:
        void move(const Contained & obj, const Rect <Precision> & boundingBox) {
            remove(obj);
            add(obj, boundingBox);
        }

    public:
        std::vector<Contained> query(const Rect <Precision> & area) const noexcept {
            std::vector<Contained> ret;

            if (!_boundingBox.intersect(area))
                return ret;

            for (const auto & i : _items)
                if (area.intersect(i.boundingBox))
                    ret.push_back(i.obj);

            for (const auto & c : _children)
                for (const auto & obj : c.query(area))
                    if (std::find_if(ret.begin(), ret.end(),
                                     [&obj](auto && other) { return obj == other; }) == ret.end())
                        ret.push_back(obj);

            return ret;
        }

    private:
        void enlarge(const Rect<Precision> & boundingBox) noexcept {
            std::vector<QuadTree> newChildren;
            newChildren.push_back(*this);

            _items.clear();
            _children = newChildren;

            if (boundingBox.topLeft.x < _boundingBox.topLeft.x)
                _boundingBox.topLeft.x -= _boundingBox.size.x;
            else
                _boundingBox.size.x += _boundingBox.size.x;

            if (boundingBox.topLeft.y < _boundingBox.topLeft.y)
                _boundingBox.topLeft.y -= _boundingBox.size.y;
            else
                _boundingBox.size.y += _boundingBox.size.y;

            const Point<Precision> childSize = { _boundingBox.size.x / 2, _boundingBox.size.y / 2 };
            _children.push_back(QuadTree({ { _boundingBox.topLeft.x, _boundingBox.topLeft.y + childSize.y },
                                           childSize }));
            _children.push_back(QuadTree({ { _boundingBox.topLeft.x + childSize.x, _boundingBox.topLeft.y },
                                           childSize }));
            _children.push_back(QuadTree({ { _boundingBox.topLeft.x + childSize.x,
                                             _boundingBox.topLeft.y + childSize.x }, childSize }));
        }

    private:
        void divideIntoChildren() noexcept {
            const Point<Precision> childSize = { _boundingBox.size.x / 2, _boundingBox.size.y / 2 };
            _children.push_back(QuadTree({ _boundingBox.topLeft, childSize }));
            _children.push_back(QuadTree({ { _boundingBox.topLeft.x, _boundingBox.topLeft.y + childSize.y },
                                           childSize }));
            _children.push_back(QuadTree({ { _boundingBox.topLeft.x + childSize.x, _boundingBox.topLeft.y },
                                           childSize }));
            _children.push_back(QuadTree({ { _boundingBox.topLeft.x + childSize.x,
                                             _boundingBox.topLeft.y + childSize.x }, childSize }));
        }

    private:
        template<typename Container>
        decltype(auto) findObject(Container && c, const Contained & obj) const {
            return std::find_if(c.begin(), c.end(), [obj](auto && other) { return obj == other.obj; });
        }

    private:
        std::vector<Obj> _items;
        std::vector<QuadTree> _children;
        Rect <Precision> _boundingBox;
    };
}