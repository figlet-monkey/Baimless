#include "SDK/IClientMode.h"



extern void Glovechanger(ClientFrameStage_t stage);
extern void Fullupdate();

extern void Skinchanger();
class c_glovechanger {
public:
	bool apply_glove_model(C_BaseAttributableItem* glove, const char* model) noexcept;
	bool apply_glove_skin(C_BaseAttributableItem* glove, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear) noexcept;
	void run() noexcept;
};
extern c_glovechanger glove_changer;