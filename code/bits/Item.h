#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <gf/Texture.h>
#include "Stat.h"

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
		Misc
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

		Item() = default;

		Item(std::string id, ItemType type, const gf::Texture &tex, std::string name)
				: m_id(id),
					m_type(type),
					m_texture(&tex),
					m_name(name)
		{
		}

		void setTexture(gf::Texture &texture) { this->m_texture = &texture; }
		
		static Item generateRandomItem(RogueCMI *game);
	};
}

#endif