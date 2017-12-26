#ifdef POGRE_CEGUI

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/System.h>
#include <CEGUI/MouseCursor.h>

#include "CEGUIApp.hpp"

namespace pogre
{
    CEGUIApp::CEGUIApp(const std::string & name) : App(name)
    {
        addAction([this] { init(); });
    }

    void CEGUIApp::init() noexcept
    {
        CEGUI::OgreRenderer::bootstrapSystem(*getRenderWindow());
        CEGUI::ImageManager::setImagesetDefaultResourceGroup("CEGUI");
        CEGUI::Font::setDefaultResourceGroup("CEGUI");
        CEGUI::Scheme::setDefaultResourceGroup("CEGUI");
        CEGUI::WidgetLookManager::setDefaultResourceGroup("CEGUI");
        CEGUI::WindowManager::setDefaultResourceGroup("CEGUI");

        _guiContext = &CEGUI::System::getSingleton().getDefaultGUIContext();

        addPermanentAction([this, &system = CEGUI::System::getSingleton()](const Ogre::FrameEvent &event)
                           {
                               system.injectTimePulse(event.timeSinceLastFrame);
                               _guiContext->injectTimePulse(event.timeSinceLastFrame);
                           }
        );
    }

    void CEGUIApp::windowResized(Ogre::RenderWindow *win)
    {
        auto s = CEGUI::System::getSingletonPtr();
        if (!s)
            return;

        auto renderer = s->getRenderer();
        renderer->setDisplaySize(
                { (float)win->getWidth(), (float)win->getHeight() }
        );

        App::windowResized(win);
    }
}

#endif