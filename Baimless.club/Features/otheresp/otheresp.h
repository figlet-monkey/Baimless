template<typename T>
class singletonanother {
protected:
	singletonanother() { }
	~singletonanother() { }

	singletonanother(const singletonanother&) = delete;
	singletonanother& operator=(const singletonanother&) = delete;

	singletonanother(singletonanother&&) = delete;
	singletonanother& operator=(singletonanother&&) = delete;
public:
	static T& get() {
		static T inst{};
		return inst;
	}
};

#include "../../SDK/Vector.h"
#include "../../Utils/Color.h"
#include <vector>
class IClientEntity;
class IGameEvent;

struct CUSTOMVERTEX {
	FLOAT x, y, z;
	FLOAT rhw;
	DWORD color;
};


class IDirect3DDevice9;
class otheresp : public singletonanother< otheresp > {
public:
	void hitmarker_paint();
	void hitmarkerdynamic_paint();

	void hitmarker_event(IGameEvent * event);
	void spread_crosshair(IDirect3DDevice9*);
private:
	float hurt_time;
};

struct impact_info {
	float x, y, z;
	long long time;
};

struct hitmarker_info {
	impact_info impact;
	int alpha;
};

extern std::vector<impact_info> impacts;
extern std::vector<hitmarker_info> hitmarkers;
