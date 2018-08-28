#ifndef TweenProgram_h
#define TweenProgram_h
#include <vector>
#include "TweenProgramStage.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <FS.h>

class TweenProgram{
	public:
		std::vector<std::unique_ptr<TweenProgramStage>> stages;
		int repeats;			// Total repeats for this program
		float value;			// Current tween value
		bool completed;			// Program completed

		TweenProgram();
		TweenProgram( int re );
		//void addStage(TweenProgramStage st);
		void addStageFromJson(JsonObject &st);
		void start( float initVal = 0 );			// Start the tween and reset everything
		void generateStages();						// Unsed on program start or loop to reload the stages with randobjects etc
		bool loop();			// Updates TweenProgram.value Returns false if the program has completed and no stages are left.
		

	private:
		long _started;			// Time the program started
		long _totalTime; 		// Total time for one iteration of the program
		int _repeats;			// Repeats left until the program ends
};

#endif