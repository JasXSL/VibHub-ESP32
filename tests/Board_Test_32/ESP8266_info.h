
const char * const RST_REASONS[] =
{
    "REASON_DEFAULT_RST",
    "REASON_WDT_RST",
    "REASON_EXCEPTION_RST",
    "REASON_SOFT_WDT_RST",
    "REASON_SOFT_RESTART",
    "REASON_DEEP_SLEEP_AWAKE",
    "REASON_EXT_SYS_RST"
};

const char * const FLASH_SIZE_MAP_NAMES[] =
{
    "FLASH_SIZE_4M_MAP_256_256",
    "FLASH_SIZE_2M",
    "FLASH_SIZE_8M_MAP_512_512",
    "FLASH_SIZE_16M_MAP_512_512",
    "FLASH_SIZE_32M_MAP_512_512",
    "FLASH_SIZE_16M_MAP_1024_1024",
    "FLASH_SIZE_32M_MAP_1024_1024"
};

const char * const OP_MODE_NAMES[] 
{
    "NULL_MODE",
    "STATION_MODE",
    "SOFTAP_MODE",
    "STATIONAP_MODE"
};

const char * const AUTH_MODE_NAMES[] 
{
    "AUTH_OPEN",
    "AUTH_WEP",
    "AUTH_WPA_PSK",
    "AUTH_WPA2_PSK",
    "AUTH_WPA_WPA2_PSK",
    "AUTH_MAX"
};

const char * const PHY_MODE_NAMES[]
{
    "",
    "PHY_MODE_11B",
    "PHY_MODE_11G",
    "PHY_MODE_11N"
};

const char * const EVENT_NAMES[]
{
    "EVENT_STAMODE_CONNECTED",
    "EVENT_STAMODE_DISCONNECTED",
    "EVENT_STAMODE_AUTHMODE_CHANGE",
    "EVENT_STAMODE_GOT_IP",
    "EVENT_SOFTAPMODE_STACONNECTED",
    "EVENT_SOFTAPMODE_STADISCONNECTED",
    "EVENT_MAX"
};

const char * const EVENT_REASONS[]
{
    "",
    "REASON_UNSPECIFIED",
    "REASON_AUTH_EXPIRE",
    "REASON_AUTH_LEAVE",
    "REASON_ASSOC_EXPIRE",
    "REASON_ASSOC_TOOMANY",
    "REASON_NOT_AUTHED",
    "REASON_NOT_ASSOCED",
    "REASON_ASSOC_LEAVE",
    "REASON_ASSOC_NOT_AUTHED",
    "REASON_DISASSOC_PWRCAP_BAD",
    "REASON_DISASSOC_SUPCHAN_BAD",
    "REASON_IE_INVALID",
    "REASON_MIC_FAILURE",
    "REASON_4WAY_HANDSHAKE_TIMEOUT",
    "REASON_GROUP_KEY_UPDATE_TIMEOUT",
    "REASON_IE_IN_4WAY_DIFFERS",
    "REASON_GROUP_CIPHER_INVALID",
    "REASON_PAIRWISE_CIPHER_INVALID",
    "REASON_AKMP_INVALID",
    "REASON_UNSUPP_RSN_IE_VERSION",
    "REASON_INVALID_RSN_IE_CAP",
    "REASON_802_1X_AUTH_FAILED",
    "REASON_CIPHER_SUITE_REJECTED",
};

const char * const EVENT_REASONS_200[]
{
    "REASON_BEACON_TIMEOUT",
    "REASON_NO_AP_FOUND"
};
