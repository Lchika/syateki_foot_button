#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "http_request.h"

/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "192.168.100.117"
#define WEB_PORT 5000

static const char *TAG = "HTTP_REQ";

void http_get(char *body_buff, size_t body_buff_len, int gun_num)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct sockaddr_in server_addr;
    int s, r;
    char recv_buf[1024];

    /*
    int err = getaddrinfo(WEB_SERVER, "5000", &hints, &res);

    if(err != 0 || res == NULL) {
        ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        continue;
    }
    */

    /* Code to print the resolved IP.

        Note: inet_ntoa is non-reentrant, look at ipaddr_ntoa_r for "real" code */        
    s = socket(hints.ai_family, hints.ai_socktype, 0);
    if(s < 0) {
        ESP_LOGE(TAG, "... Failed to allocate socket.");
        //freeaddrinfo(res);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        //continue;
    }
    ESP_LOGI(TAG, "... allocated socket");

    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, WEB_SERVER, &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(WEB_PORT);

    if(connect(s, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) != 0) {
        ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
        close(s);
        //freeaddrinfo(res);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        //continue;
    }

    ESP_LOGI(TAG, "... connected");
    //freeaddrinfo(res);

    char request[1024];
    snprintf(request, 1024, "GET http://192.168.100.117/shoot/%d HTTP/1.0\r\n Host: 192.168.100.117\r\n User-Agent: esp-idf/1.0 esp32\r\n\r\n", gun_num);
    if (write(s, request, strlen(request)) < 0) {
        ESP_LOGE(TAG, "... socket send failed");
        close(s);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        //continue;
    }
    ESP_LOGI(TAG, "... socket send success");

    struct timeval receiving_timeout;
    receiving_timeout.tv_sec = 5;
    receiving_timeout.tv_usec = 0;
    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
            sizeof(receiving_timeout)) < 0) {
        ESP_LOGE(TAG, "... failed to set socket receiving timeout");
        close(s);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        //continue;
    }
    ESP_LOGI(TAG, "... set socket receiving timeout success");

    /* Read HTTP response */
    bzero(recv_buf, sizeof(recv_buf));
    size_t read_bytes = 0;
    do {
        r = read(s, recv_buf+read_bytes, sizeof(recv_buf)-1-read_bytes);
        /*
        for(int i = 0; i < r; i++) {
            putchar(recv_buf[i]);
        }
        */
        read_bytes += r;
    } while(r > 0 && read_bytes < sizeof(recv_buf));

    ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d.", r, errno);
    close(s);
    /*
    for(int countdown = 10; countdown >= 0; countdown--) {
        ESP_LOGI(TAG, "%d... ", countdown);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    ESP_LOGI(TAG, "Starting again!");
    */
    for(int i = 0; i < read_bytes-4; i++){
        if(recv_buf[i] == '\r' && recv_buf[i+1] == '\n'
            && recv_buf[i+2] == '\r' && recv_buf[i+3] == '\n'){
            if((read_bytes-(i+4)) < (body_buff_len-1)){
                memcpy(body_buff, recv_buf+i+4, read_bytes-(i+4));
                body_buff[read_bytes-(i+4)] = '\0';
            }else{
                memcpy(body_buff, recv_buf+i+4, body_buff_len-1);
                body_buff[body_buff_len-1] = '\0';
            }
            break;
        }
    }
    return;
}

void http_request_main()
{
    //ESP_ERROR_CHECK( nvs_flash_init() );
    //tcpip_adapter_init();
    //ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    //ESP_ERROR_CHECK(example_connect());

    //xTaskCreate(&http_get, "http_get_task", 4096, NULL, 5, NULL);
}
