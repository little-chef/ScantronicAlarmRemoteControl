#include "esphome.h"
#include <string>

using namespace esphome;

#define UPDATE_PERIOD_TIME	250
#define LOGTAG	"alarm-control"


class ScantronicAlarmControl: public PollingComponent {
	private:

	public:
		ScantronicAlarmControl() : PollingComponent(UPDATE_PERIOD_TIME) {
		}

		void setup() override {
			ESP_LOGI(LOGTAG, "Setting up Alarm Control.");
		}

        void update() override {
		}
};
