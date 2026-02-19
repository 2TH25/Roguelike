#include "Inventory.h"
#include "RogueCMI.h"
#include "WorldScene.h" 
#include "WorldEntity.h"
#include "Character.h"
#include <gf/Coordinates.h>

namespace rCMI
{

	Inventory::Inventory(RogueCMI *game)
			: m_statsWidget("Stats\n", game->resources.getFont(PATH_FONT), 20),
			m_skillpoints("Points de compétences : 0\n", game->resources.getFont(PATH_FONT), 20),
				m_emptySlotTexture(&(game->resources.getTexture("SlotVide.png"))),
				m_equippedSlotTexture(&(game->resources.getTexture("SlotVideEquipement.png"))),
				m_plusTexture(&(game->resources.getTexture("Fleche.png")))
	{

		m_stackTexts.clear(); // Sécurité
		for (std::size_t i = 0; i < MaxBackpackSize; ++i) {
			gf::Text text("", game->resources.getFont(PATH_FONT), 15);
			text.setColor(gf::Color::White);
			text.setOutlineColor(gf::Color::Black);
			text.setOutlineThickness(1.0f);
			m_stackTexts.push_back(std::move(text));
		}
		float scale = static_cast<float>(TileSize) / 640.0f;
		

		m_headSlot.setTexture(game->resources.getTexture("SlotCasque.png"));
    	m_headSlot.setScale(scale);
		

		m_torsoSlot.setTexture(game->resources.getTexture("SlotArmure.png"));
		m_torsoSlot.setScale(scale);
		

		m_legSlot.setTexture(game->resources.getTexture("SlotJambe.png"));
		m_legSlot.setScale(scale);
		

		m_handSlot.setTexture(game->resources.getTexture("SlotGants.png"));
		m_handSlot.setScale(scale);
		

		m_bootsSlot.setTexture(game->resources.getTexture("SlotBottes.png"));
		m_bootsSlot.setScale(scale);
		

		m_accessorySlot.setTexture(game->resources.getTexture("SlotAccessoire.png"));
		m_accessorySlot.setScale(scale);
		

		m_weaponSlot.setTexture(game->resources.getTexture("SlotArme.png"));
		m_weaponSlot.setScale(scale);

		m_bowSlot.setTexture(game->resources.getTexture("SlotArc.png"));
		m_bowSlot.setScale(scale);
		

		m_equippedHeadSprite.setPosition(m_headSlot.getPosition());
		m_equippedTorsoSprite.setPosition(m_torsoSlot.getPosition());
		m_equippedLegsSprite.setPosition(m_legSlot.getPosition());
		m_equippedHandSprite.setPosition(m_handSlot.getPosition());
		m_equippedBootsSprite.setPosition(m_bootsSlot.getPosition());
		m_equippedAccessorySprite.setPosition(m_accessorySlot.getPosition());
		m_equippedWeaponSprite.setPosition(m_weaponSlot.getPosition());
		m_equippedBowSprite.setPosition(m_weaponSlot.getPosition());

		m_equippedHeadSprite.setColor(gf::Color::Transparent);
		m_equippedTorsoSprite.setColor(gf::Color::Transparent);
		m_equippedLegsSprite.setColor(gf::Color::Transparent);
		m_equippedHandSprite.setColor(gf::Color::Transparent);
		m_equippedBootsSprite.setColor(gf::Color::Transparent);
		m_equippedAccessorySprite.setColor(gf::Color::Transparent);
		m_equippedWeaponSprite.setColor(gf::Color::Transparent);
		m_equippedBowSprite.setColor(gf::Color::Transparent);

		m_heroSprite.setTexture(game->resources.getTexture("perso640/Perso640.png"));

		m_background.setTexture(game->resources.getTexture("BackgroundInventory.png"));

		m_statsWidget.setPosition({1000, 280});
		m_statsWidget.setDefaultTextColor(gf::Color::Black);

		gf::Vector2f startPos = {420, 500};
		float slotSize = 70.0f;
		float padding = 10.0f;

		for (std::size_t i = 0; i < MaxBackpackSize; ++i) {
			float x = startPos.x + (i % 5) * (slotSize + padding);
			float y = startPos.y + (i / 5) * (slotSize + padding);
			
			m_backpackBackgrounds[i].setPosition({x, y});
			m_backpackBackgrounds[i].setTexture(*m_emptySlotTexture);
			m_backpackBackgrounds[i].setScale(scale);

			m_itemSprites[i].setPosition({x, y});
			m_itemSprites[i].setColor(gf::Color::Transparent);
		}

		float btnScale = 0.05f;
		float statsX = 1000.0f;
		float statsY = 280.0f;
		float lineHeight = 30.0f;
		float xOffset = 250.0f;

		auto setupBtn = [&](gf::Sprite& s, float yOffset) {
			s.setTexture(*m_plusTexture);
			s.setScale(btnScale);
			s.setColor(gf::Color::Transparent);
		};

		setupBtn(m_plusPowerBtn, 0.0f);
		setupBtn(m_plusHealthBtn, lineHeight);
		setupBtn(m_plusDefBtn, lineHeight * 2);

		updateStatsText();
	}

