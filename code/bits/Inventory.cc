#include "Inventory.h"
#include "RogueCMI.h"

namespace rCMI
{

	Inventory::Inventory(RogueCMI *game)
			: m_statsWidget("Stats\n", game->resources.getFont(PATH_FONT), 20),
				m_emptySlotTexture(&(game->resources.getTexture("EmptySlot.jpg"))),
				m_headSlot(game->resources.getTexture("Casque.jpg"), game->resources.getTexture("Casque.jpg"), game->resources.getTexture("Casque.jpg")),
				m_torsoSlot(game->resources.getTexture("Plastron.png"), game->resources.getTexture("Plastron.png"), game->resources.getTexture("Plastron.png")),
				m_legSlot(game->resources.getTexture("Jambieres.jpg"), game->resources.getTexture("Jambieres.jpg"), game->resources.getTexture("Jambieres.jpg")),
				m_handSlot(game->resources.getTexture("Gants.jpg"), game->resources.getTexture("Gants.jpg"), game->resources.getTexture("Gants.jpg")),
				m_feetSlot(game->resources.getTexture("Bottes.jpg"), game->resources.getTexture("Bottes.jpg"), game->resources.getTexture("Bottes.jpg")),
				m_accessorySlot(game->resources.getTexture("accessoire.jpg"), game->resources.getTexture("accessoire.jpg"), game->resources.getTexture("accessoire.jpg"))
	{
		m_headSlot.setPosition({220, 110});
		m_legSlot.setPosition({500, 300});
		m_torsoSlot.setPosition({500, 230});
		m_handSlot.setPosition({220, 200});
		m_feetSlot.setPosition({500, 380});
		m_accessorySlot.setPosition({220, 300});

		m_heroSprite.setTexture(game->resources.getTexture("perso640/Perso640.png"));
		float scaleFactor = 400.0f / 640.0f;
		m_heroSprite.setScale({scaleFactor, scaleFactor});
		m_heroSprite.setPosition({230, 100});

		m_container.addWidget(m_headSlot);
		m_container.addWidget(m_legSlot);
		m_container.addWidget(m_torsoSlot);
		m_container.addWidget(m_handSlot);
		m_container.addWidget(m_feetSlot);
		m_container.addWidget(m_accessorySlot);

		m_background.setSize({800, 700});
		m_background.setColor(gf::Color::White);
		m_background.setPosition({200, 100});

		m_statsWidget.setPosition({800, 200});
		m_statsWidget.setDefaultTextColor(gf::Color::Black);

		gf::Vector2f startPos = {220, 500};
		float slotSize = 80.0f;
		float padding = 25.0f;

		for (std::size_t i = 0; i < MaxBackpackSize; ++i)
		{
			m_backpackWidgets[i] = gf::SpriteWidget(*m_emptySlotTexture, *m_emptySlotTexture, *m_emptySlotTexture);

			float x = startPos.x + (i % 5) * (slotSize + padding);
			float y = startPos.y + (i / 5) * (slotSize + padding);
			m_backpackWidgets[i].setPosition({x, y});

			m_container.addWidget(m_backpackWidgets[i]);

			m_backpackWidgets[i].setCallback([this, i, game]()
																			 { this->equipFromBackpack(i, game); });
		}

		updateStatsText();

		m_headSlot.setCallback([this, game]()
													 { this->onUnequip(ItemType::Head, game); });
		m_legSlot.setCallback([this, game]()
													{ this->onUnequip(ItemType::Legs, game); });
		m_torsoSlot.setCallback([this, game]()
														{ this->onUnequip(ItemType::Torso, game); });
		m_handSlot.setCallback([this, game]()
													 { this->onUnequip(ItemType::Hand, game); });
		m_feetSlot.setCallback([this, game]()
													 { this->onUnequip(ItemType::Feet, game); });
		m_accessorySlot.setCallback([this, game]()
																{ this->onUnequip(ItemType::Accessory, game); });
	}

	gf::SpriteWidget *Inventory::getWidgetByType(ItemType type)
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
		case ItemType::Feet:
			return &m_feetSlot;
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

		gf::SpriteWidget *targetWidget = getWidgetByType(type);

		if (targetWidget != nullptr)
		{
			gf::RectF r = gf::RectF::fromPositionSize({0, 0}, {80, 80});

			if (item != nullptr)
				targetWidget->setDefaultSprite(*item->m_texture, r);

			else
			{
				switch (type)
				{
				case ItemType::Head:
					targetWidget->setDefaultSprite(game->resources.getTexture("Casque.jpg"), r);
					break;
				case ItemType::Torso:
					targetWidget->setDefaultSprite(game->resources.getTexture("Plastron.png"), r);
					break;
				case ItemType::Legs:
					targetWidget->setDefaultSprite(game->resources.getTexture("Jambieres.jpg"), r);
					break;
				case ItemType::Hand:
					targetWidget->setDefaultSprite(game->resources.getTexture("Gants.jpg"), r);
					break;
				case ItemType::Feet:
					targetWidget->setDefaultSprite(game->resources.getTexture("Bottes.jpg"), r);
					break;
				case ItemType::Accessory:
					targetWidget->setDefaultSprite(game->resources.getTexture("accessoire.jpg"), r);
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
				std::cout << "Sac plein !" << std::endl;
				return;
			}

			Item item = getEquippedItem(type);
			m_backpack.push_back(item);
			setEquippedItem(type, nullptr, game);

			updateBackpackDisplay(game);
			updateStatsText();
		}
	}

	void Inventory::updateStatsText()
	{
		std::string str = "Force : " + std::to_string(m_stats.getPower()) + "\n" +
											"Santé : " + std::to_string(m_stats.getHealth()) + " / " + std::to_string(m_stats.getMaxHealth()) + "\n" +
											"Defense : " + std::to_string(m_stats.getDefense());
		m_statsWidget.setString(str);
	}

	void Inventory::render(gf::RenderTarget &target, const gf::RenderStates &states)
	{
		target.draw(m_background, states);
		target.draw(m_heroSprite, states);
		m_container.render(target, states);
		m_statsWidget.draw(target, states);
	}

	void Inventory::updateInventory(RogueCMI *game)
	{
		auto &hero = game->m_WorldScene.m_world_entity.hero();
		m_stats = hero.getStat();
		updateStatsText();
	}

	void Inventory::updateBackpackDisplay(RogueCMI *game)
	{
		gf::RectF r = gf::RectF::fromPositionSize({0, 0}, {80, 80});

		for (std::size_t i = 0; i < MaxBackpackSize; ++i)
		{
			if (i < m_backpack.size())
				m_backpackWidgets[i].setDefaultSprite(*m_backpack[i].m_texture, r);
			else
				m_backpackWidgets[i].setDefaultSprite(*m_emptySlotTexture, r);
		}
	}

	void Inventory::equipFromBackpack(std::size_t index, RogueCMI *game)
	{
		if (index >= m_backpack.size())
			return;

		Item itemToEquip = m_backpack[index];

		if (hasEquipment(itemToEquip.m_type))
		{
			Item alreadyEquipped = getEquippedItem(itemToEquip.m_type);
			Item temp = alreadyEquipped;
			m_backpack[index] = temp;
		}
		else
			m_backpack.erase(m_backpack.begin() + index);

		setEquippedItem(itemToEquip.m_type, &itemToEquip, game);

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
		case ItemType::Feet:
			return &m_equippedFeet;
		case ItemType::Accessory:
			return &m_equippedAccessory;
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
		updateBackpackDisplay(game);

		return true;
	}
}
