/*
 * (C) 2007-09 - Luca Deri <deri@ntop.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not see see <http://www.gnu.org/licenses/>
 */

#include "n2n.h"

#ifdef _DARWIN_

void tun_close(tuntap_dev *device);

/* ********************************** */

#define N2N_OSX_TAPDEVICE_SIZE 32
int tuntap_open(tuntap_dev *device, struct tuntap_config* config) {  
    char tap_device[128];  
    int tap_fd;  
      
    // 打开 TAP 设备  
    snprintf(tap_device, sizeof(tap_device), "/dev/%s", config->if_name);  
    tap_fd = open(tap_device, O_RDWR);  
    if(tap_fd < 0) {  
        traceEvent(TRACE_ERROR, "Unable to open TAP device %s", tap_device);  
        return -1;  
    }  
      
    device->fd = tap_fd;  
      
    // 复制配置信息到设备结构  
    strncpy(device->dev_name, config->if_name, N2N_IFNAMSIZ);  
    memcpy(&device->ip_addr, &config->ip_addr, sizeof(config->ip_addr));  
    device->ip_prefixlen = config->ip_prefixlen;  
    memcpy(&device->ip6_addr, &config->ip6_addr, sizeof(config->ip6_addr));  
    device->ip6_prefixlen = config->ip6_prefixlen;  
    device->mtu = config->mtu;  
    device->routes_count = config->routes_count;  
    device->routes = config->routes;  
      
    // 设置 MAC 地址(如果提供)  
    if (!(config->device_mac[0] == 0 && config->device_mac[1] == 0 &&  
          config->device_mac[2] == 0 && config->device_mac[3] == 0 &&  
          config->device_mac[4] == 0 && config->device_mac[5] == 0)) {  
        memcpy(device->mac_addr, config->device_mac, 6);  
    }  
      
    return tap_fd;  
}

/* ********************************** */

ssize_t tuntap_read(struct tuntap_dev *tuntap, unsigned char *buf, size_t len) {  
    return read(tuntap->fd, buf, len);  
}

/* ********************************** */

ssize_t tuntap_write(struct tuntap_dev *tuntap, unsigned char *buf, size_t len) {  
    return write(tuntap->fd, buf, len);  
}

/* ********************************** */

void tuntap_close(struct tuntap_dev *tuntap) {
  close(tuntap->fd);
}

/* Fill out the ip_addr value from the interface. Called to pick up dynamic
 * address changes. */
void tuntap_get_address(struct tuntap_dev *tuntap)
{
}

#endif /* _DARWIN_ */
