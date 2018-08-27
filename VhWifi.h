/*
	VibHub wifi
*/
#ifndef VhWifi_h
#define VhWifi_h

#include "Configuration.h"
#include "WiFiManager.h"
using namespace std::placeholders;
// Offload the constant CSS and JS to be injected into each page into program memory
// CSS Style shared on each page

// Todo: Improve CSS
const char CSS_SHARED[] PROGMEM = "<style>"
	"body{ "
		"margin:0;"
		"font-size:3vmax;"
		"line-height:100%;"
		"text-align:center;"
		"font-family:verdana, arial;"
		"background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAIAAAACUFjqAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAADFJREFUeNpifPTo0du3bxlgQFhYGJnLhEcOyGXCIwfkMuGRAxmORw4kjUdugJ0GEGAATr8+gQ2rX7QAAAAASUVORK5CYII=);"
	"}"
	"div.wrap{"
		"padding:2vmax;"
		"box-shadow:2px 2px 15px rgba(0,0,0,.5);"
		"background:rgba(255,255,255,.75);"
		"border: 2px solid #9AFAFF;"
		"border-radius:1vmax;"
		"margin-top:1vmax;"
		"width:50%;"
		"min-width:300px;"
		"margin-bottom:5vmax;"
	"}"
	"div, span{ position:relative; }"
	"h1, h3{"
    	"font-size:5vmax;"
        "margin:0 0 1vmax 0;"
		"color:#178;"
    "}"
	"form:not([action=wifisave]){"
		"text-align:center;"
	"}"
	"p{"
		"font-size:3vmax;"
		"margin: 0 0 1vmax 0;"
		"line-height:110%;"
	"}"
	"p.subtitle{"
		"margin:-0.5vmax 0 1vmax 0;"
		"font-style: italic;"
		"color:#666;"
		"font-size:2vmax;"
		"line-height:110%;"
	"}"
    "h3{"
        "font-size:4vmax;"
    "}"
	"input{"
		"border-radius:0.5vmax;"
		"padding:0.5vmax;"
	"}"
	"button, div.wifiNode{"
        "box-shadow:0 0 3vmax rgba(0,0,0,.5);"
        "border-radius:1vmax;"
        "background:linear-gradient(to bottom, #edfeff 0%,#d0fdff 100%);"
        "padding:2vmax 1vmax;"
        "font-size:2vmax;"
        "margin:1vmax 0 0 0;" 
        "color:#000;"
		"cursor:pointer;"
		"border:0.5vmax solid #DDD;"
        "transition: transform 0.25s ease-out, box-shadow 0.25s ease-out;"
		"outline:none;"
		"text-align: center;"
		"width:90%;"
		"box-sizing:border-box;"
    "}"
	"div.wifiNode{width:100%;text-align:left;}"
	"div.wifiNode.p75{background:linear-gradient(to bottom,#f2ffed 0%,#deffd1 100%);}"
	"div.wifiNode.p50{background:linear-gradient(to bottom,#f6ffed 0%,#e9ffd1 100%);}"
	"div.wifiNode.p25{background:linear-gradient(to bottom,#fcffed 0%,#f8ffd1 100%);}"
	"div.wifiNode.p0{background: linear-gradient(to bottom,#fff1ed 0%,#ffdbd1 100%);}"
	"button:hover, div.wifiNode:hover{"
    	"transition:none;"
        "border-color: #FFF;"
    "}"
    "button:active, div.wifiNode:active{"
     	"transform:scale(0.98,0.98);"
        "box-shadow:0 0 1vmax rgba(0,0,0,.75);"
    "}"
	"button.setup{"
        "background:linear-gradient(to bottom, #eeffed 0%,#d3ffd1 100%);"
        "font-size:4vmax;"
        "font-weight:bold;"
		"width:100%;"
    "}"
	".q{"
		"padding-left:3vmax;"
		"position: absolute;"
		"right: 1vmax;"
		"top: 50%;"
		"transform: translateY(-50%);"
		"height: auto;"
	"}"
	"div.wifiNode a{"
		"font-weight:normal;"
	"}"
	"div.wifiNode svg{"
		"height:2.5vmax;" 
		"width:2.5vmax;" 
		"position:absolute;" 
		"left:0;" 
		"top:0.5vmax;"
	"}"
	"label{"
		"font-size:1.5vmax;"
		"color:#666;"
		"font-style:italic;"
		"display:inline-block;"
		"margin-bottom:-1vmax;"
	"}"
	"a.reset{font-style:normal;color:#000;}"
	"div #qr{"
		"text-align:left;"
	"}"
	"div #qr svg{"
		"width:40%;"
		"max-width:300px;"
	"}"
	"div.right{"
		"float:right;"
		"text-align:right;"
	"}"
	"div.bottomRight{"
		"position:absolute;"
		"bottom:0; right:0;"
		"text-align:right;"
	"}"
	"p.devID{"
		"background:#FFD;"
		"padding:1vmax;"
		"border:0.5vmax dotted #FFF;"
		"border-radius:1vmax;"
		"box-shadow:0 0 3vmax rgba(0,0,0,.5);"
		"font-family:\"Courier New\", Courier, monospace;"
	"}"
	"#qr svg, #qr p.devID{"
		"cursor:pointer;"
	"}"
	"#qr svg:active, #qr p.devID:active{"
		"transform:scale(0.97);"
		"box-shadow:0 0 1vmax rgba(0,0,0,.5);"
	"}"
	"#cNote{"
		"position:fixed;"
		"top:5vmax;"
		"left:50%;"
		"transform:translateX(-50%);"
		"padding:1vmax;"
		"border-radius:1vmax;"
		"border:0.5vmax solid #EFE;"
		"box-shadow:0.5vmax 0.5vmax 3vmax rgba(0,0,0,.4);"
		"background:linear-gradient(to bottom, #eeffed 0%,#d3ffd1 100%);"
		"pointer-events:none;"
		"opacity:0;"
	"}"
	"@keyframes cNote{"
		"0%{opacity:1;}"
		"50%{opacity:1;}"
		"100%{opacity:0;}"
	"}"
	"#cNote.show{"
		"animation:cNote 3s ease-in-out;"
	"}"
	".q.l:after,.q.l:before{display:none;}"
	"div.info h3{"
        "font-size:2vmax;"
		"margin:0;"
    "}"
	"p.qa, div.info table{"
		"font-size:1.5vmax;"
    	"margin:0 0 3vmax 3vmax;"
    	"line-height:110%;"
		"color:#666;"
		"display:inline-block;"
	"}"
	"div.info table tr > *{"
		"padding:0.5vmax;"
	"}"
