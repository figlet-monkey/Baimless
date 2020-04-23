

template<typename T>
class singleton {
protected:
	singleton() { }
	~singleton() { }

	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;

	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;
public:
	static T& get() {
		static T inst{};
		return inst;
	}
};



#include "../../SDK/Vector.h"
#include "../../Utils/Color.h"
#include <vector>
class IGameEvent;

class bullettracers : public singleton< bullettracers > {
private:
	class trace_info {
	public:
		trace_info(Vector starts, Vector positions, float times) {
			this->start = starts;
			this->position = positions;
			this->time = times;
		}

		Vector position;
		Vector start;
		float time;
	};

	std::vector<trace_info> logs;

	void draw_beam(Vector src, Vector end, Color coli);
	void draw_impact(Vector end);

public:
	void events(IGameEvent* event);
};