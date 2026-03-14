#include "ParametersScene.h"
#include <gf/Coordinates.h>
#include "RogueCMI.h"
#include <thread>
#include <chrono>

namespace rCMI {

    ParametersScene::ParametersScene(RogueCMI *game)
  : gf::Scene(view_size)
  , m_game(game)
  , m_helpCommandsText("", game->resources.getFont(PATH_FONT), 20)
  , m_titre("PARAMETRES", game->resources.getFont(PATH_FONT), 35)
{
    setClearColor(gf::Color::Transparent);

    gf::Vector2f center = view_size / 2.0f;

    m_titre.setColor(gf::Color::Yellow);
    m_titre.setAnchor(gf::Anchor::Center);
    m_titre.setPosition({center.x, center.y - 220.0f});

    m_helpCommandsText.setParagraphWidth(500.0f);
    helpCommandsString = "- TAB : Inventaire\n- A : Map\n- Foncez sur un ennemi pour l'attaquer\n- Clic : Tir à distance (Arc + Flèches)\n- Gain XP : Stats dans l'inventaire\n";
    m_helpCommandsText.setString(helpCommandsString);
    
    m_helpCommandsText.setAlignment(gf::Alignment::Left);
    m_helpCommandsText.setAnchor(gf::Anchor::TopLeft);
    m_helpCommandsText.setLineSpacing(1.5f);

    m_background.setSize({600.0f, 700.0f});
    m_background.setTexture(game->resources.getTexture("backgroundItem.png"));
    m_background.setAnchor(gf::Anchor::Center);
    m_background.setPosition(center);
}

    void ParametersScene::doProcessEvent(gf::Event &event)
    {
        

    }


    void ParametersScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) 
    {
        gf::Vector2f center = target.getView().getSize() / 2.0f;
        gf::Vector2f panelSize = m_background.getSize();

        m_background.setPosition(center);
        m_titre.setPosition({center.x, center.y - (panelSize.y * 0.35f)});

        float marginLeft = center.x - (panelSize.x / 2.0f) + 50.0f;
        float marginTop = center.y - (panelSize.y / 2.0f) + 150.0f;
        
        m_helpCommandsText.setPosition({marginLeft, marginTop});

        target.draw(m_background, states);
        target.draw(m_titre, states);
        target.draw(m_helpCommandsText, states);
    }

}











