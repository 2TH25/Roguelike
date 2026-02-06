#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <gf/Texture.h>
#include "Stat.h"
#include <gf/Color.h>

namespace rCMI
{

	class RogueCMI;

	enum class ItemType
	{
		Head,
		Torso,
		Legs,
		Hand,
		Boots,
		Accessory,
		Consumable,
		Weapon,
		Misc,
		Bow,
		None
	};

	class Item
	{
	public:
		std::string m_id;
		std::string m_name;
		std::string m_description;
		ItemType m_type;
		const gf::Texture *m_texture = nullptr;
		Stat m_stat;
		int m_count = 1;

		enum class Rarity { Common, Uncommon, Rare, Epic, Legendary };

		Rarity m_rarity;

		Item();

		Item(std::string name, std::string id, ItemType type, Rarity rarity, gf::Texture& tex, std::string desc, Stat s)
				: m_id(id),
					m_type(type),
					m_texture(&tex),
					m_name(name), 
					m_rarity(rarity),
					m_description(desc),
					m_stat(s)

		{
		}
		float getChance();

		void setTexture(gf::Texture &texture) { this->m_texture = &texture; }
		
		static Item generateRandomItem(RogueCMI *game);
	};
}

#endif