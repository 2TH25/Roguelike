#ifndef RL_END_MENU_SCENE_H
#define RL_END_MENU_SCENE_H

#include <gf/Scene.h>
#include <gf/Font.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Text.h>

namespace rCMI
{
	class RogueCMI;

	class EndMenuScene : public gf::Scene
	{
	public:
		EndMenuScene(RogueCMI *game);
		void setFinalStats(int score, int kills, int highest_level);

	private:
		void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;
		void doProcessEvent(gf::Event &event) override;
		

		RogueCMI *m_game = nullptr;
		gf::Font &font;
		gf::WidgetContainer widgets;
		gf::TextButtonWidget m_quit;
		gf::Text m_title;
    	gf::Text m_statsText;
		gf::RectangleShape m_background;
	};
}

#endif // RL_END_MENU_SCENE_H