#include "Inventory.h"
#include "RogueCMI.h"

namespace rCMI
{

	Inventory::Inventory(RogueCMI *game)
			: m_statsWidget("Stats\n", game->resources.getFont(PATH_FONT), 20),
				m_emptySlotTexture(&(game->resources.getTexture("EmptySlot.jpg")))
	{

		m_headSlot.setTexture(game->resources.getTexture("Casque.jpg"));
		m_headSlot.setPosition({220, 110});

		m_torsoSlot.setTexture(game->resources.getTexture("Plastron.png"));
		m_torsoSlot.setPosition({500, 230});

		m_legSlot.setTexture(game->resources.getTexture("Jambieres.jpg"));
		m_legSlot.setPosition({500, 300});

		m_legSlot.setTexture(game->resources.getTexture("Jambieres.jpg"));
		m_handSlot.setPosition({220, 200});

		m_bootsSlot.setTexture(game->resources.getTexture("Bottes.jpg"));
		m_bootsSlot.setPosition({500, 380});

		m_accessorySlot.setTexture(game->resources.getTexture("accessoire.jpg"));
		m_accessorySlot.setPosition({220, 300});

		m_heroSprite.setTexture(game->resources.getTexture("perso640/Perso640.png"));
		float scaleFactor = 400.0f / 640.0f;
		m_heroSprite.setScale({scaleFactor, scaleFactor});
		m_heroSprite.setPosition({230, 100});

		m_background.setSize({800, 700});
		m_background.setColor(gf::Color::White);
		m_background.setPosition({200, 100});

		m_statsWidget.setPosition({800, 200});
		m_statsWidget.setDefaultTextColor(gf::Color::Black);

		gf::Vector2f startPos = {220, 500};
		float slotSize = 80.0f;
		float padding = 25.0f;

		for (std::size_t i = 0; i < MaxBackpackSize; ++i) {
			float x = startPos.x + (i % 5) * (slotSize + padding);
			float y = startPos.y + (i / 5) * (slotSize + padding);
			
			m_backpackSprites[i].setPosition({x, y});
			m_backpackSprites[i].setTexture(*m_emptySlotTexture);
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
		EquippedSlot *slot = getSlotByType(type);

		if (slot != nullptr)
		{
			if (item != nullptr)
			{
				slot->item = *item;
				slot->hasItem = true;
				
			}
			else
				slot->hasItem = false;
		}

		gf::Sprite *targetSprite = getSpriteByType(type);

		if (targetSprite != nullptr) {
			if (item != nullptr && item->m_texture != nullptr) {
				targetSprite->setTexture(*item->m_texture);
				targetSprite->setTextureRect(gf::RectF::fromSize(item->m_texture->getSize()));
			} else {

				switch (type)
				{
					case ItemType::Head:
						targetSprite->setTexture(game->resources.getTexture("Casque.jpg"));
						break;

					case ItemType::Torso:
						targetSprite->setTexture(game->resources.getTexture("Plastron.png"));
						break;

					case ItemType::Legs:
						targetSprite->setTexture(game->resources.getTexture("Jambieres.jpg"));
						break;

					case ItemType::Hand:
						targetSprite->setTexture(game->resources.getTexture("Gants.jpg"));
						break;

					case ItemType::Boots:
						targetSprite->setTexture(game->resources.getTexture("Bottes.jpg"));
						break;

					case ItemType::Accessory:
						targetSprite->setTexture(game->resources.getTexture("accessoire.jpg"));
						break;

						default:
						break;
				}

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
							ItemType::Hand, ItemType::Boots, ItemType::Accessory };

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
		
		int total_force = m_stats.getPower() + bonus_force;
		int total_def   = m_stats.getDefense() + bonus_defense;
		int total_pv    = m_stats.getHealth() + bonus_sante;
		int max_pv      = m_stats.getMaxHealth() + bonus_sante;

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

		for (std::size_t i = 0; i < MaxBackpackSize; ++i) {
			target.draw(m_backpackSprites[i], states);
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
				m_backpackSprites[i].setTexture(tex);
				
				m_backpackSprites[i].setTextureRect(gf::RectF::fromSize(tex.getSize()));
				
				float scaleX = 80.0f / tex.getSize().x;
				float scaleY = 80.0f / tex.getSize().y;
				m_backpackSprites[i].setScale({scaleX, scaleY});
			} else {
				m_backpackSprites[i].setTexture(*m_emptySlotTexture);
				m_backpackSprites[i].setScale({1.0f, 1.0f});
				m_backpackSprites[i].setTextureRect(gf::RectF::fromSize(m_emptySlotTexture->getSize()));
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
		default:
			return nullptr;
		}
	}

	bool Inventory::addItemToBackpack(int item, RogueCMI *game)
	{

		if (m_backpack.size() >= MaxBackpackSize)
		{
			std::cout << "Inventaire plein ! Impossible de ramasser : " << game->m_WorldScene.m_world_entity.m_itemManager.items[item].item.m_name << std::endl;
			return false;
		}
		m_backpack.push_back(game->m_WorldScene.m_world_entity.m_itemManager.items[item].item);
		std::cout << "Item ramassé : " << game->m_WorldScene.m_world_entity.m_itemManager.items[item].item.m_name << std::endl;
		updateBackpackDisplay(game);

		return true;
	}


	void Inventory::handleItemClick(gf::Vector2f coords, RogueCMI *game) {
		for (std::size_t i = 0; i < m_backpack.size(); ++i) {
			if (m_backpackSprites[i].getLocalBounds().contains(coords - m_backpackSprites[i].getPosition())) {
				this->equipFromBackpack(i, game);
				return; 
			}
		}

		if (m_headSlot.getLocalBounds().contains(coords - m_headSlot.getPosition())) {
        	this->onUnequip(ItemType::Head, game);
		}
		else if (m_torsoSlot.getLocalBounds().contains(coords - m_torsoSlot.getPosition())) {
			this->onUnequip(ItemType::Torso, game);
		}
		else if (m_legSlot.getLocalBounds().contains(coords - m_legSlot.getPosition())) {
			this->onUnequip(ItemType::Legs, game);
		}
		else if (m_bootsSlot.getLocalBounds().contains(coords - m_bootsSlot.getPosition())) {
			this->onUnequip(ItemType::Boots, game);
		}
		else if (m_handSlot.getLocalBounds().contains(coords - m_handSlot.getPosition())) {
			this->onUnequip(ItemType::Hand, game);
		}
		else if (m_accessorySlot.getLocalBounds().contains(coords - m_accessorySlot.getPosition())) {
			this->onUnequip(ItemType::Accessory, game);
		}
	}
}
