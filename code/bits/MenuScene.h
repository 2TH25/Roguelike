#ifndef RL_MENU_SCENE_H
#define RL_MENU_SCENE_H

#include <gf/Scene.h>
#include <gf/Font.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>

namespace rCMI
{
	class RogueCMI;

	class MenuScene : public gf::Scene
	{
	public:
		MenuScene(RogueCMI *game);

	private:
		void doHandleActions(gf::Window &window) override;
		void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
		void doProcessEvent(gf::Event& event) override;

		RogueCMI *m_game = nullptr;
		gf::Action up;
		gf::Action down;
		gf::Action select;
		gf::Font font;
		gf::WidgetContainer widgets;
		gf::TextButtonWidget m_saves;
		gf::TextButtonWidget m_quit;
	};
}

#endif // RL_MENU_SCENE_H