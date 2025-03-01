#ifndef __FLASH_PARTITION_TABLE__
#define __FLASH_PARTITION_TABLE__


//flash partition map,it's generated by the script based on the json file <flash_partition_table.json>

#define BOOT_SPACE_OFFSET                (0x00000000)
#define BOOT_SPACE_SIZE                  (1024*36)

#define PART_TAB_SPACE_OFFSET            (0x00009000)
#define PART_TAB_SPACE_SIZE              (1024*4)
#if (PART_TAB_SPACE_OFFSET < (BOOT_SPACE_OFFSET + BOOT_SPACE_SIZE))
  #error "flash partition overlap,please check <flash_partition_table.json>!!!"
#endif

#define KV_SPACE_OFFSET                  (0x0000A000)
#define KV_SPACE_SIZE                    (1024*8)
#if (KV_SPACE_OFFSET < (PART_TAB_SPACE_OFFSET + PART_TAB_SPACE_SIZE))
  #error "flash partition overlap,please check <flash_partition_table.json>!!!"
#endif

#define SIMU_EEPROM_SPACE_OFFSET         (0x0000C000)
#define SIMU_EEPROM_SPACE_SIZE           (1024*8)
#if (SIMU_EEPROM_SPACE_OFFSET < (KV_SPACE_OFFSET + KV_SPACE_SIZE))
  #error "flash partition overlap,please check <flash_partition_table.json>!!!"
#endif

#define APP_SPACE_OFFSET                 (0x0000E000)
#define APP_SPACE_SIZE                   (1024*800)
#if (APP_SPACE_OFFSET < (SIMU_EEPROM_SPACE_OFFSET + SIMU_EEPROM_SPACE_SIZE))
  #error "flash partition overlap,please check <flash_partition_table.json>!!!"
#endif

#define OTA_SPACE_OFFSET                 (0x000D6000)
#define OTA_SPACE_SIZE                   (1024*800)
#if (OTA_SPACE_OFFSET < (APP_SPACE_OFFSET + APP_SPACE_SIZE))
  #error "flash partition overlap,please check <flash_partition_table.json>!!!"
#endif

#define NVDS_SPACE_OFFSET                (0x0019E000)
#define NVDS_SPACE_SIZE                  (1024*12)
#if (NVDS_SPACE_OFFSET < (OTA_SPACE_OFFSET + OTA_SPACE_SIZE))
  #error "flash partition overlap,please check <flash_partition_table.json>!!!"
#endif

#define USER_SPACE_OFFSET                (0x001A1000)
#define USER_SPACE_SIZE                  (1024*12)
#if (USER_SPACE_OFFSET < (NVDS_SPACE_OFFSET + NVDS_SPACE_SIZE))
  #error "flash partition overlap,please check <flash_partition_table.json>!!!"
#endif


#define IMAGE_HEADER_SIZE                (0x100)

#endif /* __FLASH_PARTITION_TABLE__ */

