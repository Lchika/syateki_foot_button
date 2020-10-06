#ifndef __BT_MAIN_H__
#define __BT_MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void bt_main(void * arg);
void bt_set_bullet_num(int bullet_num);
int bt_get_bullet_num(void);
void bt_set_gun_num(int gun_num);
int bt_get_main_state(void);

#ifdef __cplusplus
};
#endif // __cplusplus

#endif // __BT_MAIN_H__