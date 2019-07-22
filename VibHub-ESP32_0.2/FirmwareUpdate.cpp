/*
	VibHub Firmware Update
    Based on: https://github.com/nhatuan84/esp32-http-fota
    Would have used it as a library, but required modifications to work with redirects on HTTPS
*/

#include "FirmwareUpdate.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "MD5Builder.h"
#include "Update.h"
#include "UserSettings.h"


const char* base_url = "https://github.com/JasXSL/VibHub-ESP32/releases/download/";

const char* github_ca = \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIEtjCCA56gAwIBAgIQDHmpRLCMEZUgkmFf4msdgzANBgkqhkiG9w0BAQsFADBs\n" \
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
    "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n" \
    "ZSBFViBSb290IENBMB4XDTEzMTAyMjEyMDAwMFoXDTI4MTAyMjEyMDAwMFowdTEL\n" \
    "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n" \
    "LmRpZ2ljZXJ0LmNvbTE0MDIGA1UEAxMrRGlnaUNlcnQgU0hBMiBFeHRlbmRlZCBW\n" \
    "YWxpZGF0aW9uIFNlcnZlciBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC\n" \
    "ggEBANdTpARR+JmmFkhLZyeqk0nQOe0MsLAAh/FnKIaFjI5j2ryxQDji0/XspQUY\n" \
    "uD0+xZkXMuwYjPrxDKZkIYXLBxA0sFKIKx9om9KxjxKws9LniB8f7zh3VFNfgHk/\n" \
    "LhqqqB5LKw2rt2O5Nbd9FLxZS99RStKh4gzikIKHaq7q12TWmFXo/a8aUGxUvBHy\n" \
    "/Urynbt/DvTVvo4WiRJV2MBxNO723C3sxIclho3YIeSwTQyJ3DkmF93215SF2AQh\n" \
    "cJ1vb/9cuhnhRctWVyh+HA1BV6q3uCe7seT6Ku8hI3UarS2bhjWMnHe1c63YlC3k\n" \
    "8wyd7sFOYn4XwHGeLN7x+RAoGTMCAwEAAaOCAUkwggFFMBIGA1UdEwEB/wQIMAYB\n" \
    "Af8CAQAwDgYDVR0PAQH/BAQDAgGGMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEF\n" \
    "BQcDAjA0BggrBgEFBQcBAQQoMCYwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRp\n" \
    "Z2ljZXJ0LmNvbTBLBgNVHR8ERDBCMECgPqA8hjpodHRwOi8vY3JsNC5kaWdpY2Vy\n" \
    "dC5jb20vRGlnaUNlcnRIaWdoQXNzdXJhbmNlRVZSb290Q0EuY3JsMD0GA1UdIAQ2\n" \
    "MDQwMgYEVR0gADAqMCgGCCsGAQUFBwIBFhxodHRwczovL3d3dy5kaWdpY2VydC5j\n" \
    "b20vQ1BTMB0GA1UdDgQWBBQ901Cl1qCt7vNKYApl0yHU+PjWDzAfBgNVHSMEGDAW\n" \
    "gBSxPsNpA/i/RwHUmCYaCALvY2QrwzANBgkqhkiG9w0BAQsFAAOCAQEAnbbQkIbh\n" \
    "hgLtxaDwNBx0wY12zIYKqPBKikLWP8ipTa18CK3mtlC4ohpNiAexKSHc59rGPCHg\n" \
    "4xFJcKx6HQGkyhE6V6t9VypAdP3THYUYUN9XR3WhfVUgLkc3UHKMf4Ib0mKPLQNa\n" \
    "2sPIoc4sUqIAY+tzunHISScjl2SFnjgOrWNoPLpSgVh5oywM395t6zHyuqB8bPEs\n" \
    "1OG9d4Q3A84ytciagRpKkk47RpqF/oOi+Z6Mo8wNXrM9zwR4jxQUezKcxwCmXMS1\n" \
    "oVWNWlZopCJwqjyBcdmdqEU79OX2olHdx3ti6G8MdOu42vi/hw15UJGQmxg7kVkn\n" \
    "8TUoE6smftX3eg==\n" \
    "-----END CERTIFICATE-----" \
;

