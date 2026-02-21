#include "ParametersScene.h"
#include <gf/Coordinates.h>
#include "RogueCMI.h"
#include <thread>
#include <chrono>

namespace rCMI {

    ParametersScene::ParametersScene(RogueCMI *game)
  : gf::Scene(view_size)
  , m_game(game)
  , m_helpCommandsText("", game->resources.getFont(PATH_FONT), 30)
  , m_titre("PARAMETRES", game->resources.getFont(PATH_FONT), 50)
    {
        setClearColor(gf::Color::Transparent);

        m_titre.setColor(gf::Color::Yellow);
        m_titre.setAnchor(gf::Anchor::Center);

        m_helpCommandsText.setParagraphWidth(450.0f);
        helpCommandsString = "- TAB : Inventaire\n- A : Map\n- Foncez sur un ennemi pour l'attaquer au corps à corps\n- Cliquez sur un ennemi pour lui tirer dessus à distance (nécessite un arc et des flèches)";
        m_helpCommandsText.setString(helpCommandsString);
        m_helpCommandsText.setAlignment(gf::Alignment::Center);
        m_helpCommandsText.setAnchor(gf::Anchor::Center);
        gf::Vector2f center = view_size / 2.0f;
        m_helpCommandsText.setPosition(center);

        m_background.setSize({500.0f, 700.0f});
        m_background.setTexture(game->resources.getTexture("backgroundItem.png"));
        m_background.setAnchor(gf::Anchor::Center);
        m_background.setPosition(center);
        m_background.setOutlineColor(gf::Color::Black);
        m_background.setOutlineThickness(2.0f);
        
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

        m_helpCommandsText.setPosition(center);

        target.draw(m_background, states);
        target.draw(m_titre, states);
        target.draw(m_helpCommandsText, states);
    }

}











