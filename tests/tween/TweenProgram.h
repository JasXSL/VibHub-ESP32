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
		void addStageFromJson(JsonObject &st);
		void start();								// Resets loops, generates randobjects, and starts the tween
		void generateStages();						// Unsed on program start or loop to reload the stages with randobjects etc
		void reset( int rep );						// Resets the stages and sets repeats, lets you keep the program without destroying it
		bool loop();			// Updates TweenProgram.value Returns false if the program has completed and no stages are left.
		

	private:
		long _started;			// Time the program started
		long _totalTime; 		// Total time for one iteration of the program
		int _repeats;			// Repeats left until the program ends
};

#endif