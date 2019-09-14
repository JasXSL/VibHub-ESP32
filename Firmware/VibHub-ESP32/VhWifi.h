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

const char CSS_SHARED[] PROGMEM = "<style>"
	"body{ "
		"margin:0;"
		"line-height:100%;"
		"text-align:center;"
		"font-family:verdana, arial;"
		"background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAIAAAACUFjqAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAADFJREFUeNpifPTo0du3bxlgQFhYGJnLhEcOyGXCIwfkMuGRAxmORw4kjUdugJ0GEGAATr8+gQ2rX7QAAAAASUVORK5CYII=);"
	"}"
	"div.wrap{"
		"position:absolute;"
		"padding:1em;"
		"box-shadow:2px 2px 15px rgba(0,0,0,.25);"
		"background:rgba(255,255,255,.75);"
		"border: 2px solid #9AFAFF;"
		"border-radius:1em;"
		"margin:1vh 0 5vh 0;"
		"left:2vw; right:2vw;"
		"max-width:800px;"
		"font-size:1em;"
	"}"
	"div, span{ position:relative; }"
	"h1, h3{"
    	"font-size:3em;"
        "margin:0.5em 0;"
		"color:#178;"
    "}"
	"form:not([action=wifisave]){"
		"text-align:center;"
	"}"
	"p{"
		"font-size:1em;"
		"margin: 0 0 0.25em 0;"
		"line-height:110%;"
	"}"
	"p.subtitle{"
		"margin:-0.5em 0 1em 0;"
		"font-style: italic;"
		"color:#666;"
		"font-size:1em;"
	"}"
	"p.devID{"
		"font-size:2em;"
		"background:#FFD;"
		"padding:0.5em;"
		"border:0.2em dotted #FFF;"
		"border-radius:0.5em;"
		"box-shadow:0 0 1em rgba(0,0,0,.25);"
		"font-family:\"Courier New\", Courier, monospace;"
		"text-align:center;"
	"}"
    "h3{"
        "font-size:1em;"
    "}"
	"input:not([type=radio]){"
		"border-radius:0.1em;"
		"padding:0.5em;"
		"margin-bottom:1em;"
	"}"
	
	// Note that shows when you click to copy the ID
	"#cNote{"
		"position:fixed;"
		"top:5vh;"
		"left:50%;"
		"transform:translateX(-50%);"
		"padding:1em;"
		"border-radius:1em;"
		"border:0.25em solid #EFE;"
		"box-shadow:0.5em 0.5em 1em rgba(0,0,0,.4);"
		"background:linear-gradient(to bottom, #eeffed 0%,#d3ffd1 100%);"
		"pointer-events:none;"
		"opacity:0;"
	"}"
	"#cNote:active{"
		"transform:scale(0.95,0.95)"
	"}"
	"@keyframes cNote{"
		"0%{opacity:1;}"
		"50%{opacity:1;}"
		"100%{opacity:0;}"
	"}"
	"#cNote.show{"
		"animation:cNote 3s ease-in-out;"
	"}"

	"button, div.wifiNode{"
        "box-shadow:0 0 0.3em rgba(0,0,0,.25);"
        "border-radius:0.5em;"
        "background:linear-gradient(to bottom, #edfeff 0%,#d0fdff 100%);"
        "padding:0.5em;"
        "font-size:1.5em;"
        "margin:0.5em 0 0 0;" 
        "color:#000;"
		"border:0.1em solid #DDD;"
        "transition:transform 0.25s ease-out, box-shadow 0.25s ease-out;"
		"outline:none;"
		"text-align: center;"
		"width:90%;"
		"box-sizing:border-box;"
    "}"
	"button:active, div.wifiNode:active{"
     	"transform:scale(0.98,0.98);"
        "box-shadow:0 0 0.1em rgba(0,0,0,.75);"
    "}"
	"button.setup{"
        "background:linear-gradient(to bottom, #eeffed 0%,#d3ffd1 100%);"
        "font-size:2em;"
        "font-weight:bold;"
		"width:100%;"
    "}"
	"div.wifiNode{"
		"width:100%;"
		"text-align:left;"
		"margin:0.5em 0;"
	"}"
	"button:hover, div.wifiNode:hover{"
    	"transition:none;"
        "border-color: #FFF;"
    "}"
    
	
	".q{"
		"padding-left:1em;"
		"position: absolute;"
		"right: 1em;"
		"top: 50%;"
		"transform: translateY(-50%);"
		"height: auto;"
	"}"
	"div.wifiNode a{"
		"font-weight:normal;"
	"}"
	"div.wifiNode svg{"
		"height:1em;" 
		"width:1em;" 
		"position:absolute;" 
		"left:0;" 
		"top:-0.2em;"
	"}"
	"label{"
		"font-size:1.5em;"
		"color:#666;"
		"font-style:italic;"
		"display:inline-block;"
		"margin-bottom:-1em;"
	"}"
	//"a.reset{font-style:normal;color:#000;}"
	".q.l:after,.q.l:before{display:none;}"

"</style>";

