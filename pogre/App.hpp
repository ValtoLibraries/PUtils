#pragma once

#include <memory>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/OgreRoot.h>

#include "CameraManager.hpp"
#include "KeyManager.hpp"
#include "ActionManager.hpp"
#include "AnimationManager.hpp"

namespace pogre
{
    class App : public OgreBites::ApplicationContext, public OgreBites::InputListener,
                public detail::CameraManager, public detail::KeyManager,
                public detail::ActionManager, public detail::AnimationManager
    {
    public:
        template<typename String>
        App(String && name = "App")
                : OgreBites::ApplicationContext(FWD(name))
        {
            addInputListener(this);
        }
        virtual ~App() = default;

        void start()
        {
            initApp();
            getRoot()->startRendering();
            closeApp();
        }

        // Update
    protected:
        bool frameRenderingQueued(const Ogre::FrameEvent &event) override;

        // Key handling
    protected:
        bool keyPressed(const OgreBites::KeyboardEvent &event) override;
        bool keyReleased(const OgreBites::KeyboardEvent &event) override;

        // Mouse handling
    protected:
        bool mouseMoved(const OgreBites::MouseMotionEvent &event) override;
        bool mousePressed(const OgreBites::MouseButtonEvent &event) override;
        bool mouseReleased(const OgreBites::MouseButtonEvent &event) override;
   };
}