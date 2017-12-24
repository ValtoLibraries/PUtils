#include "KeyManager.hpp"

namespace pogre
{
    namespace detail
    {
        void KeyManager::addKeyHandler(SDL_Keycode key, const KeyManager::KeyHandler &onPress,
                                       const KeyManager::KeyHandler &onRelease) noexcept
        {
            Lock _(_currentState->_keyHandlersMutex);
            _currentState->_keyHandlersToAdd[key].push_back({ onPress, onRelease });
        }

        void KeyManager::addKeyHandler(const std::string & state,
                                       SDL_Keycode key, const KeyManager::KeyHandler &onPress,
                                       const KeyManager::KeyHandler &onRelease) noexcept
        {
            Lock _(_states[state]._keyHandlersMutex);
            _states[state]._keyHandlersToAdd[key].push_back({ onPress, onRelease });
        }

        void KeyManager::addKeyHandler(const AnyKeyHandler &onPress, const AnyKeyHandler &onRelease) noexcept
        {
            Lock _(_currentState->_keyHandlersMutex);
            _currentState->_anyKeyHandlersToAdd.push_back({ onPress, onRelease });
        }

        void KeyManager::addKeyHandler(const std::string & state,
                                       const AnyKeyHandler &onPress, const AnyKeyHandler &onRelease) noexcept
        {
            Lock _(_states[state]._keyHandlersMutex);
            _states[state]._anyKeyHandlersToAdd.push_back({ onPress, onRelease });
        }

        void KeyManager::updateKeyHandlers() noexcept
        {
            Lock _(_currentState->_keyHandlersMutex);

            for (const auto &p : _currentState->_anyKeyHandlersToAdd)
                _currentState->_anyKeyHandlers.push_back(std::move(p));
            _currentState->_anyKeyHandlersToAdd.clear();

            for (const auto &category : _currentState->_keyHandlersToAdd)
            {
                for (const auto &p : category.second)
                    _currentState->_keyHandlers[category.first].push_back(std::move(p));
            }
            _currentState->_keyHandlersToAdd.clear();
        }

        void KeyManager::keyPressed(const OgreBites::KeyboardEvent &event) noexcept
        {
            updateKeyHandlers();

            for (const auto &p : _currentState->_anyKeyHandlers)
                p.first(event);

            const auto it = _currentState->_keyHandlers.find(event.keysym.sym);
            if (it != _currentState->_keyHandlers.end())
                for (const auto &p : it->second)
                    p.first();
        }

        void KeyManager::keyReleased(const OgreBites::KeyboardEvent &event) noexcept
        {
            updateKeyHandlers();

            for (const auto &p : _currentState->_anyKeyHandlers)
                p.second(event);

            const auto it = _currentState->_keyHandlers.find(event.keysym.sym);
            if (it != _currentState->_keyHandlers.end())
                for (const auto &p : it->second)
                    p.second();
        }

        void KeyManager::addMouseMovedHandler(const KeyManager::MouseMovedHandler &func) noexcept
        {
            Lock _(_currentState->_mouseHandlersMutex);
            _currentState->_mouseMoveHandlersToAdd.push_back(func);
        }

        void KeyManager::addMouseMovedHandler(const std::string & state, const KeyManager::MouseMovedHandler &func) noexcept
        {
            Lock _(_states[state]._mouseHandlersMutex);
            _states[state]._mouseMoveHandlersToAdd.push_back(func);
        }

        void KeyManager::addMouseButtonHandler(const MouseButtonHandler &onPress, const MouseButtonHandler &onRelease) noexcept
        {
            Lock _(_currentState->_mouseHandlersMutex);
            _currentState->_mouseButtonHandlersToAdd.push_back({ onPress, onRelease });
        }

        void KeyManager::addMouseButtonHandler(const std::string & state,
                                               const MouseButtonHandler &onPress, const MouseButtonHandler &onRelease) noexcept
        {
            Lock _(_states[state]._mouseHandlersMutex);
            _states[state]._mouseButtonHandlersToAdd.push_back({ onPress, onRelease });
        }

        void KeyManager::updateMouseHandlers() noexcept
        {
            Lock _(_currentState->_mouseHandlersMutex);

            for (const auto &p : _currentState->_mouseButtonHandlersToAdd)
                _currentState->_mouseButtonHandlers.push_back(std::move(p));
            _currentState->_mouseButtonHandlersToAdd.clear();

            for (const auto &p : _currentState->_mouseMoveHandlersToAdd)
                _currentState->_mouseMoveHandlers.push_back(std::move(p));
            _currentState->_mouseMoveHandlersToAdd.clear();
        }

        void KeyManager::mouseMoved(const OgreBites::MouseMotionEvent &event) noexcept
        {
            updateMouseHandlers();
            for (const auto &f : _currentState->_mouseMoveHandlers)
                f(event);
        }

        void KeyManager::mousePressed(const OgreBites::MouseButtonEvent &event) noexcept
        {
            updateMouseHandlers();
            for (const auto &p : _currentState->_mouseButtonHandlers)
                p.first(event);
        }

        void KeyManager::mouseReleased(const OgreBites::MouseButtonEvent &event) noexcept
        {
            updateMouseHandlers();
            for (const auto &p : _currentState->_mouseButtonHandlers)
                p.second(event);
        }
    }
}

