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
		std::string m_name;
		std::string m_description;
		ItemType m_type;
		const gf::Texture *m_texture = nullptr;
		Stat m_stat;

		Item() = default;

		Item(std::string name, ItemType type, const gf::Texture &tex)
				: m_name(name),
					m_type(type),
					m_texture(&tex)
		{
		}

		void setTexture(gf::Texture &texture) { this->m_texture = &texture; }
		
		static Item generateRandomItem(RogueCMI *game);
	};
}

#endif