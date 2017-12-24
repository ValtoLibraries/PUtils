#pragma once

#ifdef POGRE_CEGUI

#include "App.hpp"
#include "CEGUIUtils.hpp"

namespace pogre
{
    class CEGUIApp : public App
    {
    public:
        CEGUIApp(const std::string & name = "App");

    public:
        CEGUI::GUIContext &getGuiContext() noexcept { return *_guiContext; }

    private:
        void init() noexcept;

    protected:
        bool keyPressed(const OgreBites::KeyboardEvent &event) override
        {
            if (_guiContext)
            {
                if (_guiContext->injectKeyDown(SDL_ScanCodeToCEGUIKey(event.keysym.scancode)))
                {
                    _guiContext->injectChar(event.keysym.sym);
                    return true;
                }
                if (_guiContext->injectChar(event.keysym.sym))
                    return true;
            }
            return App::keyPressed(event);
        }

        bool keyReleased(const OgreBites::KeyboardEvent &event) override
        {
            if (_guiContext && _guiContext->injectKeyUp(pogre::SDL_ScanCodeToCEGUIKey(event.keysym.scancode)))
                return true;
            return App::keyReleased(event);
        }

    protected:
        bool mouseMoved(const OgreBites::MouseMotionEvent &event) override
        {
            if (_guiContext && _guiContext->getMouseCursor().isVisible() && _guiContext->injectMouseMove(event.xrel, event.yrel))
                return true;
            return App::mouseMoved(event);
        }

        bool mousePressed(const OgreBites::MouseButtonEvent &event) override
        {
            if (_guiContext && _guiContext->getMouseCursor().isVisible() && _guiContext->injectMouseButtonDown(pogre::SDL_toMouseButton(event.button)))
                return true;
            return App::mousePressed(event);
        }

        bool mouseReleased(const OgreBites::MouseButtonEvent &event) override
        {
            if (_guiContext && _guiContext->getMouseCursor().isVisible() && _guiContext->injectMouseButtonUp(pogre::SDL_toMouseButton(event.button)))
                return true;
            return App::mouseReleased(event);
        }

    protected:
        void windowResized(Ogre::RenderWindow *win) override;

    private:
        CEGUI::GUIContext *_guiContext = nullptr;
    };
}

#endif