	gf::Sprite *Inventory::getSpriteByType(ItemType type)
	{
		switch (type)
		{
		case ItemType::Head:
			return &m_headSlot;
		case ItemType::Torso:
			return &m_torsoSlot;
		case ItemType::Legs:
			return &m_legSlot;
		case ItemType::Hand:
			return &m_handSlot;
		case ItemType::Boots:
			return &m_bootsSlot;
		case ItemType::Accessory:
			return &m_accessorySlot;
		case ItemType::Weapon:
			return &m_weaponSlot;
		case ItemType::Bow:
			return &m_bowSlot;
		default:
			return nullptr;
		}
	}

	bool Inventory::hasEquipment(ItemType type)
	{
		EquippedSlot *slot = getSlotByType(type);
		return (slot != nullptr && slot->hasItem);
	}

	Item Inventory::setEquippedItem(ItemType type, Item *Newitem, RogueCMI *game)
{
	Character &hero = game->m_WorldScene.m_world_entity.hero();
    EquippedSlot *slot = getSlotByType(type);
	Item oldItem;
    if (slot != nullptr) {
            
		if (slot->hasItem) {
			oldItem = slot->item;
			hero.getStat().setPower(hero.getStat().getPower() - slot->item.m_stat.getPower());
			hero.getStat().setDefense(hero.getStat().getDefense() - slot->item.m_stat.getDefense());
			hero.addMaxHealth(-slot->item.m_stat.getHealth());
		}

		if (Newitem != nullptr) {
			slot->item = *Newitem;
			slot->hasItem = true;

			hero.getStat().setPower(hero.getStat().getPower() + Newitem->m_stat.getPower());
			hero.getStat().setDefense(hero.getStat().getDefense() + Newitem->m_stat.getDefense());
			hero.addMaxHealth(Newitem->m_stat.getHealth());
		} else {
			slot->hasItem = false;
		}
	}

	m_stats = hero.getStat();

	gf::Sprite *itemSprite = nullptr;
	gf::Sprite *backgroundSlot = nullptr;
	switch (type) {
        case ItemType::Head:      itemSprite = &m_equippedHeadSprite; backgroundSlot = &m_headSlot; break;
        case ItemType::Torso:     itemSprite = &m_equippedTorsoSprite; backgroundSlot = &m_torsoSlot; break;
        case ItemType::Legs:      itemSprite = &m_equippedLegsSprite;  backgroundSlot = &m_legSlot; break;
        case ItemType::Hand:      itemSprite = &m_equippedHandSprite;  backgroundSlot = &m_handSlot; break;
        case ItemType::Boots:     itemSprite = &m_equippedBootsSprite; backgroundSlot = &m_bootsSlot; break;
        case ItemType::Accessory: itemSprite = &m_equippedAccessorySprite; backgroundSlot = &m_accessorySlot; break;
        case ItemType::Weapon:    itemSprite = &m_equippedWeaponSprite;    backgroundSlot = &m_weaponSlot; break;
        case ItemType::Bow:       itemSprite = &m_equippedBowSprite;       backgroundSlot = &m_bowSlot; break;
        default: break;
    }

    if (itemSprite && backgroundSlot) {
        if (Newitem != nullptr && Newitem->m_texture != nullptr) {
            itemSprite->setTexture(*Newitem->m_texture);
            itemSprite->setTextureRect(gf::RectF::fromSize(Newitem->m_texture->getSize()));
            itemSprite->setColor(gf::Color::White); // On le rend visible
			backgroundSlot->setTexture(*m_equippedSlotTexture);
            float scale = static_cast<float>(TileSize) / 640.0f;
            itemSprite->setScale(scale);
        } else {
            itemSprite->setColor(gf::Color::Transparent); // On le cache
			resetSlotBackground(type, backgroundSlot, game);
        }
    }
    updateStatsText();
	return oldItem;
}


