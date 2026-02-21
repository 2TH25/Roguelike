#ifndef RL_VICTORY_SCENE_H
#define RL_VICTORY_SCENE_H

#include <gf/Scene.h>
#include <gf/Font.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Text.h>

namespace rCMI
{
	class RogueCMI;

	class VictoryScene : public gf::Scene
	{
	public:
		VictoryScene(RogueCMI *game);
		void setFinalScore(int score);

	private:
		void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;
		void doProcessEvent(gf::Event &event) override;

		RogueCMI *m_game = nullptr;
		gf::Font &font;
		gf::WidgetContainer widgets;
		gf::TextButtonWidget m_quit;
		gf::TextButtonWidget m_continue;
		gf::Text m_title;
    	gf::Text m_messageText;
        gf::Text m_scoreText;
		gf::RectangleShape m_background;
	};
}

#endif // RL_VICTORY_SCENE_H