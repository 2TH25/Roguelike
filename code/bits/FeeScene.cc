#include "FeeScene.h"
#include "RogueCMI.h"
#include "GameData.h"
#include "WorldEntity.h"

namespace rCMI {

    FeeScene::FeeScene(RogueCMI *game)
    : gf::Scene(gf::Vector2f(600, 400)) 
    , m_game(game)
    , m_currentLine(0)
    {
        setClearColor(gf::Color::Transparent); 

        m_box.setSize({960.0f, 180.0f});
        m_box.setColor(gf::Color::fromRgba32(0x0A0A0FDD));
        m_box.setOutlineColor(gf::Color::fromRgba32(0x8C6118FF)); // bordure dorée
        m_box.setOutlineThickness(2.5f);
        m_box.setAnchor(gf::Anchor::BottomCenter);
        m_box.setPosition({600.0f, 770.0f});

        m_text.setFont(m_game->resources.getFont(PATH_FONT));
        m_text.setCharacterSize(18);
        m_text.setColor(gf::Color::fromRgba32(0xD9CCBEFF)); // parchemin
        m_text.setAnchor(gf::Anchor::TopLeft);
        m_text.setPosition({125.0f, 620.0f});
        m_text.setParagraphWidth(920.0f);
        m_text.setLineSpacing(1.6f);

        m_clickHint.setFont(m_game->resources.getFont(PATH_FONT));
        m_clickHint.setString("(Cliquez pour continuer)");
        m_clickHint.setCharacterSize(13);
        m_clickHint.setColor(gf::Color::fromRgba32(0x8C6118FF)); // doré discret
        m_clickHint.setAnchor(gf::Anchor::BottomRight);
        m_clickHint.setPosition({1075.0f, 765.0f});
    }

    void FeeScene::setDialogue(const std::vector<std::string>& lines) {
        m_dialogueLines = lines;
        m_currentLine = 0;
        updateText();
    }

    void FeeScene::updateText() {
        if (m_currentLine < m_dialogueLines.size()) {
            m_text.setString(m_dialogueLines[m_currentLine]);
        }
    }

    void FeeScene::doProcessEvent(gf::Event &event) {
        if (event.type == gf::EventType::MouseButtonPressed && event.mouseButton.button == gf::MouseButton::Left) {
            m_currentLine++;
            
            if (m_currentLine >= m_dialogueLines.size()) {
                m_game->popScene();
            } else {
                updateText();
            }
        }
    }

    void FeeScene::doRender(gf::RenderTarget &target, const gf::RenderStates &states) {
        target.draw(m_box, states);
        target.draw(m_text, states);
        target.draw(m_clickHint, states);
    }

    void FeeScene::startFairyInteraction(WorldEntity& world) {
        std::vector<std::string> discours;

        if (!world.m_feeVisitee) {
            discours = {
                "Bonjour jeune voyageur !",
                "Je suis la fée et je suis là pour te venir en aide. \n ",
                "Fais attention aux monstres qui rodent...\n Ils sont puissants et surtout ils ne veulent pas que \ntu atteignes la fin du donjon.",
                "Bonne chance dans ton aventure ! \n Voici un petit cadeau pour t'aider.",
                "Tu as reçu une potion de soin et une épée en bois !\n Utilise-les judicieusement."
            };

            Item starterPotion;
            starterPotion.m_name = "Potion de départ";
            starterPotion.m_id = "ItemSoin";
            starterPotion.m_type = ItemType::Consumable;
            starterPotion.m_description = "Une petite potion de soin offerte par la fée.";
            starterPotion.m_stat.setHealth(20);
            starterPotion.m_texture = &m_game->resources.getTexture("ItemSoin.png");
            m_game->m_InventoryScene->m_inventory.addItemToBackpack(starterPotion, m_game);

            Item newItem("Epee de base", "EpeeBase", ItemType::Weapon, Item::Rarity::Common,
                         m_game->resources.getTexture("EpeeBase.png"),
                         "Cette épée de mauvaise facture sera le point de départ de votre valeureuse aventure...",
                         Stat(0, 0, 2));
            
            m_game->m_InventoryScene->m_inventory.addItemToBackpack(newItem, m_game);
            m_game->m_FeeScene->setDialogue(discours);

            world.m_feeVisitee = true;
        } else {
            discours = {
                "Tu vas avoir besoin de courage pour \naffronter tous ces dangers qui t'attendent.",
                "Je suis de tout coeur avec toi  ! \n ",
                "J'espère que ta quête se déroulera bien ! Courage !"
            };
            m_game->m_FeeScene->setDialogue(discours);
        }

        m_game->pushScene(*(m_game->m_FeeScene));
        return;
    }


    
}