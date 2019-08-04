// Functions based off of https://github.com/jesusgollonet/ofpennereasing/tree/master/PennerEasing

#include "TweenEasing.h"
namespace TweenEasing{

	// No Easing, hold the value for the duration
	float easeNone( float b, float c, float t, float d ){ 
		return b+c; 
	}

	float easeLinear( float b, float c, float t, float d ){ 
		return c*t/d+b; 
	}

	float easeBack( float b, float c, float t, float d, EaseType e ){
		float s = 1.70158f;
		// In 
		if( e == EaseType::IN ){
			float postFix = t/=d;
			return c*(postFix)*t*((s+1)*t - s) + b;
		}
		// Out
		if( e == EaseType::OUT ){
			return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b;
		}
		// INOUT
		if( (t/=d/2) < 1 ) 
			return c/2*(t*t*(((s*=(1.525f))+1)*t - s)) + b;
		float postFix = t-=2;
		return c/2*((postFix)*t*(((s*=(1.525f))+1)*t + s) + 2) + b;
	}
	
	float easeSine( float b, float c, float t, float d, EaseType e ){
		// In 
		if( e == EaseType::IN )
			return -c * cos(t/d * (PI/2)) + c + b;
		// Out
		if( e == EaseType::OUT )
			return c * sin(t/d * (PI/2)) + b;	
		// INOUT
		return -c/2 * (cos(PI*t/d) - 1) + b;
	}

	float easeQuad( float b, float c, float t, float d, EaseType e ){
		if( e == EaseType::IN )
			return c*(t/=d)*t + b;
		if( e == EaseType::OUT )
			return -c *(t/=d)*(t-2) + b;
		if( (t/=d/2) < 1 )
			return ((c/2)*(t*t)) + b;
		return -c/2 * (((t-2)*(--t)) - 1) + b;
	}

	float easeCubic( float b, float c, float t, float d, EaseType e ){
		if( e == EaseType::IN )
			return c*(t/=d)*t*t + b;
		if( e == EaseType::OUT )
			return c*((t=t/d-1)*t*t + 1) + b;
		if( (t/=d/2) < 1 )
			return c/2*t*t*t + b;
		return c/2*((t-=2)*t*t + 2) + b;
	}

	float easeQuart( float b, float c, float t, float d, EaseType e ){
		if( e == EaseType::IN )
			return c*(t/=d)*t*t*t + b;
		if( e == EaseType::OUT )
			return -c * ((t=t/d-1)*t*t*t - 1) + b;
		if( (t/=d/2) < 1 )
			return c/2*t*t*t*t + b;
		return -c/2 * ((t-=2)*t*t*t - 2) + b;
	}

	float easeQuint( float b, float c, float t, float d, EaseType e ){
		if( e == EaseType::IN )
			return c*(t/=d)*t*t*t*t + b;
		if( e == EaseType::OUT )
			return c*((t=t/d-1)*t*t*t*t + 1) + b;
		if( (t/=d/2) < 1 )
			return c/2*t*t*t*t*t + b;
		return c/2*((t-=2)*t*t*t*t + 2) + b;
	}

	// Helpers
	float easeBounceOut( float b, float c, float t, float d ){
		float out = 0.0;
		if( (t/=d) < (1/2.75f) )
			return c*(7.5625f*t*t) + b;
		else if( t < (2/2.75f) ){

			float postFix = t-=(1.5f/2.75f);
			return c*(7.5625f*(postFix)*t + .75f) + b;

		} 
		else if( t < (2.5/2.75) ){

			float postFix = t-=(2.25f/2.75f);
			return c*(7.5625f*(postFix)*t + .9375f) + b;

		} 
		float postFix = t-=(2.625f/2.75f);
		return c*(7.5625f*(postFix)*t + .984375f) + b;
	}
	float easeBounceIn( float b, float c, float t, float d ){
		return c - easeBounceOut (d-t, 0, c, d) + b;
	}
	float easeBounce( float b, float c, float t, float d, EaseType e ){

		if( e == EaseType::IN )
			return c - easeBounceOut(d-t, 0, c, d) + b;
		if( e == EaseType::OUT )
			return easeBounceOut(b,c,t,d);
		if( t < d/2 )
			return easeBounceIn(t*2, 0, c, d) * .5f + b;
		return easeBounceOut(t*2-d, 0, c, d) * .5f + c*.5f + b;

	}

	float easeCircular( float b, float c, float t, float d, EaseType e ){
		if( e == EaseType::IN )
			return -c * (sqrt(1 - (t/=d)*t) - 1) + b;
		if( e == EaseType::OUT )
			return c * sqrt(1 - (t=t/d-1)*t) + b;
		if( (t/=d/2) < 1 )
			return -c/2 * (sqrt(1 - t*t) - 1) + b;
		return c/2 * (sqrt(1 - t*(t-=2)) + 1) + b;
	}
	float easeElastic( float b, float c, float t, float d, EaseType e ){
		if( e == EaseType::IN ){
			if( t==0 )
				return b;  
			if( (t/=d)==1 )
				return b+c;  
			float p=d*.3f;
			float a=c; 
			float s=p/4;
			float postFix =a*pow(2,10*(t-=1)); // this is a fix, again, with post-increment operators
			return -(postFix * sin((t*d-s)*(2*PI)/p )) + b;
		}
		if( e == EaseType::OUT ){
			if( t==0 )
				return b;
			if( (t/=d)==1 )
				return b+c;  
			float p=d*.3f;
			float a=c; 
			float s=p/4;
			return (a*pow(2,-10*t) * sin( (t*d-s)*(2*PI)/p ) + c + b);	
		}
		if( t==0 )
			return b;  
		if( (t/=d/2)==2 )
			return b+c; 
		float p=d*(.3f*1.5f);
		float a=c; 
		float s=p/4;
		if( t < 1 ) {
			float postFix =a*pow(2,10*(t-=1)); // postIncrement is evil
			return -.5f*(postFix* sin( (t*d-s)*(2*PI)/p )) + b;
		} 
		float postFix = a*pow(2,-10*(t-=1)); // postIncrement is evil
		return postFix * sin( (t*d-s)*(2*PI)/p )*.5f + c + b;
	}
	float easeExponential( float b, float c, float t, float d, EaseType e ){
		if( e == EaseType::IN )
			return (t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b;
		if( e == EaseType::OUT )
			return (t==d) ? b+c : c * (-pow(2, -10 * t/d) + 1) + b;	
		if( t==0 )
			return b;
		if( t==d )
			return b+c;
		if( (t/=d/2) < 1 )
			return c/2 * pow(2, 10 * (t - 1)) + b;
		return c/2 * (-pow(2, -10 * --t) + 2) + b;
	}

};

