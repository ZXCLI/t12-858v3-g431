// /* AT24C08 EEPROM驱动库
//  * 版本：2.0
//  * 特性：
//  * - 支持块/页/字节级读写
//  * - 自动处理跨页/跨块写入
//  * - 数据校验（CRC16）
//  * - 磨损均衡支持
//  * - 错误重试机制
//  * - 版本兼容性处理
//  */

// //#include "at24c08.h"
// //#include "crc16.h"  // CRC16校验库
// #include "main.h"
// #include <string.h>
// #include <assert.h>

// /* 常量定义 */
// #define AT24_BASE_ADDR        0xA0
// #define PAGE_SIZE             16
// #define MAX_RETRY             3
// #define WRITE_DELAY_MS        5
// #define EEPROM_VERSION        0x0103
// #define MAGIC_NUMBER          0x55AA1234
// #define WEAR_LEVELING_ENTRIES 16

// /* 类型定义 */
// #pragma pack(push, 1)
// typedef struct {
//     uint16_t version;
//     uint16_t config_size;
//     uint32_t magic_number;
//     uint16_t checksum;
// } EEPROM_Header;

// typedef struct {
//     uint8_t block;
//     uint8_t page;
//     uint16_t write_count;
// } WearLevelingEntry;

// typedef struct {
//     I2C_HandleTypeDef* hi2c;
//     uint8_t current_block;
//     uint8_t current_page;
//     uint16_t write_counter;
//     WearLevelingEntry wl_table[WEAR_LEVELING_ENTRIES];
// } AT24_Handle;
// #pragma pack(pop)

// /* 全局变量 */
// static AT24_Handle at24_handle;

// /* 私有函数声明 */
// static HAL_StatusTypeDef at24_PageWrite(uint8_t block, uint8_t page, uint8_t bit, uint8_t* data, uint16_t len);
// static void at24_UpdateWL(uint8_t block, uint8_t page);
// static uint16_t at24_CalculateChecksum(uint8_t* data, uint16_t len);
// static EEPROM_Status at24_VerifyWrite(uint8_t block, uint8_t page, uint8_t bit, uint8_t* data, uint16_t len);

// /* 初始化函数 */
// void AT24_Init(I2C_HandleTypeDef* hi2c) {
//     memset(&at24_handle, 0, sizeof(at24_handle));
//     at24_handle.hi2c = hi2c;
//     at24_handle.current_block = 0;
//     at24_handle.current_page = 0;
// }

// /* 页写入函数 */
// static HAL_StatusTypeDef at24_PageWrite(uint8_t block, uint8_t page, uint8_t bit, uint8_t* data, uint16_t len) {
//     HAL_StatusTypeDef status;
//     uint8_t retry = MAX_RETRY;
    
//     do {
//         uint8_t dev_addr = AT24_BASE_ADDR | (block << 1);
//         uint8_t mem_addr = (page << 4) | bit;
        
//         status = HAL_I2C_Mem_Write(at24_handle.hi2c, dev_addr, mem_addr, I2C_MEMADD_SIZE_8BIT, data, len, 100);
//         if(status == HAL_OK) {
//             uint32_t tickstart = HAL_GetTick();
//             while(HAL_GetTick() - tickstart < WRITE_DELAY_MS);
//             return HAL_OK;
//         }
        
//         HAL_I2C_DeInit(at24_handle.hi2c);
//         HAL_Delay(1);
//         MX_I2C1_Init();
//     } while(retry-- > 0);
    
//     return status;
// }

// /* 随机地址写入 */
// EEPROM_Status AT24_Write(uint8_t block, uint8_t page, uint8_t bit, uint8_t* data, uint16_t len) {
//     uint8_t current_block = block;
//     uint8_t current_page = page;
//     uint8_t current_bit = bit;
    
//     while(len > 0) {
//         uint16_t avail = PAGE_SIZE - current_bit;
//         uint16_t write_len = (len > avail) ? avail : len;
        
//         HAL_StatusTypeDef status = at24_PageWrite(current_block, current_page, current_bit, data, write_len);
//         if(status != HAL_OK) {
//             return EEPROM_HW_ERROR;
//         }
        
//         // 更新指针
//         len -= write_len;
//         data += write_len;
//         current_bit += write_len;
        
//         // 处理页/块边界
//         if(current_bit >= PAGE_SIZE) {
//             current_bit = 0;
//             if(++current_page >= 16) {
//                 current_page = 0;
//                 if(++current_block >= 4) {
//                     current_block = 0; // 循环覆盖
//                 }
//             }
//         }
//     }
    
