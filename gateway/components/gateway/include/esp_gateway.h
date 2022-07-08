// Copyright 2022 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef CONFIG_LITEMESH_ENABLE
#include "esp_litemesh.h"
#endif

#if defined(CONFIG_GATEWAY_EXTERNAL_NETIF_MODEM)
/**
* @brief Create modem netif for gateway.
*
* @param[in] ip_info: custom ip address, if set NULL, it will automatically be assigned.
* @param[in] mac: custom mac address, if set NULL, it will automatically be assigned.
* @param[in] data_forwarding: whether to use as data forwarding netif
* @param[in] enable_dhcps: whether to enable DHCP server
*
* @return
*      - instance: the netif instance created successfully
*      - NULL: failed because some error occurred
*/
esp_netif_t* esp_gateway_create_modem_netif(esp_netif_ip_info_t* ip_info, uint8_t mac[6], bool data_forwarding, bool enable_dhcps);
#endif

#if defined(CONFIG_GATEWAY_EXTERNAL_NETIF_STATION)
/**
* @brief Create station netif for gateway.
*
* @param[in] ip_info: custom ip address, if set NULL, it will automatically be assigned.
* @param[in] mac: custom mac address, if set NULL, it will automatically be assigned.
* @param[in] data_forwarding: whether to use as data forwarding netif
* @param[in] enable_dhcps: whether to enable DHCP server
*
* @return
*      - instance: the netif instance created successfully
*      - NULL: failed because some error occurred
*/
esp_netif_t* esp_gateway_create_station_netif(esp_netif_ip_info_t* ip_info, uint8_t mac[6], bool data_forwarding, bool enable_dhcps);
#endif

#if defined(CONFIG_GATEWAY_EXTERNAL_NETIF_ETHERNET) || defined(CONFIG_GATEWAY_DATA_FORWARDING_NETIF_ETHERNET)
/**
* @brief Create eth netif for gateway.
*
* @param[in] ip_info: custom ip address, if set NULL, it will automatically be assigned.
* @param[in] mac: custom mac address, if set NULL, it will automatically be assigned.
* @param[in] data_forwarding: whether to use as data forwarding netif
* @param[in] enable_dhcps: whether to enable DHCP server
*
* @return
*      - instance: the netif instance created successfully
*      - NULL: failed because some error occurred
*/
esp_netif_t* esp_gateway_create_eth_netif(esp_netif_ip_info_t* ip_info, uint8_t mac[6], bool data_forwarding, bool enable_dhcps);
#endif

#if defined(CONFIG_GATEWAY_DATA_FORWARDING_NETIF_SOFTAP)
/**
* @brief Create softap netif for gateway.
*
* @param[in] ip_info: custom ip address, if set NULL, it will automatically be assigned.
* @param[in] mac: custom mac address, if set NULL, it will automatically be assigned.
* @param[in] data_forwarding: whether to use as data forwarding netif
* @param[in] enable_dhcps: whether to enable DHCP server
*
* @return
*      - instance: the netif instance created successfully
*      - NULL: failed because some error occurred
*/
esp_netif_t* esp_gateway_create_softap_netif(esp_netif_ip_info_t* ip_info, uint8_t mac[6], bool data_forwarding, bool enable_dhcps);
#endif

#if defined(CONFIG_GATEWAY_DATA_FORWARDING_NETIF_USB)
/**
* @brief Create usb netif for gateway.
*
* @param[in] ip_info: custom ip address, if set NULL, it will automatically be assigned.
* @param[in] mac: custom mac address, if set NULL, it will automatically be assigned.
* @param[in] data_forwarding: whether to use as data forwarding netif
* @param[in] enable_dhcps: whether to enable DHCP server
*
* @return
*      - instance: the netif instance created successfully
*      - NULL: failed because some error occurred
*/
esp_netif_t* esp_gateway_create_usb_netif(esp_netif_ip_info_t* ip_info, uint8_t mac[6], bool data_forwarding, bool enable_dhcps);
#endif

#if defined(CONFIG_GATEWAY_DATA_FORWARDING_NETIF_SDIO)
/**
* @brief Create sdio netif for gateway.
*
* @param[in] ip_info: custom ip address, if set NULL, it will automatically be assigned.
* @param[in] mac: custom mac address, if set NULL, it will automatically be assigned.
* @param[in] data_forwarding: whether to use as data forwarding netif
* @param[in] enable_dhcps: whether to enable DHCP server
*
* @return
*      - instance: the netif instance created successfully
*      - NULL: failed because some error occurred
*/
esp_netif_t* esp_gateway_create_sdio_netif(esp_netif_ip_info_t* ip_info, uint8_t mac[6], bool data_forwarding, bool enable_dhcps);
#endif

#if defined(CONFIG_GATEWAY_DATA_FORWARDING_NETIF_SPI)
/**
* @brief Create spi netif for gateway.
*
* @param[in] ip_info: custom ip address, if set NULL, it will automatically be assigned.
* @param[in] mac: custom mac address, if set NULL, it will automatically be assigned.
* @param[in] data_forwarding: whether to use as data forwarding netif
* @param[in] enable_dhcps: whether to enable DHCP server
*
* @return
*      - instance: the netif instance created successfully
*      - NULL: failed because some error occurred
*/
esp_netif_t* esp_gateway_create_spi_netif(esp_netif_ip_info_t* ip_info, uint8_t mac[6], bool data_forwarding, bool enable_dhcps);
#endif

/**
* @brief Create all netif which are enabled in menuconfig, for example, station, modem, ethernet.
*
*/
void esp_gateway_create_all_netif(void);
