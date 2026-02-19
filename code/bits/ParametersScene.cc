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
{
    setClearColor(gf::Color::Transparent);

    m_helpCommandsText.setParagraphWidth(450.0f);
    helpCommandsString = "Parameters\n\n- TAB : Inventory\n- A : Map";
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


    void ParametersScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
        gf::Vector2f center = target.getView().getSize() / 2.0f;

        m_background.setPosition(center);
        m_helpCommandsText.setPosition(center);

        target.draw(m_background, states);
        target.draw(m_helpCommandsText, states);
    }

}











