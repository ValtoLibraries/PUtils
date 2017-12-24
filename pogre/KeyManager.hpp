#pragma once

#include <mutex>
#include <functional>
#include <vector>
#include <unordered_map>
#include <Bites/OgreInput.h>

namespace pogre {
    namespace detail {
        class KeyManager {
        public:
            KeyManager() {
                _currentState = &_states["default"];
            }

        public:
            using KeyHandler = std::function<void()>;
            void addKeyHandler(SDL_Keycode key,
                               const KeyHandler & onPress,
                               const KeyHandler & onRelease = [] {}) noexcept;
            void addKeyHandler(const std::string & state,
                               SDL_Keycode key,
                               const KeyHandler & onPress,
                               const KeyHandler & onRelease = [] {}) noexcept;

            using AnyKeyHandler = std::function<void( const OgreBites::KeyboardEvent &)>;
            void addKeyHandler(const AnyKeyHandler & onPress, const AnyKeyHandler & onRelease = [](auto &&) {}) noexcept;
            void addKeyHandler(const std::string & state, const AnyKeyHandler & onPress, const AnyKeyHandler & onRelease = [](auto &&) {}) noexcept;

            using MouseMovedHandler = std::function<void( const OgreBites::MouseMotionEvent &)>;
            void addMouseMovedHandler(const MouseMovedHandler & func) noexcept;
            void addMouseMovedHandler(const std::string & state, const MouseMovedHandler & func) noexcept;

            using MouseButtonHandler = std::function<void( const OgreBites::MouseButtonEvent &)>;
            void addMouseButtonHandler(const MouseButtonHandler & onPress, const MouseButtonHandler & onRelease = [](auto &&) {}) noexcept;
            void addMouseButtonHandler(const std::string & state, const MouseButtonHandler & onPress, const MouseButtonHandler & onRelease = [](auto &&) {}) noexcept;

        public:
            void setState(const std::string & state) { _currentState = &_states[state]; }
            std::string getState() const {
                auto it = std::find_if(_states.begin(), _states.end(),
                                       [this](auto && p) { return &p.second == _currentState; }
                );
                return it->first;
            }


            // Key handling
        protected:
            void keyPressed(const OgreBites::KeyboardEvent & event) noexcept;
            void keyReleased(const OgreBites::KeyboardEvent & event) noexcept;

            void mouseMoved(const OgreBites::MouseMotionEvent & event) noexcept;
            void mousePressed(const OgreBites::MouseButtonEvent & event) noexcept;
            void mouseReleased(const OgreBites::MouseButtonEvent & event) noexcept;

        private:
            void updateKeyHandlers() noexcept;
            void updateMouseHandlers() noexcept;

            using Lock = std::unique_lock<std::mutex>;
            struct State {
                using KeyHandlerPair = std::pair<KeyHandler, KeyHandler>;
                std::unordered_map<SDL_Keycode, std::vector<KeyHandlerPair>> _keyHandlers;
                std::unordered_map<SDL_Keycode, std::vector<KeyHandlerPair>> _keyHandlersToAdd;

                using AnyKeyHandlerPair = std::pair<AnyKeyHandler, AnyKeyHandler>;
                std::vector<AnyKeyHandlerPair> _anyKeyHandlers;
                std::vector<AnyKeyHandlerPair> _anyKeyHandlersToAdd;

                std::mutex _keyHandlersMutex;

                std::vector<MouseMovedHandler> _mouseMoveHandlers;
                std::vector<MouseMovedHandler> _mouseMoveHandlersToAdd;

                using MouseButtonHandlerPair = std::pair<MouseButtonHandler, MouseButtonHandler>;
                std::vector<MouseButtonHandlerPair> _mouseButtonHandlers;
                std::vector<MouseButtonHandlerPair> _mouseButtonHandlersToAdd;

                std::mutex _mouseHandlersMutex;
            };

            std::unordered_map<std::string, State> _states;
            State * _currentState;
        };
    }
}