const char JS_SHARED[] PROGMEM = "\n"
    "let path = document.location.pathname;\n"
	"let QSA=document.querySelectorAll.bind(document);"
	"let QS=document.querySelector.bind(document);"
	"let GID=document.getElementById.bind(document);"
	
	// Clear default style
	"QS('body > div').style = '';\n"
	"document.title = 'VibHub Settings';\n"
	"QSA(\"div.msg, br, form[action='/info']\").forEach(e => e.remove());"
	// HOME
	"if(path==='/'){\n"
		"QSA(\"h3, h1\").forEach(e => e.remove());"
		"let BT=QSA('button'),div=QS('div');"
		"BT[0].className='setup';"
		"BT[0].innerText='Setup';"
		"div.innerHTML='"
			"<h1>VibHub</h1>"
			"<p class=\"subtitle\">Version <span class=\"VH_VERSION\"></span></p>"
			"<p class=\"devID VH_DEV_ID\"></p>"
			"<div id=\"cNote\">Device ID copied</div>"
		"'+div.innerHTML;"

		// Bind events
		"let dID=QS('p.devID');"
		"let cN=GID('cNote');"
		"let qs=document.queryCommandSupported;"
		"qs=qs&&qs.bind(document)('copy');"
		"dID.onclick=()=>{"
			"cN.className='';"
			"setTimeout(()=>{cN.className='show';}, 10);"
			"if(qs){"
				"let ta = document.createElement('textarea');"
				"ta.textContent = QS('p.devID').innerText;"
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
	"}\n"

	// Wifi page
	"if(path==='/wifi'||path==='/0wifi'){\n"
		// Adds wifiNode class to all wifiNodes
		"[...QSA('body > div > *')].map(el=>{\n"
			"if(el.nodeName!=='DIV')return;\n"
			"el.className='wifiNode';\n"
		"});\n"
		// Replace the default padlocks that have white backgrounds with transparent ones
		"QSA('body > div > div > div.q.l').forEach(el => {\n"
			"el.innerHTML = '<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 512 512\"><g class=\"\" transform=\"translate(0,0)\" style=\"touch-action: none;\"><path d=\"M254.28 17.313c-81.048 0-146.624 65.484-146.624 146.406V236h49.594v-69.094c0-53.658 43.47-97.187 97.03-97.187 53.563 0 97.032 44.744 97.032 97.186V236h49.594v-72.28c0-78.856-65.717-146.407-146.625-146.407zM85.157 254.688c-14.61 22.827-22.844 49.148-22.844 76.78 0 88.358 84.97 161.5 191.97 161.5 106.998 0 191.968-73.142 191.968-161.5 0-27.635-8.26-53.95-22.875-76.78H85.155zM254 278.625c22.34 0 40.875 17.94 40.875 40.28 0 16.756-10.6 31.23-25.125 37.376l32.72 98.126h-96.376l32.125-98.125c-14.526-6.145-24.532-20.62-24.532-37.374 0-22.338 17.972-40.28 40.312-40.28z\" fill=\"#000000\" fill-opacity=\"1\"></path></g></svg>'+el.title;\n"
		"});\n"
		// Make the port a number
		"let port=GID('port');"
		"if(port){"
			"port.type = 'number';\n"
			"port.min = 0;\n"
			"port.max = 65535;\n"
		"}\n"
		// Make the sleep timer a number
		"let sl = GID('sleep_after_min');"
		"if(sl){"
			"sl.type = 'number';\n"
			"sl.min = 0;\n"
			"sl.max = 10080;\n"
		"}\n"
		// Update the save button
		"let sub = QS('button[type=submit]');"
		"if(sub){"
			"sub.className='setup';\n"
			"sub.innerText=\"Save\"\n"
		"}"
		// Replace the re-scan button with a back button, since re-scan will wipe any changes made to settings
		"let cDiv = QS(\"div.c\");"
		"if(cDiv)"
			"cDiv.remove();\n"
		"let nodes = QSA('body > div > br');\n"
		"if(nodes.length)"
			"nodes[nodes.length-1].remove();\n"
		"QS('body > div').innerHTML += '<form action=\"/\"><button>Cancel</button></form>';\n"

		// Event bindings
		"QSA('div.wifiNode').forEach(el => {\n"
			"el.onclick = function(){ c(this.querySelector('a')); window.location.href = '#p'; }\n"
		"});\n"

		
	"}\n"

	// Credentials saved page
	"if(path === '/wifisave'){\n"
		"QS('body > div').innerHTML = '<h1>Settings Saved</h1><p>Connecting to network. If it fails, connect to the VibHub device again and update the settings.</p>';\n"
	"}\n"
;


class VhWifi{
	public:
		VhWifi(void) : 
			connected(false)
		{}
		void connect( bool force=false, bool resetSetting=false );
		void clearSettings();
		void saveConfigCallback();
		void configModeCallback( WiFiManager *myWiFiManager );
		bool connected;

	private:
        WiFiManager* _wifiManager;
		void handleFatalError();		// If something happened that can't be recovered from
		// Any non-constant data needed to be loaded should go in here
		String getCustomJSPre();		// Non constant data that should go above the constant data
		String getCustomJSPost();		// == || == below the constant data
		String getParam(String name);	// gets a custom param

};



extern VhWifi vhWifi;

#endif //VhWifi_h