enum ACT
{
	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP,
};

//shonax wtf
template<typename T>
class singletonxd1 {
protected:
	singletonxd1() { }
	~singletonxd1() { }

	singletonxd1(const singletonxd1&) = delete;
	singletonxd1& operator=(const singletonxd1&) = delete;

	singletonxd1(singletonxd1&&) = delete;
	singletonxd1& operator=(singletonxd1&&) = delete;
public:
	static T& get() {
		static T inst{};
		return inst;
	}
};

#include "SDK/Vector.h"
#include "Utils/Color.h"
#include <vector>
class C_Trace;
class CViewSetup;
class GrenadePrediction : public singletonxd1<GrenadePrediction>
{
private:
	std::vector<Vector> path;
	std::vector<std::pair<Vector, QAngle>> others;
	//std::vector<Vector> others;
	int type = 0;
	int act = 0;

	void Setup(Vector& vecSrc, Vector& vecThrow, Vector viewangles);
	void Simulate(CViewSetup* setup);

	int Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval);
	bool CheckDetonate(const Vector& vecThrow, const C_Trace& tr, int tick, float interval);

	void TraceHull(Vector& src, Vector& end, C_Trace& tr);
	void AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground);
	void PushEntity(Vector& src, const Vector& move, C_Trace& tr);
	void ResolveFlyCollisionCustom(C_Trace& tr, Vector& vecVelocity, float interval);
	int PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce);
public:
	void Tick(int buttons);
	void View(CViewSetup* setup);
	void Paint();
};