	void Inventory::resetSlotBackground(ItemType type, gf::Sprite* backgroundSlot, RogueCMI *game) {
		switch (type) {
			case ItemType::Head:      backgroundSlot->setTexture(game->resources.getTexture("SlotCasque.png")); break;
			case ItemType::Torso:     backgroundSlot->setTexture(game->resources.getTexture("SlotArmure.png")); break;
			case ItemType::Legs:      backgroundSlot->setTexture(game->resources.getTexture("SlotJambe.png")); break;
			case ItemType::Hand:      backgroundSlot->setTexture(game->resources.getTexture("SlotGants.png")); break;
			case ItemType::Boots:     backgroundSlot->setTexture(game->resources.getTexture("SlotBottes.png")); break;
			case ItemType::Accessory: backgroundSlot->setTexture(game->resources.getTexture("SlotAccessoire.png")); break;
			case ItemType::Weapon:    backgroundSlot->setTexture(game->resources.getTexture("SlotArme.png")); break;
			case ItemType::Bow:       backgroundSlot->setTexture(game->resources.getTexture("SlotArc.png")); break;
			default: break;
		}
	}

	Item Inventory::getEquippedItem(ItemType type)
	{
		EquippedSlot *slot = getSlotByType(type);
		if (slot && slot->hasItem)
			return slot->item;

		return Item();
	}

	void Inventory::onUnequip(ItemType type, RogueCMI *game)
	{
		if (hasEquipment(type))
		{
			if (m_backpack.size() >= MaxBackpackSize)
			{
				std::cout << "Sac plein ! Veuillez jeter un item avant de déséquiper celui-ci !" << std::endl;
				return;
			}
			Item item = getEquippedItem(type);
			m_backpack.push_back(item);
			setEquippedItem(type, nullptr, game);
			updateStatsText();
			updateBackpackDisplay(game);
		}
	}

	void Inventory::updateStatsText()
	{
		int bonus_force = 0;
		int bonus_sante = 0;
		int bonus_defense = 0;

		ItemType types[] = { ItemType::Head, ItemType::Torso, ItemType::Legs, 
							ItemType::Hand, ItemType::Boots, ItemType::Accessory, ItemType::Weapon};

		for (int i = 0; i < 7; ++i)
		{
			if (hasEquipment(types[i]))
			{
				Item it = getEquippedItem(types[i]);
				bonus_force   += it.m_stat.getPower();
				bonus_sante   += it.m_stat.getHealth();
				bonus_defense += it.m_stat.getDefense();
			}
		}
		
		int total_force = m_stats.getPower() ;
		int total_def   = m_stats.getDefense() ;
		int total_pv    = m_stats.getHealth() ;
		int max_pv      = m_stats.getMaxHealth() ;

		std::string str = "Force : " + std::to_string(total_force) + " (+" + std::to_string(bonus_force) + ")\n" +
						"Sante : " + std::to_string(total_pv) + " / " + std::to_string(max_pv) + " (+" + std::to_string(bonus_sante) + ")\n" +
						"Defense : " + std::to_string(total_def) + " (+" + std::to_string(bonus_defense) + ")";
		
		m_statsWidget.setString(str);

		int pts = m_stats.getSkillPoints(); 
    	m_skillpoints.setString("Points de compétences : " + std::to_string(pts));

		if (pts > 0) {
			m_plusPowerBtn.setColor(gf::Color::White);
			m_plusHealthBtn.setColor(gf::Color::White);
			m_plusDefBtn.setColor(gf::Color::White);
		} else {
			m_plusPowerBtn.setColor(gf::Color::Transparent);
			m_plusHealthBtn.setColor(gf::Color::Transparent);
			m_plusDefBtn.setColor(gf::Color::Transparent);
		}
	}

	void Inventory::updateSkillPointsText() {
		
	}

