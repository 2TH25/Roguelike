#include "Inventory.h"
#include "RogueCMI.h"
#include "WorldScene.h" 
#include "WorldEntity.h"
#include "Character.h"

namespace rCMI
{

	Inventory::Inventory(RogueCMI *game)
			: m_statsWidget("Stats\n", game->resources.getFont(PATH_FONT), 20),
				m_emptySlotTexture(&(game->resources.getTexture("SlotVide.png")))
	{
		float scale = static_cast<float>(TileSize) / 640.0f;
		float x_slot_left = 430;
		float x_slot_right = 720;
		float y_slot_first = 220;
		float y_slot_second = 310;
		float y_slot_third = 400;

		m_headSlot.setTexture(game->resources.getTexture("SlotCasque.png"));
    	m_headSlot.setScale(scale);
		m_headSlot.setPosition({x_slot_left, y_slot_first});

		m_torsoSlot.setTexture(game->resources.getTexture("SlotArmure.png"));
		m_torsoSlot.setScale(scale);
		m_torsoSlot.setPosition({x_slot_right, y_slot_first});

		m_legSlot.setTexture(game->resources.getTexture("SlotJambe.png"));
		m_legSlot.setScale(scale);
		m_legSlot.setPosition({x_slot_right, y_slot_second});

		m_handSlot.setTexture(game->resources.getTexture("SlotGants.png"));
		m_handSlot.setScale(scale);
		m_handSlot.setPosition({x_slot_left, y_slot_second});

		m_bootsSlot.setTexture(game->resources.getTexture("SlotBottes.png"));
		m_bootsSlot.setScale(scale);
		m_bootsSlot.setPosition({x_slot_right, y_slot_third});

		m_accessorySlot.setTexture(game->resources.getTexture("SlotAccessoire.png"));
		m_accessorySlot.setScale(scale);
		m_accessorySlot.setPosition({x_slot_left, y_slot_third});

		m_weaponSlot.setTexture(game->resources.getTexture("SlotArme.png"));
		m_weaponSlot.setScale(scale);
		m_weaponSlot.setPosition({850, y_slot_first});

		m_equippedHeadSprite.setPosition(m_headSlot.getPosition());
		m_equippedTorsoSprite.setPosition(m_torsoSlot.getPosition());
		m_equippedLegsSprite.setPosition(m_legSlot.getPosition());
		m_equippedHandSprite.setPosition(m_handSlot.getPosition());
		m_equippedBootsSprite.setPosition(m_bootsSlot.getPosition());
		m_equippedAccessorySprite.setPosition(m_accessorySlot.getPosition());
		m_equippedWeaponSprite.setPosition(m_weaponSlot.getPosition());

		m_equippedHeadSprite.setColor(gf::Color::Transparent);
		m_equippedTorsoSprite.setColor(gf::Color::Transparent);
		m_equippedLegsSprite.setColor(gf::Color::Transparent);
		m_equippedHandSprite.setColor(gf::Color::Transparent);
		m_equippedBootsSprite.setColor(gf::Color::Transparent);
		m_equippedAccessorySprite.setColor(gf::Color::Transparent);
		m_equippedWeaponSprite.setColor(gf::Color::Transparent);

		m_heroSprite.setTexture(game->resources.getTexture("perso640/Perso640.png"));
		float scaleFactor = 400.0f / 640.0f;
		m_heroSprite.setScale({scaleFactor, scaleFactor});
		m_heroSprite.setPosition({430, 150});

		m_background.setSize({1100, 700});
		m_background.setTexture(game->resources.getTexture("BackgroundInventory.png"));
		// m_background.setScale();
		m_background.setPosition({300, 100});

		m_statsWidget.setPosition({1000, 280});
		m_statsWidget.setDefaultTextColor(gf::Color::Black);

		gf::Vector2f startPos = {420, 500};
		float slotSize = 80.0f;
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
		default:
			return nullptr;
		}
	}

	bool Inventory::hasEquipment(ItemType type)
	{
		EquippedSlot *slot = getSlotByType(type);
		return (slot != nullptr && slot->hasItem);
	}

	void Inventory::setEquippedItem(ItemType type, Item *item, RogueCMI *game)
{
	Character &hero = game->m_WorldScene.m_world_entity.hero();
    EquippedSlot *slot = getSlotByType(type);
    if (slot != nullptr) {
            
		if (slot->hasItem) {
			hero.getStat().setPower(hero.getStat().getPower() - slot->item.m_stat.getPower());
			hero.getStat().setDefense(hero.getStat().getDefense() - slot->item.m_stat.getDefense());
			hero.addMaxHealth(-slot->item.m_stat.getHealth());
		}

		if (item != nullptr) {
			slot->item = *item;
			slot->hasItem = true;

			hero.getStat().setPower(hero.getStat().getPower() + item->m_stat.getPower());
			hero.getStat().setDefense(hero.getStat().getDefense() + item->m_stat.getDefense());
			hero.addMaxHealth(item->m_stat.getHealth());
		} else {
			slot->hasItem = false;
		}
	}

	m_stats = hero.getStat();

	gf::Sprite *itemSprite = nullptr;
	switch (type) {
		case ItemType::Head:      itemSprite = &m_equippedHeadSprite; break;
		case ItemType::Torso:     itemSprite = &m_equippedTorsoSprite; break;
		case ItemType::Legs:      itemSprite = &m_equippedLegsSprite; break;
		case ItemType::Hand:      itemSprite = &m_equippedHandSprite; break;
		case ItemType::Boots:     itemSprite = &m_equippedBootsSprite; break;
		case ItemType::Accessory: itemSprite = &m_equippedAccessorySprite; break;
		case ItemType::Weapon: itemSprite = &m_equippedWeaponSprite; break;
		default: break;
	}

    if (itemSprite != nullptr) {
        if (item != nullptr && item->m_texture != nullptr) {
            itemSprite->setTexture(*item->m_texture);
            itemSprite->setTextureRect(gf::RectF::fromSize(item->m_texture->getSize()));
            itemSprite->setColor(gf::Color::White); // On le rend visible
            float scale = static_cast<float>(TileSize) / 640.0f;
            itemSprite->setScale(scale);
        } else {
            itemSprite->setColor(gf::Color::Transparent); // On le cache
        }
    }
    updateStatsText();
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

		for (int i = 0; i < 6; ++i)
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
	}

	void Inventory::render(gf::RenderTarget &target, const gf::RenderStates &states) {
		target.draw(m_background, states);
		target.draw(m_heroSprite, states);
		
		target.draw(m_headSlot, states);
		target.draw(m_torsoSlot, states);
		target.draw(m_legSlot, states);
		target.draw(m_handSlot, states);
		target.draw(m_bootsSlot, states);
		target.draw(m_accessorySlot, states);
		target.draw(m_weaponSlot, states);

		target.draw(m_equippedHeadSprite, states);
		target.draw(m_equippedTorsoSprite, states);
		target.draw(m_equippedLegsSprite, states);
		target.draw(m_equippedHandSprite, states);
		target.draw(m_equippedBootsSprite, states);
		target.draw(m_equippedAccessorySprite, states);
		target.draw(m_equippedWeaponSprite, states);

		for (std::size_t i = 0; i < MaxBackpackSize; ++i) {
			target.draw(m_backpackBackgrounds[i], states);
		}
		for (std::size_t i = 0; i < MaxBackpackSize; ++i) {
        	target.draw(m_itemSprites[i], states);
    	}
		
		target.draw(m_statsWidget, states);
	}

	void Inventory::updateInventory(RogueCMI *game)
	{
		auto &hero = game->m_WorldScene.m_world_entity.hero();
		m_stats = hero.getStat();
		updateStatsText();
	}

	void Inventory::updateBackpackDisplay(RogueCMI *game) {
		std::cout << "Update fait" << std::endl;
		for (std::size_t i = 0; i < MaxBackpackSize; ++i) {
			if (i < m_backpack.size()) {
				const gf::Texture& tex = *(m_backpack[i].m_texture);
				m_itemSprites[i].setTexture(tex);
				m_itemSprites[i].setTextureRect(gf::RectF::fromSize(tex.getSize()));
				float scale = static_cast<float>(TileSize) / 640.0f;
				m_itemSprites[i].setScale(scale);
				m_itemSprites[i].setColor(gf::Color::White); // On le rend visible
			} else {
				m_itemSprites[i].setColor(gf::Color::Transparent);
				
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
		default:
			return nullptr;
		}
	}

	bool Inventory::addItemToBackpack(Item item, RogueCMI *game)
	{
		if (m_backpack.size() >= MaxBackpackSize)
		{
			std::cout << "Inventaire plein ! Impossible de ramasser : " << item.m_name << std::endl;
			return false;
		}

		m_backpack.push_back(item);
		
		std::cout << "Item ramasse : " << item.m_name << std::endl;

		updateBackpackDisplay(game);

		return true;
	}



	void Inventory::handleItemClick(gf::Vector2f coords, RogueCMI *game) {
		const gf::Vector2f slotSize = { 80.0f, 80.0f };

		for (std::size_t i = 0; i < MaxBackpackSize; ++i) {
			gf::Vector2f pos = m_backpackBackgrounds[i].getPosition();
			
			gf::RectF box = gf::RectF::fromPositionSize(pos, slotSize);

			if (box.contains(coords)) {
				if (i < m_backpack.size()) {
					this->equipFromBackpack(i, game);
				}
				return;
			}
		}

		auto checkSlot = [&](gf::Sprite& sprite, ItemType type) {
			gf::RectF box = gf::RectF::fromPositionSize(sprite.getPosition(), slotSize);
			if (box.contains(coords)) {
				this->onUnequip(type, game);
				return true;
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
	}

	bool Inventory::addItemFromChest(int chestIndex, RogueCMI *game) {
		std::vector<Item>& itemsInChest = game->m_WorldScene.m_world_entity.m_chestManager.getChest(chestIndex).content;

		if (itemsInChest.empty()) return true;

		auto it = itemsInChest.begin();
		while (it != itemsInChest.end()) {
			if (m_backpack.size() < MaxBackpackSize) {
				
				std::cout << "Vous avez obtenu : " << it->m_name << std::endl;
				m_backpack.push_back(*it);
			
				it = itemsInChest.erase(it); 
			} else {
				std::cout << "Sac plein ! Certains objets sont restes dans le coffre." << std::endl;
				updateBackpackDisplay(game);
				return false;
			}
		}

		updateBackpackDisplay(game);
		return true;
	}
}
