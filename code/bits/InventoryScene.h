#ifndef RL_INVENTORY_SCENE_H
#define RL_INVENTORY_SCENE_H

#include <gf/Scene.h>
#include <gf/Font.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include "Inventory.h"

namespace rCMI
{
	class RogueCMI;

	class InventoryScene : public gf::Scene
	{
	public:
		InventoryScene(RogueCMI *game);
		Inventory m_inventory;

	private:
		void doRender(gf::RenderTarget &target, const gf::RenderStates &states) override;
		void doProcessEvent(gf::Event &event) override;

		RogueCMI *m_game = nullptr;
		gf::Font font;
	};
}

#endif // RL_INVENTORY_SCENE_H