const char*  aws_ca = \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIEYzCCA0ugAwIBAgIQAYL4CY6i5ia5GjsnhB+5rzANBgkqhkiG9w0BAQsFADBa\n" \
    "MQswCQYDVQQGEwJJRTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJl\n" \
    "clRydXN0MSIwIAYDVQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTE1\n" \
    "MTIwODEyMDUwN1oXDTI1MDUxMDEyMDAwMFowZDELMAkGA1UEBhMCVVMxFTATBgNV\n" \
    "BAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3LmRpZ2ljZXJ0LmNvbTEjMCEG\n" \
    "A1UEAxMaRGlnaUNlcnQgQmFsdGltb3JlIENBLTIgRzIwggEiMA0GCSqGSIb3DQEB\n" \
    "AQUAA4IBDwAwggEKAoIBAQC75wD+AAFz75uI8FwIdfBccHMf/7V6H40II/3HwRM/\n" \
    "sSEGvU3M2y24hxkx3tprDcFd0lHVsF5y1PBm1ITykRhBtQkmsgOWBGmVU/oHTz6+\n" \
    "hjpDK7JZtavRuvRZQHJaZ7bN5lX8CSukmLK/zKkf1L+Hj4Il/UWAqeydjPl0kM8c\n" \
    "+GVQr834RavIL42ONh3e6onNslLZ5QnNNnEr2sbQm8b2pFtbObYfAB8ZpPvTvgzm\n" \
    "+4/dDoDmpOdaxMAvcu6R84Nnyc3KzkqwIIH95HKvCRjnT0LsTSdCTQeg3dUNdfc2\n" \
    "YMwmVJihiDfwg/etKVkgz7sl4dWe5vOuwQHrtQaJ4gqPAgMBAAGjggEZMIIBFTAd\n" \
    "BgNVHQ4EFgQUwBKyKHRoRmfpcCV0GgBFWwZ9XEQwHwYDVR0jBBgwFoAU5Z1ZMIJH\n" \
    "WMys+ghUNoZ7OrUETfAwEgYDVR0TAQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMC\n" \
    "AYYwNAYIKwYBBQUHAQEEKDAmMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdp\n" \
    "Y2VydC5jb20wOgYDVR0fBDMwMTAvoC2gK4YpaHR0cDovL2NybDMuZGlnaWNlcnQu\n" \
    "Y29tL09tbmlyb290MjAyNS5jcmwwPQYDVR0gBDYwNDAyBgRVHSAAMCowKAYIKwYB\n" \
    "BQUHAgEWHGh0dHBzOi8vd3d3LmRpZ2ljZXJ0LmNvbS9DUFMwDQYJKoZIhvcNAQEL\n" \
    "BQADggEBAC/iN2bDGs+RVe4pFPpQEL6ZjeIo8XQWB2k7RDA99blJ9Wg2/rcwjang\n" \
    "B0lCY0ZStWnGm0nyGg9Xxva3vqt1jQ2iqzPkYoVDVKtjlAyjU6DqHeSmpqyVDmV4\n" \
    "7DOMvpQ+2HCr6sfheM4zlbv7LFjgikCmbUHY2Nmz+S8CxRtwa+I6hXsdGLDRS5rB\n" \
    "bxcQKegOw+FUllSlkZUIII1pLJ4vP1C0LuVXH6+kc9KhJLsNkP5FEx2noSnYZgvD\n" \
    "0WyzT7QrhExHkOyL4kGJE7YHRndC/bseF/r/JUuOUFfrjsxOFT+xJd1BDKCcYm1v\n" \
    "upcHi9nzBhDFKdT3uhaQqNBU4UtJx5g=\n" \
    "-----END CERTIFICATE-----" \
;


FWUpdate::FWUpdate() :
    running(false)
{
    
}

FWUpdate::~FWUpdate() {
    
}


void FWUpdate::start(const char* file, const char* md5) {
    
    running = true;

    char* url;
    url = (char*)calloc(strlen(base_url)+strlen(file)+1, sizeof(char));
    strcpy(url, base_url);
    strcat(url, file);
    
    Serial.printf("OTA: Update Started - URL: %s  MD5: %s\n", url, md5);
    
    int downloaded = 0;
    int written = 0;
    int total = 1;
    int len = 1;
    uint8_t buff[1024] = { 0 };
    size_t size = sizeof(buff);
    int ret = 0;
    HTTPClient http;
    
    const char * headerkeys[] = {"Location"} ;
    size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
    http.collectHeaders(headerkeys, headerkeyssize);
    
    http.begin(url, github_ca);
    
    int httpCode = http.GET();

    if(httpCode > 0 && httpCode == HTTP_CODE_FOUND) {
        Serial.printf("OTA: Redirected to: %s\n", http.header("Location").c_str());
        http.end();
        http.begin(http.header("Location").c_str(), aws_ca);
        httpCode = http.GET();
    }
    
    if(httpCode > 0 && httpCode == HTTP_CODE_OK) {
        // TODO: Pre-download calls, if any;
        // get length of document (is -1 when Server sends no Content-Length header)
        len = http.getSize();
        total = len;
        downloaded = 0;
        // get tcp stream
        WiFiClient * stream = http.getStreamPtr();
        if (Update.begin(total, U_FLASH)){
            Update.setMD5(md5);
            downloaded = 0;
            while (!Update.isFinished()) {
                // read all data from server
                if(http.connected() && (len > 0)) {
                  // get available data size
                  size = stream->available();

                    if(size > 0) {
                        // read up to 128 byte
                        int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
                        // write to storage
                        written = Update.write(buff, c);
                        if (written > 0) {
                            if(written != c){
                                Serial.println("OTA: Flashing chunk not full ... warning!");
                            }
                            downloaded += written;
                            Serial.printf("OTA: Progress: %d%%\n", (100*downloaded)/total);
                        } else {
                            Serial.println("OTA: Writing flash ... failed!");
                            ret = -1;
                            break;
                        }

                        if(len > 0) {
                            len -= c;
                        }
                    }
                  delay(1);
                }
            }
        } else {
            Serial.println("OTA: Flashing init ... failed!");
            ret = -1;
        }
    // TODO: Post-download calls, if any;
    }else {
        Serial.printf("OTA: [HTTP] GET... failed! httpCode: %i\n", httpCode);
        ret -1;
    }

    http.end();
    if(downloaded == total && len == 0){
        if(Update.end()){
            Serial.println("OTA: Flashing ... done!");
            delay(100);
            ESP.restart();                
        } else {
            Serial.println("OTA: Flashing md5 ... failed!"); 
            ret = -1;
        }
    } else {
        Serial.println("OTA: Download firmware ... failed!");
        ret = -1;
    }
    
    running = false;
    
    Serial.println("OTA: End");
}

FWUpdate fwUpdate = FWUpdate();
