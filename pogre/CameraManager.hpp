#pragma once

#include <OgreInput.h>
#include "CameraMan.hpp"

namespace pogre {
    namespace detail {
        class CameraManager {
        public:
            CameraMan & addCamera(const std::string & name, CameraMan && cameraMan) noexcept {
                return _cameras[name] = std::move(cameraMan);
            }

            void removeCamera(const std::string & name) noexcept {
                _cameras.erase(name);
            }

            CameraMan & getCamera(const std::string & name) noexcept {
                return _cameras[name];
            }

        public:
            void enableAllCameras() noexcept {
                for (auto & p : _cameras)
                    p.second.enable();
            }

            void disableAllCameras() noexcept {
                for (auto & p : _cameras)
                    p.second.disable();
            }

        protected:
            void frameRendered(const Ogre::FrameEvent & event) noexcept {
                for (auto & p : _cameras)
                    p.second.frameRendered(event);
            }

            void keyPressed(const OgreBites::KeyboardEvent & event) noexcept {
                for (auto & p : _cameras)
                    p.second.keyPressed(event);
            }

            void keyReleased(const OgreBites::KeyboardEvent & event) noexcept {
                for (auto & p : _cameras)
                    p.second.keyReleased(event);
            }

            void mouseMoved(const OgreBites::MouseMotionEvent & event) noexcept {
                for (auto & p : _cameras)
                    p.second.mouseMoved(event);
            }

            void mousePressed(const OgreBites::MouseButtonEvent & event) noexcept {
                for (auto & p : _cameras)
                    p.second.mousePressed(event);
            }

            void mouseReleased(const OgreBites::MouseButtonEvent & event) noexcept {
                for (auto & p : _cameras)
                    p.second.mouseReleased(event);
            }

        private:
            std::unordered_map<std::string, CameraMan> _cameras;
        };
    }
}