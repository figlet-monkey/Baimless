struct Box
{
	int x = -1, y = -1, w = -1, h = -1;
};

class C_BaseEntity;
class ClientClass;
class Color;
class Vector;
class Drops
{
public:
	void drawammodrop(C_BaseEntity* pEntity, Box box);
	void drawbox(Box box, Color color);
	bool transf(const Vector& in, Vector& out);
	bool createbox(C_BaseEntity* m_entity, Box& box, bool dynamic);
	void box_text(C_BaseEntity* entity, std::string text);
	void drawgrenade(C_BaseEntity* pEntity);
	void drawdrop(C_BaseEntity* pEntity, ClientClass* cClass, DWORD fontf);
	void drawchicken(C_BaseEntity* pEntity, ClientClass* cClass);
};

extern Drops _drops;