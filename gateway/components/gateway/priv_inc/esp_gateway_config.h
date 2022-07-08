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

/**
 * @brief Maximum MAC size
 *
 */
#define ESP_GATEWAY_MAC_MAX_LEN     (6)

/**
 * @brief Maximum SSID size
 *
 */
#define ESP_GATEWAY_SSID_MAX_LEN    (32)

#define ESP_GATEWAY_SOFTAP_SSID     CONFIG_ESP_GATEWAY_SOFTAP_SSID
#define ESP_GATEWAY_SOFTAP_PASSWORD CONFIG_ESP_GATEWAY_SOFTAP_PASSWORD

enum {
    ESP_GATEWAY_EXTERNAL_NETIF_INVALID = -1,
#ifdef CONFIG_GATEWAY_EXTERNAL_NETIF_STATION
    ESP_GATEWAY_EXTERNAL_NETIF_STATION,
#endif
#ifdef CONFIG_GATEWAY_EXTERNAL_NETIF_MODEM
    ESP_GATEWAY_EXTERNAL_NETIF_MODEM,
#endif
#ifdef CONFIG_GATEWAY_EXTERNAL_NETIF_ETHERNET
    ESP_GATEWAY_EXTERNAL_NETIF_ETHERNET,
#endif
    ESP_GATEWAY_EXTERNAL_NETIF_MAX
};
