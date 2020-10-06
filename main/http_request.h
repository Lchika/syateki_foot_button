#ifndef __HTTP_REQUEST_MAIN_H__
#define __HTTP_REQUEST_MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void http_get(char *body_buff, size_t body_buff_len, int gun_num);

#ifdef __cplusplus
};
#endif // __cplusplus

#endif // __HTTP_REQUEST_MAIN_H__