	void Inventory::render(gf::RenderTarget &target, const gf::RenderStates &states) {
		const gf::Vector2f vSize = target.getView().getSize();
		gf::Coordinates coords(target);

		gf::Vector2f invSize = vSize * 0.75f;
		gf::Vector2f invPos = (vSize - invSize) / 2.0f;

		m_background.setSize(invSize);
		m_background.setPosition(invPos);
		target.draw(m_background, states);

		float slotSize = invSize.x * 0.06;
		float scale = slotSize / 640.0f; 

		auto setPos = [&](gf::Sprite& s, gf::Sprite& equipped, float rx, float ry) {
			s.setScale(scale);
			s.setPosition({invPos.x + invSize.x * rx, invPos.y + invSize.y * ry});
			equipped.setScale(scale);
			equipped.setPosition(s.getPosition());
		};

		
		float colLeft = 0.15f;
		float colMid  = 0.43f;
		float colRight = 0.55f;

		float y_first = 0.17f;
		float y_second = 0.32f;
		float y_third = 0.47f;

		setPos(m_headSlot, m_equippedHeadSprite, colLeft, y_first);
		setPos(m_handSlot, m_equippedHandSprite, colLeft, y_second);
		setPos(m_accessorySlot, m_equippedAccessorySprite, colLeft, y_third);
		
		setPos(m_torsoSlot, m_equippedTorsoSprite, colMid, y_first);
		setPos(m_legSlot, m_equippedLegsSprite, colMid, y_second);
		setPos(m_bootsSlot, m_equippedBootsSprite, colMid, y_third);
		
		setPos(m_weaponSlot, m_equippedWeaponSprite, colRight, y_first);
		setPos(m_bowSlot, m_equippedBowSprite, colRight, y_second);

	
		float hero_rx = (colLeft + colMid) / 2.0f; 
		

		float heroScale = (invSize.x * 0.35f) / 640.0f; 
		m_heroSprite.setScale({heroScale, heroScale});

		
		float heroWidth = 640.0f * heroScale;
		m_heroSprite.setPosition({
			invPos.x + (invSize.x * hero_rx) - (heroWidth / 2.5f),
			invPos.y + (invSize.y * 0.05f)
		});

	
		float padding = invSize.x * 0.01f;
		float startX = invPos.x + (invSize.x * 0.15f);
		float startY = invPos.y + (invSize.y * 0.60f);

		for (std::size_t i = 0; i < MaxBackpackSize; ++i) {
			float x = startX + (i % 5) * (slotSize + padding);
			float y = startY + (i / 5) * (slotSize + padding);
			
			m_backpackBackgrounds[i].setScale(scale);
			m_backpackBackgrounds[i].setPosition({x, y});
			m_itemSprites[i].setScale(scale);
			m_itemSprites[i].setPosition({x, y});

			target.draw(m_backpackBackgrounds[i], states);
			target.draw(m_itemSprites[i], states);
    
			if (!m_stackTexts[i].getString().empty()) {
				float textX = x + slotSize - 25.0f;
				float textY = y + slotSize - 20.0f;
				m_stackTexts[i].setPosition({textX, textY});
				target.draw(m_stackTexts[i], states);
			}
		}

		m_skillpoints.setCharacterSize(static_cast<unsigned int>(invSize.y * 0.035f));
		m_skillpoints.setPosition({invPos.x + invSize.x * 0.65f, invPos.y + invSize.y * 0.25f});

	
		m_statsWidget.setCharacterSize(static_cast<unsigned int>(invSize.y * 0.035f));
		m_statsWidget.setPosition({invPos.x + invSize.x * 0.65f, invPos.y + invSize.y * 0.35f});

		float buttonsX = m_statsWidget.getPosition().x + (invSize.x * 0.22f);
		float firstButtonY = m_statsWidget.getPosition().y + (invSize.y * 0.005f);
		float spacingY = invSize.y * 0.038f;

		m_plusPowerBtn.setPosition({buttonsX, firstButtonY});
		m_plusHealthBtn.setPosition({buttonsX, firstButtonY + spacingY});
		m_plusDefBtn.setPosition({buttonsX, firstButtonY + (spacingY * 2)});

		
		target.draw(m_heroSprite, states);
		target.draw(m_headSlot, states); target.draw(m_equippedHeadSprite, states);
		target.draw(m_torsoSlot, states); target.draw(m_equippedTorsoSprite, states);
		target.draw(m_legSlot, states); target.draw(m_equippedLegsSprite, states);
		target.draw(m_handSlot, states); target.draw(m_equippedHandSprite, states);
		target.draw(m_bootsSlot, states); target.draw(m_equippedBootsSprite, states);
		target.draw(m_accessorySlot, states); target.draw(m_equippedAccessorySprite, states);
		target.draw(m_weaponSlot, states); target.draw(m_equippedWeaponSprite, states);
		target.draw(m_bowSlot, states); target.draw(m_equippedBowSprite, states);
		target.draw(m_statsWidget, states);
		target.draw(m_skillpoints, states);
		target.draw(m_plusPowerBtn, states);
		target.draw(m_plusHealthBtn, states);
		target.draw(m_plusDefBtn, states);
	}

