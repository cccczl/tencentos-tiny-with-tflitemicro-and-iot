-include $(SCRIPT_DIR)/internal_make_funcs.mk

SETTING_VARS := \
    BUILD_TYPE \
    PLATFORM_CC \
    PLATFORM_AR \
    PLATFORM_OS \

SWITCH_VARS := \
    FEATURE_MQTT_COMM_ENABLED \
    FEATURE_COAP_COMM_ENABLED \
    FEATURE_OTA_COMM_ENABLED \
    FEATURE_MQTT_DEVICE_SHADOW \
    FEATURE_AUTH_WITH_NOTLS \
    FEATURE_GATEWAY_ENABLED \
    FEATURE_LOG_UPLOAD_ENABLED \
    FEATURE_MULTITHREAD_ENABLED \
	FEATURE_DEV_DYN_REG_ENABLED \
    FEATURE_AT_TCP_ENABLED \
    FEATURE_AT_UART_RECV_IRQ \
    FEATURE_AT_OS_USED \
    FEATURE_AT_DEBUG \
    FEATURE_DEBUG_DEV_INFO_USED \
    FEATURE_OTA_USE_HTTPS \
    
$(foreach v, \
    $(SETTING_VARS) $(SWITCH_VARS), \
    $(eval export $(v)=$($(v))) \
)

$(foreach v, \
    $(SWITCH_VARS), \
    $(if $(filter y,$($(v))), \
        $(eval CFLAGS += -D$(subst FEATURE_,,$(v)))) \
)


ifeq (debug,$(strip $(BUILD_TYPE)))
CFLAGS  += -DIOT_DEBUG -g -O2
endif

ifneq (linux,$(strip $(PLATFORM_OS)))
ifeq (y,$(strip $(FEATURE_SDKTESTS_ENABLED)))
$(error FEATURE_SDKTESTS_ENABLED with gtest framework just supports to be enabled on PLATFORM_OS = linux!)
endif
else
ifeq (y,$(strip $(FEATURE_SDKTESTS_ENABLED)))
CFLAGS += -DSDKTESTS_ENABLED
endif
endif

ifeq (y,$(strip $(FEATURE_OTA_COMM_ENABLED)))
ifeq (MQTT,$(strip $(FEATURE_OTA_SIGNAL_CHANNEL)))
ifneq (y,$(strip $(FEATURE_MQTT_COMM_ENABLED)))
$(error FEATURE_OTA_SIGNAL_CHANNEL = MQTT requires FEATURE_MQTT_COMM_ENABLED = y!)
endif
CFLAGS += -DOTA_MQTT_CHANNEL
else
ifeq (COAP,$(strip $(FEATURE_OTA_SIGNAL_CHANNEL)))
ifneq (y,$(strip $(FEATURE_COAP_COMM_ENABLED)))
$(error FEATURE_OTA_SIGNAL_CHANNEL = COAP requires FEATURE_COAP_COMM_ENABLED = y!)
endif
CFLAGS += -DOTA_COAP_CHANNEL
else
$(error FEATURE_OTA_SIGNAL_CHANNEL must be MQTT or COAP!)
endif # COAP
endif # MQTT
endif # OTA Enabled

ifeq (CERT,$(strip $(FEATURE_AUTH_MODE)))
ifeq (y,$(strip $(FEATURE_AUTH_WITH_NOTLS)))
$(error FEATURE_AUTH_MODE = CERT requires FEATURE_AUTH_WITH_NOTLS = n!)
endif
CFLAGS += -DAUTH_MODE_CERT
else
ifeq (KEY,$(strip $(FEATURE_AUTH_MODE)))
CFLAGS += -DAUTH_MODE_KEY
else
$(error FEATURE_AUTH_MODE must be CERT or KEY!)
endif
endif # Auth mode

ifeq (y, $(strip $(FEATURE_SYSTEM_COMM_ENABLED)))
CFLAGS += -DSYSTEM_COMM
endif

ifeq (y, $(strip $(FEATURE_LOG_UPLOAD_ENABLED)))
CFLAGS += -DLOG_UPLOAD
endif

ifeq (y, $(strip $(FEATURE_AT_TCP_ENABLED)))
CFLAGS += -DAT_TCP_ENABLED
ifeq (y, $(strip $(FEATURE_AT_UART_RECV_IRQ)))
CFLAGS += -DAT_UART_RECV_IRQ
endif
ifeq (y, $(strip $(FEATURE_AT_OS_USED)))
CFLAGS += -DAT_OS_USED
endif
ifeq (y, $(strip $(FEATURE_AT_DEBUG)))
CFLAGS += -DFEATURE_AT_DEBUG
endif
endif