"</style>";

const char JS_SHARED[] PROGMEM = "\n"
    "const path = document.location.pathname;\n"
	// Clear default style
	"document.querySelector('body > div').style = '';\n"
	"document.title = 'VibHub Settings';\n"
	"document.querySelectorAll('div.msg, br').forEach(el => el.remove());"
	// HOME
	"if(path === '/'){\n"
		// Remove the unnecessary H3
		"if(document.querySelector('h3'))"
			"document.querySelector('h3').remove();\n"
		// Header can be the same
		"if(document.querySelector('h1'))"
			"document.querySelector('h1').remove();\n"
		"if(document.querySelector('button')){"
			"document.querySelector('button').className = 'setup';\n"
			"document.querySelector(\"form:nth-of-type(1) button\").innerText = \"Setup\";\n"
			"document.querySelector(\"form:nth-of-type(2) button\").innerText = \"Guide\";"
		"}"
		// Draw the QR code
		"let qr = '<svg viewbox=\"0 0 '+QR_SIZE+' '+QR_SIZE+'\" shape-rendering=\"optimizeSpeed\"><rect width='+QR_SIZE+' height='+QR_SIZE+' style=\"fill:#FFF\"></rect>';"
		"for(let y =0; y<QR_SIZE; ++y){"
			"for(let x=0; x<QR_SIZE; ++x){"
				"if(+QR_DATA.substr(y*QR_SIZE+x,1))"
					"qr += '<rect x='+x+' y='+y+' width=1 height=1 style=\"fill:#000;\" />';"
			"}"
		"}"
		"qr+= '</svg>';"
		"let right = '<div class=\"right\">"
			"<h1>VibHub</h1>"
			"<p class=\"subtitle\">Version <span class=\"VH_VERSION\"></span></p>"
		"</div>'"
		"+'<div class=\"bottomRight\">"
			"<p class=\"subtitle\">Device ID:</p>"
			"<p class=\"devID VH_DEV_ID\"></p>"
		"</div>';"
		"if(document.querySelector('div'))"
			"document.querySelector('div').innerHTML = "
				"'<div id=\"qr\">'+right+qr+'</div>'+"
				"document.querySelector('div').innerHTML+"
				"'<div id=\"cNote\">Device ID copied</div>'"
			";"
		// Bind events
		"document.querySelectorAll('p.devID, #qr svg').forEach(el => {"
			"el.onclick = () => {"
				"document.getElementById('cNote').className = '';"
				"setTimeout(()=>{ document.getElementById('cNote').className = 'show'; }, 10);"
				"if(document.queryCommandSupported && document.queryCommandSupported('copy')){"
					"let ta = document.createElement('textarea');"
					"ta.textContent = document.querySelector('p.devID').innerText;"
					"ta.style.position = 'fixed';"
					"document.body.appendChild(ta);"
					"ta.select();"
					"try{"
						"return document.execCommand('copy');"
					"}catch(ex){"
						"alert('Unable to copy. Browser not supported.');"
						"return false;"
					"}finally{"
						"document.body.removeChild(ta);"
					"}"
				"}"
			"};"
		"});"
	"}\n"

	// Wifi page
	"if(path === '/wifi' || path === '/0wifi'){\n"
		// Adds wifiNode class to all wifiNodes
		"Array.from(document.querySelectorAll('body > div > *')).some(el => {\n"
			"if(el.nodeName !== 'DIV')return true;\n"
			"el.className = 'wifiNode';\n"
		"});\n"
		// Replace the default padlocks that have white backgrounds with transparent ones
		"document.querySelectorAll('body > div > div > div.q.l').forEach(el => {\n"
			"el.innerHTML = '<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 512 512\"><g class=\"\" transform=\"translate(0,0)\" style=\"touch-action: none;\"><path d=\"M254.28 17.313c-81.048 0-146.624 65.484-146.624 146.406V236h49.594v-69.094c0-53.658 43.47-97.187 97.03-97.187 53.563 0 97.032 44.744 97.032 97.186V236h49.594v-72.28c0-78.856-65.717-146.407-146.625-146.407zM85.157 254.688c-14.61 22.827-22.844 49.148-22.844 76.78 0 88.358 84.97 161.5 191.97 161.5 106.998 0 191.968-73.142 191.968-161.5 0-27.635-8.26-53.95-22.875-76.78H85.155zM254 278.625c22.34 0 40.875 17.94 40.875 40.28 0 16.756-10.6 31.23-25.125 37.376l32.72 98.126h-96.376l32.125-98.125c-14.526-6.145-24.532-20.62-24.532-37.374 0-22.338 17.972-40.28 40.312-40.28z\" fill=\"#000000\" fill-opacity=\"1\"></path></g></svg>'+el.title;\n"
			"let colors = [75,50,25,0],"
				"n = +el.title.substr(0, el.title.length-1)"
			";"
			"for(let c of colors){"
				"if(n>=c){el.parentNode.classList.add('p'+c);break;}"
			"}"
		"});\n"
		// Make the port a number
		"if(document.getElementById('port')){"
			"document.getElementById('port').type = 'number';\n"
			"document.getElementById('port').min = 0;\n"
			"document.getElementById('port').max = 65535;\n"
		"}"
		// Update the save button
		"if(document.querySelector('button[type=submit]')){"
			"document.querySelector('button[type=submit]').className='setup';\n"
			"document.querySelector(\"button[type=submit]\").innerText=\"Save\"\n"
		"}"
		// Replace the re-scan button with a back button, since re-scan will wipe any changes made to settings
		"if(document.querySelector(\"div.c\"))"
			"document.querySelector(\"div.c\").remove();\n"
		"let nodes = document.querySelectorAll('body > div > br');\n"
		"if(nodes.length)"
			"nodes[nodes.length-1].remove();\n"
		"document.querySelector('body > div').innerHTML += '<form action=\"/\"><button>Cancel</button></form>';\n"

		// Event bindings
		// Reset buttons
		"if(document.getElementById('resetHost')){"
			"document.getElementById('resetHost').onclick = function(){ this.parentNode.nextSibling.value='vibhub.io'; }\n"
			"document.getElementById('resetPort').onclick = function(){ this.parentNode.nextSibling.value='80'; }\n"
		"}"
		"document.querySelectorAll('div.wifiNode').forEach(el => {\n"
			"el.onclick = function(){ c(this.querySelector('a')); window.location.href = '#p'; };\n"
		"});\n"

		
	"}\n"

	// Help page
	"if(path === '/info'){"
		"document.querySelector('div.wrap').innerHTML = \"<div class='info'>"
			"<h3>How do I update the device?</h3>"
			"<p class='qa'>TODO: WRITE THIS</p>"
			"<h3>What's a device ID?</h3>"
			"<p class='qa'>A device ID is a unique series of numbers and letters that work as a passphrase to give an app access to your VibHub device. Your device ID is <em class='VH_DEV_ID'></em> and can also be found on the main page.</p>"
			"<h3>Can I reset the device ID?</h3>"
			"<p class='qa'>You can reset the device ID by performing a factory reset. This will also revert any settings to default, and wipe wifi configuration. Perform a factory reset by holding down the button as you power the device.</p>"
			"<h3>Where can I get apps?</h3>"
			"<p class='qa'>Check https://vibhub.io or simply google for <em>VibHub App</em>.</p>"
			"<h3>What does the main LED color mean?</h3>"
			"<table><tr><th width='30%'>Color</th><th>Explanation</th></tr>"
				"<tr><td>BLUE / CYAN</td><td>The device is booting</td></tr>"
				"<tr><td>Blinking BLUE</td><td>Device has entered configuration mode and can be connected to through WiFi.</td></tr>"
				"<tr><td>Blinking RED</td><td>WiFi error. Hold the button for 5 seconds and enter config mode to update network settings.</td></tr>"
				"<tr><td>YELLOW</td><td>Socket connection lost. Make sure the device is within range of a WiFi network.</td></tr>"
				"<tr><td>GREEN</td><td>All clear, device can now be controlled via an app.</td></tr>"
				"<tr><td>TODO: UPDATE</td><td>TODO: UPDATE</td></tr>"
			"</table>"
			"<h3>I wanna help development!</h3>"
			"<p class='qa'>Yay! Check out https://github.com/JasXSL/VibHub-ESP32 or visit https://vibhub.io for more info!</p>"
			"<form action='/'><button>Back</button></form>"
		"</div>\";"
	"}"

	// Credentials saved page
	"if(path === '/wifisave'){\n"
		"document.querySelector('body > div').innerHTML = '<h1>Settings Saved</h1><p>Connecting to network. If it fails, connect to the VibHub device again and update the settings.</p>';\n"
	"}\n"
;


class VhWifi{
	public:
		VhWifi(void) : 
			shouldSaveConfig(false)
		{}
		void connect( bool force=false, bool resetSetting=false );
		void clearSettings();
		void saveConfigCallback();
		void configModeCallback( WiFiManager *myWiFiManager );

	private:
        WiFiManager* _wifiManager;
		bool shouldSaveConfig;			// Checks if config should be saved, default false
		void handleFatalError();		// If something happened that can't be recovered from
		// Any non-constant data needed to be loaded should go in here
		String getCustomJSPre();		// Non constant data that should go above the constant data
		String getCustomJSPost();		// == || == below the constant data
		

};



extern VhWifi vhWifi;

#endif //VhWifi_h