//     at24_UpdateWL(block, page);
//     return at24_VerifyWrite(block, page, bit, data, len);
// }

// /* 随机地址读取 */
// EEPROM_Status AT24_Read(uint8_t block, uint8_t page, uint8_t bit, uint8_t* data, uint16_t len) {
//     uint8_t dev_addr = AT24_BASE_ADDR | (block << 1) | 0x01;
//     uint8_t mem_addr = (page << 4) | bit;
    
//     if(HAL_I2C_Mem_Read(at24_handle.hi2c, dev_addr, mem_addr, I2C_MEMADD_SIZE_8BIT, data, len, 100) != HAL_OK) {
//         return EEPROM_HW_ERROR;
//     }
    
//     return EEPROM_OK;
// }

// /* 磨损均衡更新 */
// static void at24_UpdateWL(uint8_t block, uint8_t page) {
//     for(int i=0; i<WEAR_LEVELING_ENTRIES; i++) {
//         if(at24_handle.wl_table[i].block == block && at24_handle.wl_table[i].page == page) {
//             at24_handle.wl_table[i].write_count++;
//             return;
//         }
//     }
    
//     uint16_t min_count = 0xFFFF;
//     uint8_t min_index = 0;
//     for(int i=0; i<WEAR_LEVELING_ENTRIES; i++) {
//         if(at24_handle.wl_table[i].write_count < min_count) {
//             min_count = at24_handle.wl_table[i].write_count;
//             min_index = i;
//         }
//     }
    
//     at24_handle.wl_table[min_index].block = block;
//     at24_handle.wl_table[min_index].page = page;
//     at24_handle.wl_table[min_index].write_count = 1;
// }

// /* 校验写入数据 */
// static EEPROM_Status at24_VerifyWrite(uint8_t block, uint8_t page, uint8_t bit, uint8_t* data, uint16_t len) {
//     uint8_t read_buf[PAGE_SIZE];
//     EEPROM_Status status = AT24_Read(block, page, bit, read_buf, len);
//     if(status != EEPROM_OK) {
//         return status;
//     }
    
//     if(memcmp(data, read_buf, len) != 0) {
//         return EEPROM_VERIFY_ERROR;
//     }
    
//     return EEPROM_OK;
// }

// /* 计算校验和 */
// static uint16_t at24_CalculateChecksum(uint8_t* data, uint16_t len) {
//     return CRC16_Calculate(data, len);
// }

// /* 写入配置数据 */
// EEPROM_Status AT24_WriteConfig(uint8_t* config, uint16_t len) {
//     EEPROM_Header header = {
//         .version = EEPROM_VERSION,
//         .config_size = len,
//         .magic_number = MAGIC_NUMBER,
//         .checksum = at24_CalculateChecksum(config, len)
//     };
    
//     // 写入头信息
//     EEPROM_Status status = AT24_Write(0, 0, 0, (uint8_t*)&header, sizeof(header));
//     if(status != EEPROM_OK) {
//         return status;
//     }
    
//     // 写入配置数据
//     return AT24_Write(0, 0, sizeof(header), config, len);
// }

// /* 读取配置数据 */
// EEPROM_Status AT24_ReadConfig(uint8_t* config, uint16_t len) {
//     EEPROM_Header header;
    
//     // 读取头信息
//     EEPROM_Status status = AT24_Read(0, 0, 0, (uint8_t*)&header, sizeof(header));
//     if(status != EEPROM_OK) {
//         return status;
//     }
    
//     // 验证头信息
//     if(header.magic_number != MAGIC_NUMBER || header.config_size != len) {
//         return EEPROM_FORMAT_ERROR;
//     }
    
//     // 读取配置数据
//     status = AT24_Read(0, 0, sizeof(header), config, len);
//     if(status != EEPROM_OK) {
//         return status;
//     }
    
//     // 校验数据
//     if(at24_CalculateChecksum(config, len) != header.checksum) {
//         return EEPROM_CHECKSUM_ERROR;
//     }
    
//     return EEPROM_OK;
// }

// /* 擦除整个EEPROM */
// void AT24_EraseFull() {
//     uint8_t blank[PAGE_SIZE] = {0xFF};
    
//     for(uint8_t block=0; block<4; block++) {
//         for(uint8_t page=0; page<16; page++) {
//             AT24_Write(block, page, 0, blank, PAGE_SIZE);
//         }
//     }
    
//     memset(&at24_handle.wl_table, 0, sizeof(at24_handle.wl_table));
// }