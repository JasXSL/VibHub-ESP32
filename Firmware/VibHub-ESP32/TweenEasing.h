
#ifndef TweenEasing_h
#define TweenEasing_h
#include <arduino.h>

namespace TweenEasing{

	enum class Ease { LINEAR, SINE, QUAD, CUBIC, QUART, QUINT, BACK, BOUNCE, CIRCULAR, ELASTIC, EXPONENTIAL, NONE }; 
	enum class EaseType { IN, OUT, INOUT, NONE };

	extern float easeNone( float b, float c, float t, float d );
	extern float easeLinear( float b, float c, float t, float d );
	extern float easeBack( float b, float c, float t, float d, EaseType e );
	extern float easeSine( float b, float c, float t, float d, EaseType e );
	extern float easeQuad( float b, float c, float t, float d, EaseType e );
	extern float easeCubic( float b, float c, float t, float d, EaseType e );
	extern float easeQuart( float b, float c, float t, float d, EaseType e );
	extern float easeQuint( float b, float c, float t, float d, EaseType e );
	extern float easeBounce( float b, float c, float t, float d, EaseType e );
	extern float easeCircular( float b, float c, float t, float d, EaseType e );
	extern float easeElastic( float b, float c, float t, float d, EaseType e );
	extern float easeExponential( float b, float c, float t, float d, EaseType e );
	

};



#endif