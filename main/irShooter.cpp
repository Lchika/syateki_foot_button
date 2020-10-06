#include "irShooter.hpp"

/**
 * @brief コンストラクタ
 * @param[in] magazine_capacity 弾倉の容量(最大装填弾数)
 * @param[in] initial_bullets_num 初期装填弾数
 * @return None
 */
IrShooter::IrShooter(unsigned int magazine_capacity, unsigned int initial_bullets_num)
    : _magazine_capacity(magazine_capacity)
    , _loaded_bullets_num(initial_bullets_num)
{
  //  赤外線は出しっぱなしにする
  //pinMode(pin_output_ir, OUTPUT);
  //digitalWrite(pin_output_ir, HIGH);
};

/**
 * @brief 発射
 * @param None
 * @return bool true:正常に処理完了、false:異常終了
 */
bool IrShooter::shoot(){
  //  弾が装填されているか確認し、残弾数更新
  if(_loaded_bullets_num > 0){
    _loaded_bullets_num--;
  }else{
    //  弾がなければ異常としてfalseを返して終了
    return false;
  }
  
  return true;
}

/**
 * @brief 残弾数取得
 * @param
 * @return
 */
unsigned int IrShooter::get_bullets_num(){
  return _loaded_bullets_num;
}

/**
 * @brief リロード
 * @oaram
 * @return
 */
void IrShooter::reload(){
  _loaded_bullets_num = _magazine_capacity;
}