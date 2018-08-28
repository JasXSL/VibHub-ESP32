#include "TweenProgram.h"
#include "TweenProgramStage.h"
#include <FS.h>

// Constructor
TweenProgram::TweenProgram() :
	repeats(0)
{}
TweenProgram::TweenProgram( int re ){
	repeats = _repeats = re;
}
		
void TweenProgram::addStageFromJson(JsonObject &st){

	stages.push_back(
		std::unique_ptr<TweenProgramStage>(new TweenProgramStage(st))
	);

}


// Start the tween
void TweenProgram::start( float initVal ){

	// Reset stuff
	completed = false;
	_repeats = repeats;
	value = initVal;
	generateStages();

}

// Reshuffles the program stages
void TweenProgram::generateStages(){

	long duration = 0;
	stages[0]->inValue = value;
	for( int i=0; i<stages.size(); ++i ){
		auto stage = stages[i].get();
		if( i )
			stage->inValue = stages[i-1]->outValue();
		stage->reset();
		//Serial.printf("Added stage with intens %i, dur %i \n", stage->intensity, stage->duration);
		duration += stage->getDuration();
	}
	_totalTime = duration;
	_started = millis();

}

// Updates TweenProgram.value Returns false if the program has completed and no stages are left.
bool TweenProgram::loop(){
	
	bool pre = completed;
	if( !completed ){

		long delta = millis()-_started;
		// Program has ended
		if( delta >= _totalTime ){

			value = stages.back().get()->outValue();	// makes sure the new inValue is proper
			// There are repeats left
			if( repeats == -1 || _repeats ){

				// Reset the stages
				generateStages();

				// Remove from repeat ticker unless infinite
				if( repeats != -1 )
					--_repeats;

			}
			else
				completed = true;

		}

		
		// Iterate the stages and find the active one
		long tot = 0;			// Holds total time for stages, Tracks which program stage we're on
		// Value the tween is starting from
		for( int i = 0; i<stages.size(); ++i ){
			
			auto stage = stages[i].get();
			long dur = stage->getDuration();
			// The active stage or if it's the last element
			if( tot+dur >= delta || i == stages.size()-1 ){

				value = stage->getValueAtDelta(delta-tot);	// Delta here is relative to the program
				break;

			}
			tot+= dur;

		}

	}

	return !pre;

}			
