/**
 * @file irShooter.hpp
 * @brief 赤外線銃としての機能を管理するクラスヘッダ
 */

#ifndef IR_SHOOTER_HPP
#define IR_SHOOTER_HPP

/**
 * @class IrShooter
 * @brief 赤外線銃としての機能を管理するクラス
 */
class IrShooter {
private:
  //int _pin_output_ir = -1;               //  赤外線出力用のピン番号
  unsigned int _magazine_capacity = 20;  //  弾倉の容量(最大装填弾数)
  unsigned int _loaded_bullets_num = 0;  //  現在の装填弾数

public:
  IrShooter(unsigned int magazine_capacity, unsigned int initial_bullets_num);
  bool shoot();
  unsigned int get_bullets_num();
  void reload();
};

#endif