	void Inventory::updateInventory(RogueCMI *game)
	{
		auto &hero = game->m_WorldScene.m_world_entity.hero();
		m_stats = hero.getStat();
		updateStatsText();
	}

	void Inventory::updateBackpackDisplay(RogueCMI *game) {
		for (std::size_t i = 0; i < MaxBackpackSize; ++i) {
			if (i < m_backpack.size()) {
				const gf::Texture& tex = *(m_backpack[i].m_texture);
				m_itemSprites[i].setTexture(tex);
				m_itemSprites[i].setColor(gf::Color::White);
				
				if (m_backpack[i].m_count > 1) {
					m_stackTexts[i].setString("x" + std::to_string(m_backpack[i].m_count));
				} else {
					m_stackTexts[i].setString("");
				}
			} else {
				m_itemSprites[i].setColor(gf::Color::Transparent);
				m_stackTexts[i].setString("");
			}
		}
	}

	void Inventory::equipFromBackpack(std::size_t index, RogueCMI *game)
	{
		if (index >= m_backpack.size())
			return;

		Item itemToEquip = m_backpack[index];
		ItemType type = itemToEquip.m_type;

		if (hasEquipment(type))
		{
			Item alreadyEquipped = getEquippedItem(type);
			m_backpack[index] = alreadyEquipped; 
		}
		else
		{
			m_backpack.erase(m_backpack.begin() + index);
		}
		setEquippedItem(type, &itemToEquip, game);

		updateBackpackDisplay(game);
		updateStatsText();
	}

	Inventory::EquippedSlot *Inventory::getSlotByType(ItemType type)
	{
		switch (type)
		{
		case ItemType::Head:
			return &m_equippedHead;
		case ItemType::Torso:
			return &m_equippedTorso;
		case ItemType::Legs:
			return &m_equippedLegs;
		case ItemType::Hand:
			return &m_equippedHand;
		case ItemType::Boots:
			return &m_equippedBoots;
		case ItemType::Accessory:
			return &m_equippedAccessory;
		case ItemType::Weapon:
			return &m_equippedWeapon;
		case ItemType::Bow:
			return &m_equippedBow;
		default:
			return nullptr;
		}
	}

	bool Inventory::addItemToBackpack(Item item, RogueCMI *game) {
		if (item.m_name.empty() || item.m_texture == nullptr) return false;

		if (item.m_name == "Fleche" || item.m_type == ItemType::Misc) { 
			for (auto &backpackItem : m_backpack) {
				if (backpackItem.m_name == item.m_name && backpackItem.m_count < 10) {
					backpackItem.m_count += 1;
					updateBackpackDisplay(game);
					return true;
				}
			}
		}

		if (m_backpack.size() >= MaxBackpackSize) {
			return false;
		}

		item.m_count = 1;
		m_backpack.push_back(item);
		updateBackpackDisplay(game);
		return true;
	}



	void Inventory::handleItemClick(gf::Vector2f coords, RogueCMI *game) {
		const gf::Vector2f slotSize = { 80.0f, 80.0f };

		if (m_plusPowerBtn.getColor().a > 0) { 
			handleStatUpgrade(coords, game);
		}

		for (std::size_t i = 0; i < MaxBackpackSize; ++i) {
			gf::Vector2f pos = m_backpackBackgrounds[i].getPosition();
			
			gf::RectF box = gf::RectF::fromPositionSize(pos, slotSize);

			if (box.contains(coords)) {
				if (i < m_backpack.size()) {
					game->m_ItemScene.setItem(m_backpack[i], false);
                	game->pushScene(game->m_ItemScene);
				}
				return;
			}
		}

		auto checkSlot = [&](gf::Sprite& sprite, ItemType type) {
			gf::RectF box = gf::RectF::fromPositionSize(sprite.getPosition(), slotSize);
			if (box.contains(coords)) {
				if (hasEquipment(type)) {
					Item equippedItem = getEquippedItem(type);
					game->m_ItemScene.setItem(equippedItem, true);
					game->pushScene(game->m_ItemScene);
				}
			}
			return false;
		};

		if (checkSlot(m_headSlot, ItemType::Head)) return;
		if (checkSlot(m_torsoSlot, ItemType::Torso)) return;
		if (checkSlot(m_legSlot, ItemType::Legs)) return;
		if (checkSlot(m_bootsSlot, ItemType::Boots)) return;
		if (checkSlot(m_handSlot, ItemType::Hand)) return;
		if (checkSlot(m_accessorySlot, ItemType::Accessory)) return;
		if (checkSlot(m_weaponSlot, ItemType::Weapon)) return;
		if (checkSlot(m_bowSlot, ItemType::Bow)) return;
	}

	bool Inventory::addItemFromChest(int chestIndex, RogueCMI *game) {
		std::vector<Item>& itemsInChest = game->m_WorldScene.m_world_entity.m_chestManager.getChest(chestIndex).content;
		if (itemsInChest.empty()) return true;

		auto it = itemsInChest.begin();
		while (it != itemsInChest.end()) {
			if (addItemToBackpack(*it, game)) {
				it = itemsInChest.erase(it); 
			} else {
				std::cout << "Sac plein !" << std::endl;
				return false;
			}
		}
		return true;
	}


	void Inventory::removeItemFromBackpack(const Item& item, RogueCMI *game) {

		auto it = std::find_if(m_backpack.begin(), m_backpack.end(), [&](const Item& backpackItem) {
			return backpackItem.m_name == item.m_name && backpackItem.m_type == item.m_type;
		});

		if (it != m_backpack.end()) {
			m_backpack.erase(it);
			std::cout << item.m_name << " a été retiré du sac." << std::endl;
			updateBackpackDisplay(game);
		}
	}


	void Inventory::consumeItem(const Item& item, RogueCMI *game) {

		Character &hero = game->m_WorldScene.m_world_entity.hero();
		int healAmount = item.m_stat.getHealth();
		
		if (healAmount > 0) {
			hero.heal(healAmount);
			std::cout << "Vous consommez " << item.m_name << " et récupérez " << healAmount << " PV." << std::endl;
		}
		removeItemFromBackpack(item, game);
		updateStatsText();
	}

	void Inventory::handleStatUpgrade(gf::Vector2f coords, RogueCMI *game) {
		Character &hero = game->m_WorldScene.m_world_entity.hero();
		
		if (hero.getStat().getSkillPoints() <= 0) return; 

		gf::Vector2f textureSize = m_plusTexture->getSize();
		gf::Vector2f btnSize = textureSize * m_plusPowerBtn.getScale(); 

		gf::RectF powerBox = gf::RectF::fromPositionSize(m_plusPowerBtn.getPosition(), btnSize);
		gf::RectF healthBox = gf::RectF::fromPositionSize(m_plusHealthBtn.getPosition(), btnSize);
		gf::RectF defBox = gf::RectF::fromPositionSize(m_plusDefBtn.getPosition(), btnSize);

		bool statChanged = false;

		if (powerBox.contains(coords)) {
			hero.getStat().setPower(hero.getStat().getPower() + 1);
			statChanged = true;
		}
		else if (healthBox.contains(coords)) {
			hero.addMaxHealth(10);
			hero.heal(10);
			statChanged = true;
		}
		else if (defBox.contains(coords)) {
			hero.getStat().setDefense(hero.getStat().getDefense() + 1);
			statChanged = true;
		}

		if (statChanged) {
			hero.getStat().setSkillPoints(hero.getStat().getSkillPoints() - 1);
			updateInventory(game); 
		}
	}
}
